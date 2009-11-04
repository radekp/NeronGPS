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

#include <math.h>

#include "include/gpsdata.h"
#include "include/converter.h"

TGpsData::TGpsData()
{
	_lastValid = false;
	_lastSkipped = false;
	_lastCourseValid = false;
}

TGpsData::~TGpsData()
{
}

void TGpsData::slotGpsData(const QWhereaboutsUpdate &update)
{
	qreal course = 0;
	bool courseValid = false;

	int newX = TConverter::prepareX(update.coordinate().longitude());
	int newY = TConverter::prepareY(update.coordinate().latitude());

	if(_lastValid && !_lastSkipped && (newX == _lastX) && (newX == _lastX)) {
		_lastSkipped = true;
		return;
	}

	_lastSkipped = false;
	
	if(update.dataValidityFlags() & QWhereaboutsUpdate::Course) {
		course = update.course();
		courseValid = true;
	} else if(_lastValid) {
		int xOffset, yOffset;
		float distance;
		
		xOffset = newX - _lastX;
		yOffset = newY - _lastY;

		distance = sqrt(xOffset * xOffset + yOffset * yOffset);

		if(distance != 0) {
			course = acos(xOffset / distance);
			course *= 180 / M_PI;
			if(yOffset < 0) {
				course = 360 - course;
			}

			course += 90;
			courseValid = true;
		}
	}

	if(courseValid)	{
		while(course >= 360) {
			course -= 360;
		}

		while(course < 0) {
			course += 360;
		}

		if(_lastCourseValid) {
			qreal diff = course - _lastCourse;

			if(diff > 180) {
				diff = diff - 360;
			}

			if(diff < -180) {
				diff = diff + 360;
			}

			if(diff > 20) {
				diff = 20;
			}

			if(diff < -20) {
				diff = -20;
			}

			course = _lastCourse + diff;
		}
	} else {
		if(_lastCourseValid) {
			course = _lastCourse;
			courseValid = true;
		}
	}

	if(courseValid)	{
		emit signalGpsData(newX, newY, course, true);
	} else {
		emit signalGpsData(newX, newY, 0, false);
	}

	_lastX = newX;
	_lastY = newY;
	_lastCourse = course;
	_lastCourseValid = courseValid;
	_lastValid = true;
}

