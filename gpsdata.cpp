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

#include <math.h>

#include "include/gpsdata.h"
#include "include/converter.h"

TGpsData::TGpsData()
{
	_lastValid = false;
	_lastSkipped = false;
	_lastCourse = 0;
	_altitude = 0;
	_accuracy = 5;
	_rotationSpeed = 20;
}

TGpsData::~TGpsData()
{
}

void TGpsData::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_accuracy = settings.getValue("accuracyforcourse", 5).toInt();
	_rotationSpeed = settings.getValue("rotationspeed", 20).toInt();

	settings.endGroup();
}

void TGpsData::slotGpsData(const QWhereaboutsUpdate &update)
{
	int newX = TConverter::prepareX(update.coordinate().longitude());
	int newY = TConverter::prepareY(update.coordinate().latitude());

	if(_lastValid && !_lastSkipped && (newX == _lastX) && (newX == _lastX)) {
		_lastSkipped = true;
		return;
	} else {
		_lastSkipped = false;
	}
	
	qreal course = 0;
	bool courseValid = false;

	if(update.coordinate().type() == QWhereaboutsCoordinate::Coordinate3D) {
		_altitude = update.coordinate().altitude();
	}

	bool noise = true;
	if(_lastValid) {
		float dist = TConverter::distance(_lastLat, _lastLon, update.coordinate().latitude(), update.coordinate().longitude(), _altitude);
		if(dist > _accuracy) {
			int xOffset = newX - _lastX;
			int yOffset = newY - _lastY;

			float distance = sqrt(xOffset * xOffset + yOffset * yOffset);

			if(distance != 0) {
				course = acos(xOffset / distance);
				course *= 180 / M_PI;
				if(yOffset < 0) {
					course = 360 - course;
				}

				course += 90;
			}

			_lastLat = update.coordinate().latitude();
			_lastLon = update.coordinate().longitude();
			_lastX = newX;
			_lastY = newY;
			noise = false;
			courseValid = true;
		}
	} else {
		_lastLat = update.coordinate().latitude();
		_lastLon = update.coordinate().longitude();
		_lastX = newX;
		_lastY = newY;
		_lastValid = true;
	}

	if(update.dataValidityFlags() & QWhereaboutsUpdate::Course) {
		course = update.course();
		courseValid = true;
	}

	if(courseValid)	{
		while(course >= 360) { course -= 360; }
		while(course < 0) { course += 360; }

		qreal diff = course - _lastCourse;
		if(diff > 180) { diff = diff - 360; }
		if(diff < -180) { diff = diff + 360; }
		if(diff > _rotationSpeed) { diff = _rotationSpeed; }
		if(diff < -_rotationSpeed) { diff = -_rotationSpeed; }

		course = _lastCourse + diff;
	} else {
		course = _lastCourse;
	}

	emit signalGpsData(noise, newX, newY, course);

	_lastCourse = course;
}

