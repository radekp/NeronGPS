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
#include <QFile>

#include "include/power.h"
#include "include/global.h"

bool TPower::getGPS(void)
{
	return TPower::getPower("/sys/bus/platform/drivers/neo1973-pm-gps/neo1973-pm-gps.0/pwron");
}

void TPower::setGPS(bool power)
{
	if(power) {
		tUserLog() << "GPS power on";
	} else {
		tUserLog() << "GPS power off";
	}

	return TPower::setPower("/sys/bus/platform/drivers/neo1973-pm-gps/neo1973-pm-gps.0/pwron", power);
}

bool TPower::getGSM(void)
{
	return TPower::getPower("/sys/bus/platform/drivers/neo1973-pm-gsm/neo1973-pm-gsm.0/power_on");
}

void TPower::setGSM(bool power)
{
	if(power) {
		tUserLog() << "GSM power on";
	} else {
		tUserLog() << "GSM power off";
	}

	return TPower::setPower("/sys/bus/platform/devices/neo1973-pm-gsm.0/power_on", power);
}

bool TPower::getBT(void)
{
	return TPower::getPower("/sys/bus/platform/devices/neo1973-pm-bt.0/power_on");
}

void TPower::setBT(bool power)
{
	if(power) {
		tUserLog() << "Bluetooth power on";
	} else {
		tUserLog() << "Bluetooth power off";
	}

	return TPower::setPower("/sys/bus/platform/devices/neo1973-pm-bt.0/power_on", power);
}

bool TPower::getPower(const char path[])
{
	bool power = false;
	char value;

	QFile file(path);

	if (!file.open(QIODevice::ReadOnly)) {
		qDebug("Error opening power state file");
	}
	else {
		if(file.read(&value, 1) != 1) {
			qDebug("Error reading power state");
		}
		else {
			power = (value == '1') ? true : false;			
		}
	}

	return power;
}

void TPower::setPower(const char path[], bool power)
{
	char value = power ? '1' : '0';

	QFile file(path);

	if (!file.open(QIODevice::WriteOnly)) {
		qDebug("Error opening power state file");
	}
	else {
		if(file.write(&value, 1) != 1) {
			qDebug("Error writing power state");
		}
	}
}


