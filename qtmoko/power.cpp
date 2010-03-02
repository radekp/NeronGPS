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
#include <QFile>

#include "qtmoko/power.h"

bool TPower::gpsGetPower()
{
	bool ret = false;

	QFile file("/sys/class/i2c-adapter/i2c-0/0-0073/pcf50633-regltr.7/neo1973-pm-gps.0/power_on");

	if(file.open(QIODevice::ReadOnly)) {
		char pow = (file.read(1))[0];
		if(pow == '1') {
			ret = true;
		} else if(pow != '0') {
			qDebug() << "Error, invalid gps power status, got: " << pow;
		}
	} else {
		qDebug() << "Error opening file to get gps power status";
	}

	return ret;
}

void TPower::gpsSetPower(bool power)
{
	QFile file("/sys/class/i2c-adapter/i2c-0/0-0073/pcf50633-regltr.7/neo1973-pm-gps.0/power_on");

	if(file.open(QIODevice::WriteOnly)) {
		QString str = (power) ? QString('1') : QString('0');
		file.write(str.toAscii());
	} else {
		qDebug() << "Error opening file to get gps power status";
	}
}

