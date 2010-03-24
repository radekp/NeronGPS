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

#include "include/gpssample.h"

TGpsSample::TGpsSample(const QDateTime &time, double latitude, double longitude) : TObject(OBJECT_TYPE_GPSSAMPLE)
{
	TObject::modify();	

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_valid = true;
	data->_timeValid = true;
	data->_time = time;
	data->_longitude = longitude;
	data->_latitude = latitude;
}

TObjectData *TGpsSample::createData()
{
	return new TGpsSampleData;
}

TObjectData *TGpsSample::createData(TObjectData *data)
{
	TGpsSampleData *data1 = (TGpsSampleData *)data;
	TGpsSampleData *data2 = new TGpsSampleData;


	data2->_time = data1->_time;
	data2->_latitude = data1->_latitude;
	data2->_longitude = data1->_longitude;

	data2->_valid = data1->_valid;
	data2->_timeValid = data1->_timeValid;
	data2->_altitudeValid = data1->_altitudeValid;
	data2->_speedValid = data1->_speedValid;
	data2->_courseValid = data1->_courseValid;
	data2->_dopValid = data1->_dopValid;
	data2->_hdopValid = data1->_hdopValid;

	data2->_altitude = data1->_altitude;
	data2->_speed = data1->_speed;
	data2->_course = data1->_course;
	data2->_pdop = data1->_pdop;
	data2->_hdop = data1->_hdop;
	data2->_vdop = data1->_vdop;

	return data2;
}

void TGpsSample::setTime(const QDateTime &time)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_timeValid = true;
	data->_time = time;
}

void TGpsSample::setPosition(double latitude, double longitude)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_valid = true;
	data->_longitude = longitude;
	data->_latitude = latitude;
}

void TGpsSample::setAltitude(float altitude)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_altitudeValid = true;
	data->_altitude = altitude;
}

void TGpsSample::setSpeed(float speed)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_speedValid = true;
	data->_speed = speed;
}

void TGpsSample::setCourse(float course)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_courseValid = true;
	data->_course = course;
}

void TGpsSample::setDop(float pdop, float hdop, float vdop)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_dopValid = true;
	data->_hdopValid = true;
	data->_pdop = pdop;
	data->_hdop = hdop;
	data->_vdop = vdop;
}

void TGpsSample::setHdop(float hdop)
{
	modify();

	TGpsSampleData *data = (TGpsSampleData *)_data;

	data->_hdopValid = true;
	data->_hdop = hdop;
}

