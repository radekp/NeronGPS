/*
 * Copyright 2009, 2010 Thierry Vuillaume
 *
 * This file is part of NeronGPS.
 *
 * NeronGPS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NeronGPS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NeronGPS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>

#include "include/tracerecorder.h"
#include "include/converter.h"
#include "include/directory.h"
#include "include/global.h"
#include "include/settings.h"
#include "include/gpxbuilder.h"

TTraceRecorder::TTraceRecorder()
{
	_samples = -1;
	_recordState = stopped;
	_tracks = NULL;
	_waypoints = NULL;
}

TTraceRecorder::~TTraceRecorder()
{
	QMutexLocker locker(&_mutex);

	close();
}

void TTraceRecorder::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_dir = settings.getValue("recorddir", settings.rootDir() + QString("/traces/records")).toString();
	_tmpDir = settings.getValue("tempdir", settings.rootDir() + QString("/traces/temp")).toString();

	bool record = settings.getValue("autorecord", "true").toBool();

	settings.endGroup();

	QDir::current().mkpath(_dir);
	QDir::current().mkpath(_tmpDir);

	if(record) {
		QMessageBox dialog(QMessageBox::Question, "Auto-recording enabled", "Start recording?", QMessageBox::Yes | QMessageBox::No);
		if(dialog.exec() == QMessageBox::Yes) {
			slotRecord(true);
		}
	}
}

void TTraceRecorder::slotGpsState(bool fix)
{
	QMutexLocker locker(&_mutex);

	if(!fix && (_trackState == onSeg)) {
		_tracks->write(QString("    </trkseg>\n").toAscii());
		_trackState = onTrack;
	}
}

void TTraceRecorder::slotGpsSample(TGpsSample sample)
{
	QMutexLocker locker(&_mutex);

	switch(_recordState)
	{
		case starting:
			if(createFile(sample)) {
				_recordState = started;
				addSample(sample);
			} else {
				_recordState = stopped;
			}
			break;

		case started:
			addSample(sample);
			break;

		default:
			break;
	}
}

void TTraceRecorder::slotRecord(bool record)
{
	QMutexLocker locker(&_mutex);

	if(record) {
		if(_recordState == stopped) {
			_recordState = starting;
			_samples = 0;
			emit signalRecordInfo("Wait for fix", 0);
		}
	} else {
		if(_recordState == started) {
			tUserLog() << _filename << " ended";
		}
	
		_recordState = stopped;
		_samples = -1;

		close();

		emit signalRecordInfo("-", 0);
	}
}

void TTraceRecorder::slotNewTrack(QString name)
{
	QMutexLocker locker(&_mutex);

	if(_recordState == started) {
		endTrack();
		_trackName = (name == QString("")) ? QString("Transit") : name;

		_samples = 0;
		emit signalRecordInfo(_filename, 0);
	}
}

void TTraceRecorder::slotNewWayPoint(QString name)
{
	QMutexLocker locker(&_mutex);

	if(_waypoints != NULL) {
		_pendingWaypoints += name;
	}
}

bool TTraceRecorder::createFile(const TGpsSample &sample)
{
	bool ret;

	_filename = sample.time().date().toString("yyyyMMdd") + '_' + sample.time().time().toString("hhmmss");
	_tracks = new QFile(_tmpDir + '/' + _filename + ".trk");
	_waypoints = new QFile(_tmpDir + '/' + _filename + ".wpt");

	if(ret = (_tracks->open(QIODevice::WriteOnly) && _waypoints->open(QIODevice::WriteOnly))) {
		_trackName = QString("Transit");
		_trackState = out;
		_firstSample = true;

		tUserLog() << _filename << " started";

		emit signalRecordInfo(_filename, 0);
	} else {
		qDebug() << "Error opening trace file: " << _tmpDir + '/' + _filename + ".trk";

		delete _tracks;
		delete _waypoints;
		_tracks = NULL;
		_waypoints = NULL;

		emit signalRecordInfo("Recording error", 0);
	}

	return ret;
}

void TTraceRecorder::addSample(const TGpsSample &sample)
{
	if(_firstSample) {
		_firstSample = false;
		_minLat = _maxLat = sample.latitude();
		_minLon = _maxLon = sample.longitude();
	} else {
		_minLat = (_minLat < sample.latitude()) ? _minLat : sample.latitude();
		_maxLat = (_maxLat > sample.latitude()) ? _maxLat : sample.latitude();
		_minLon = (_minLon < sample.longitude()) ? _minLon : sample.longitude();
		_maxLon = (_maxLon > sample.longitude()) ? _maxLon : sample.longitude();
	}

	if(_trackState == out) {
		QString str = QString("\n  <trk>\n    <name>") + _trackName + QString("</name>\n    <trkseg>\n");
		_tracks->write(str.toAscii());
	} else if(_trackState == onTrack) {
		_tracks->write(QString("    <trkseg>\n").toAscii());
	}
	_trackState = onSeg;

	QString str;
	QString latitude = QString("%1").arg(sample.latitude(), 0, 'f', 10);
	QString longitude = QString("%1").arg(sample.longitude(), 0, 'f', 10);
	QString altitude = (sample.altitudeValid()) ? QString::number(sample.altitude()) : QString("nan");

	while(_pendingWaypoints.count() > 0) {
		str = QString("\n  <wpt lat=\"") + latitude + QString("\" lon=\"") + longitude + QString("\">\n");
		str += QString("    <name>") + _pendingWaypoints.takeFirst() + QString("</name>\n");
		if(sample.altitudeValid()) { str += QString("    <ele>") + altitude + QString("</ele>\n"); }
		str += QString("  </wpt>\n");
		_waypoints->write(str.toAscii());
	}

	str = QString("      <trkpt lat=\"") + latitude + QString("\" lon=\"") + longitude + QString("\">\n");
	if(sample.altitudeValid()) { str += QString("        <ele>") + altitude + QString("</ele>\n"); }
	str += QString("        <time>") + sample.time().toString("yyyy-MM-ddThh:mm:ssZ") + QString("</time>\n");
	str += QString("      </trkpt>\n");
	_tracks->write(str.toAscii());

	_samples ++;
	emit signalRecordInfo(_filename, _samples);
}

void TTraceRecorder::close()
{
	if(_tracks != NULL) {
		endTrack();

		delete _tracks;
		delete _waypoints;
		_tracks = NULL;
		_waypoints = NULL;

		QFile meta(_tmpDir + '/' + _filename + ".met");
		if(meta.open(QIODevice::WriteOnly)) {
			QString str;
			str = QString("\n  <metadata>\n");
			str += QString("    <link href=\"http://tvuillaume.free.fr/NeronGPS\">\n");
			str += QString("      <text>NeronGPS</text>\n");
			str += QString("    </link>\n");
			if(!_firstSample) {
				str += QString("    <bounds");
				str += QString(" minlat=\"") + QString("%1").arg(_minLat, 0, 'f', 10) + QString("\"");
				str += QString(" minlon=\"") + QString("%1").arg(_minLon, 0, 'f', 10) + QString("\"");
				str += QString(" maxlat=\"") + QString("%1").arg(_maxLat, 0, 'f', 10) + QString("\"");
				str += QString(" maxlon=\"") + QString("%1").arg(_maxLon, 0, 'f', 10) + QString("\"");
				str += QString("/>\n");
			}
			str += QString("  </metadata>\n");

			meta.write(str.toAscii());
			meta.close();
		}

		QStringList input;
		input += QString(_tmpDir + '/' + _filename + ".met");
		input += QString(_tmpDir + '/' + _filename + ".wpt");
		input += QString(_tmpDir + '/' + _filename + ".trk");

		TGpxBuilder *builder = new TGpxBuilder(_dir + '/' + _filename + ".gpx", input);
		_threads.addThread(builder);
	}
}

void TTraceRecorder::endTrack()
{
	QString str;
	if(_trackState == onSeg) {
		_tracks->write(QString("    </trkseg>\n  </trk>\n").toAscii());
	} else if(_trackState == onTrack) {
		_tracks->write(QString("  </trk>\n").toAscii());
	}

	_trackState = out;
}


