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

#include "include/gpsstatistics.h"
#include "include/converter.h"

TGpsStatistics::TGpsStatistics()
{
	_accuracy = 5;
	_lostTime = 20;

	slotReset();
}

TGpsStatistics::~TGpsStatistics()
{
}

void TGpsStatistics::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_accuracy = settings.getValue("accuracyfordistance", 5).toInt();
	_lostTime = settings.getValue("timebeforelost", 20).toInt();

	settings.endGroup();
}

void TGpsStatistics::slotGpsData(const QWhereaboutsUpdate &update)
{
	QMutexLocker locker(&_mutex);

	uint newTime = update.updateDateTime().toTime_t();

	if(_first) {
		_lastLat = update.coordinate().latitude();
		_lastLon = update.coordinate().longitude();
		_lastTime = newTime;
		_beginning = _lastTime;
		_lastSampleTime = _lastTime;
		_first = false;
	} else if (newTime > _lastTime) {

		if(update.coordinate().type() == QWhereaboutsCoordinate::Coordinate3D) {
			_altitude = update.coordinate().altitude();

			if(_altMin == STAT_INVALID_ALTITUDE) {
				_altMin = _altMax = _altitude;
			} else if(_altitude < _altMin) {
				_altMin = _altitude;
			} else if(_altitude > _altMax) {
				_altMax = _altitude;
			}
		}

		if(update.dataValidityFlags() & QWhereaboutsUpdate::GroundSpeed) {
			_speed = update.groundSpeed() * 3.6;
		}

		float altitude = (_altitude == STAT_INVALID_ALTITUDE) ? 0 : _altitude;
		float dist = TConverter::distance(_lastLat, _lastLon, update.coordinate().latitude(), update.coordinate().longitude(), altitude);

		if(dist > _accuracy) {
			_distance += dist;
			_lastLat = update.coordinate().latitude();
			_lastLon = update.coordinate().longitude();
			_lastSampleTime = newTime;
		}

		if((newTime - _lastTime) > _lostTime) {
			uint lostTime = newTime - _lastTime;
			_fixLost += lostTime;
		}

		_lastTime = newTime;

		int interval = (int)(newTime - _beginning);
		emit signalStatistics(interval, (int)_distance, (int)_speed, (int)_altitude, (int)_altMin, (int)_altMax, ((interval - _fixLost) * 100) / interval);
	}
}

void TGpsStatistics::resend()
{
	QMutexLocker locker(&_mutex);

	int interval = (int)(_lastTime - _beginning);
	if(interval != 0) {
		emit signalStatistics(interval, (int)_distance, _speed, (int)_altitude, (int)_altMin, (int)_altMax, ((interval - _fixLost) * 100) / interval);
	} else {
		emit signalStatistics(interval, (int)_distance, _speed, (int)_altitude, (int)_altMin, (int)_altMax, 100);
	}
}

void TGpsStatistics::slotReset()
{
	QMutexLocker locker(&_mutex);

	_beginning = 0;
	_lastTime = 0;

	_distance = 0;
	_lastSampleTime = 0;

	_altitude = STAT_INVALID_ALTITUDE;
	_altMin = STAT_INVALID_ALTITUDE;
	_altMax = STAT_INVALID_ALTITUDE;

	_speed = STAT_INVALID_SPEED;

	_fixLost = 0;

	_first = true;

	emit signalStatistics(0, (int)_distance, _speed, (int)_altitude, (int)_altMin, (int)_altMax, 100);
}

