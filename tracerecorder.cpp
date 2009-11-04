/*
 * Copyright 2009 Thierry Vuillaume
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

TTraceRecorder::TTraceRecorder()
{
	_samples = -1;
	_state = stopped;
	_file = NULL;
	_bin = NULL;
}

TTraceRecorder::~TTraceRecorder()
{
	flush();
}

void TTraceRecorder::setDir(const QString &dir)
{
	_dirName = dir;
}

void TTraceRecorder::newGpsData(const QWhereaboutsUpdate &update)
{
	QMutexLocker locker(&_mutex);

	switch(_state)
	{
		case stopped:
			break;

		case starting:
			selectNextName(update.updateDate().toString("yyyyMMdd"));
			createFile(update.updateTime().toString("hhmmss"));
			addSample(update);
			_state = started;
			break;

		case reseting:
			createFile(update.updateTime().toString("hhmmss"));
			addSample(update);
			_state = started;
			break;

		case started:
			addSample(update);
			break;
	}
}

void TTraceRecorder::start()
{
	QMutexLocker locker(&_mutex);

	if(_state == stopped) {
		_state = starting;
		_samples = 0;
		emit signalSample(_samples);
	}
}

void TTraceRecorder::stop()
{
	QMutexLocker locker(&_mutex);

	if(_state == started) {
		tUserLog() << _filename << " ended";
	}
	
	_state = stopped;
	_samples = -1;

	flush();
}

void TTraceRecorder::reset()
{
	QMutexLocker locker(&_mutex);

	if(_state == started) {
		_state = reseting;
		_samples = 0;
		emit signalSample(0);
	}
}

void TTraceRecorder::selectNextName(const QString &day)
{
	QString ret;
	TDirectory dir;

	dir.setDir(_dirName);
	QStringList files = dir.loadFiles(day + "_??_??????.*", true, true);

	if(files.size() == 0) {
		ret = "00";
	} else {
		int tmp = files[0].mid(9, 2).toInt() + 1;
		ret = QString("%1").arg(tmp, (int)2, (int)10, QLatin1Char('0'));
	}

	_shortName = day + '_' + ret + '_';
}

void TTraceRecorder::createFile(const QString &time)
{
	flush();

	_filename = _shortName + time;
	
	QString fullName = _dirName + '/' + _filename ;
	_file = new QFile(fullName + ".log");
	_bin = new QFile(fullName + ".bin");

	if(!_file->open(QIODevice::WriteOnly) || !_bin->open(QIODevice::WriteOnly)) {
		qDebug() << "Error opening trace file: " << fullName;
		delete _file;
		_file = NULL;
		delete _bin;
		_bin = NULL;
		emit signalSample(-1);
	} else {
		tUserLog() << _filename << " started";
		emit signalSample(0);
	}
}

void TTraceRecorder::addSample(const QWhereaboutsUpdate &update)
{
	if(_file !=NULL) {
		QDateTime time = update.updateDateTime();
		QString str;


		str = QString("%1").arg(update.coordinate().latitude(), 0, 'f', 10);
		str += ',';

		str += QString("%1").arg(update.coordinate().longitude(), 0, 'f', 10);
		str += ',';

		if(update.coordinate().type() == QWhereaboutsCoordinate::Coordinate3D) {
			str += QString::number(update.coordinate().altitude());
		} else {
			str += "0.0";
		}
		str += ',';

		if(update.dataValidityFlags() & QWhereaboutsUpdate::GroundSpeed) {
			str += QString::number(update.groundSpeed() * 60 / 1000);
		} else {
			str += "0.0";
		}
		str += ',';

		if(update.dataValidityFlags() & QWhereaboutsUpdate::Course) {
			str += QString::number(update.course());
		} else {
			str += "0.0";
		}
		str += ",1.0,"; /* unknown hdop */

		str += time.toString("yyyy-MM-ddThh:mm:ssZ");
		str += '\n';

		_concatLog += str;

		int tmp[2];
		tmp[0] = TConverter::prepareX(update.coordinate().longitude());
		tmp[1] = TConverter::prepareY(update.coordinate().latitude());

		_concatBin += QByteArray((char *)tmp, 2 * sizeof(int));

		_concatCount --;
		if(_concatCount <= 0) {
			_file->write(_concatLog.toAscii());
			_concatLog.clear();
			_bin->write(_concatBin);
			_concatBin.clear();
			_concatCount = TRACERECORDER_CONCATENATION;
		}

		_samples ++;
		emit signalSample(_samples);
	}	
}

void TTraceRecorder::flush()
{
	if((_concatLog.size() > 0) && (_file != NULL)) {
		_file->write(_concatLog.toAscii());
	}

	if((_concatBin.size() > 0) && (_bin != NULL)) {
		_bin->write(_concatBin);
	}

	if(_file != NULL) {
		delete _file;
		_file = NULL;
	}

	if(_bin != NULL) {
		delete _bin;
		_bin = NULL;
	}

	_concatLog.clear();
	_concatBin.clear();
	_concatCount = TRACERECORDER_CONCATENATION;
}

