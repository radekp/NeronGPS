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

TTraceRecorder::TTraceRecorder()
{
	_samples = -1;
	_state = stopped;
	_gpx = NULL;
	_onSeg = false;
}

TTraceRecorder::~TTraceRecorder()
{
	QMutexLocker locker(&_mutex);

	close();
}

void TTraceRecorder::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_dir = settings.getValue("tracedir", "/media/card/NeronGPS/traces").toString();
	bool record = settings.getValue("autorecord", "true").toBool();

	settings.endGroup();

	QDir::current().mkpath(_dir);

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

	if(!fix && _onSeg) {
		gpxEndSegment();
		_onSeg = false;
	}
}

void TTraceRecorder::slotGpsData(const QWhereaboutsUpdate &update)
{
	QMutexLocker locker(&_mutex);

	switch(_state)
	{
		case stopped:
			break;

		case starting:
			createFile(update);
			addSample(update);
			_state = started;
			break;

		case started:
			addSample(update);
			break;
	}
}

void TTraceRecorder::slotRecord(bool record)
{
	QMutexLocker locker(&_mutex);

	if(record) {
		if(_state == stopped) {
			_state = starting;
			_samples = 0;
			emit signalRecordInfo("Wait for fix", 0);
		}
	} else {
		if(_state == started) {
			tUserLog() << _filename << " ended";
		}
	
		_state = stopped;
		_samples = -1;

		close();

		emit signalRecordInfo("-", 0);
	}
}

void TTraceRecorder::slotNewTrack(QString name)
{
	QMutexLocker locker(&_mutex);

	if(_state == started) {
		QString str = (name == QString("")) ? QString("Transit") : name;
		gpxNewTrack(str);
		_samples = 0;
		emit signalRecordInfo(_filename, 0);
	}
}

void TTraceRecorder::slotNewWayPoint(QString name)
{
}

void TTraceRecorder::createFile(const QWhereaboutsUpdate &update)
{
	close();

	_filename = update.updateDate().toString("yyyyMMdd") + '_' + update.updateTime().toString("hhmmss");
	_gpx = new QFile(_dir + '/' + _filename + ".gpx");

	if(_gpx->open(QIODevice::WriteOnly)) {
		gpxHeader();

		tUserLog() << _filename << " started";

		emit signalRecordInfo(_filename, 0);
	} else {
		qDebug() << "Error opening trace file: " << _dir + '/' + _filename + ".gpx";

		delete _gpx;
		_gpx = NULL;

		emit signalRecordInfo("Recording error", 0);
	}
}

void TTraceRecorder::addSample(const QWhereaboutsUpdate &update)
{
	if(!_onSeg) {
		gpxStartSegment();
		_onSeg = true;
	}

	QString str;

	str = QString("<trkpt lat=\"");
	str += QString("%1").arg(update.coordinate().latitude(), 0, 'f', 10);
	str += QString("\" lon=\"");
	str += QString("%1").arg(update.coordinate().longitude(), 0, 'f', 10);
	str += QString("\">\n");

	if(update.coordinate().type() == QWhereaboutsCoordinate::Coordinate3D) {
		str += QString("  <ele>");
		str += QString::number(update.coordinate().altitude());
		str += QString("</ele>\n");
	}

	str += QString("  <time>");
	str += update.updateDateTime().toString("yyyy-MM-ddThh:mm:ssZ");
	str += QString("</time>\n");

	str += QString("</trkpt>\n");

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
		_samples ++;
		emit signalRecordInfo(_filename, _samples);
	}
}

void TTraceRecorder::gpxHeader()
{
	QString str;

	str = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	str += QString("<gpx\n");
	str += QString("  version=\"1.1\"\n");
	str += QString("  creator=\"NeronGPS\"\n");
	str += QString("  xmlns=\"http://www.topografix.com/GPX/1/1\"\n");
	str += QString("  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
	str += QString("  xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");

	str += QString("<trk> <name>Transit</name>\n");

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
	}
}

void TTraceRecorder::gpxFooter()
{
	QString str;

	str += QString("</trk>\n");
	str += QString("</gpx>\n");

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
	}
}

void TTraceRecorder::gpxNewTrack(QString &name)
{
	QString str;

	if(_onSeg) {
		str = QString("</trkseg>\n");
		str += QString("</trk>\n");
		str += QString("<trk> <name>") + name + QString("</name>\n");
		str += QString("<trkseg>\n");
	} else {
		str = QString("</trk>\n");
		str += QString("<trk> <name>") + name + QString("</name>\n");
	}

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
	}
}

void TTraceRecorder::gpxStartSegment()
{
	QString str;

	str = QString("<trkseg>\n");

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
	}
}

void TTraceRecorder::gpxEndSegment()
{
	QString str;

	str = QString("</trkseg>\n");

	if(_gpx != NULL) {
		_gpx->write(str.toAscii());
	}
}

void TTraceRecorder::close()
{
	if(_gpx != NULL) {
		if(_onSeg) {
			gpxEndSegment();
			_onSeg = false;
		}

		gpxFooter();

		delete _gpx;
		_gpx = NULL;
	}
}


