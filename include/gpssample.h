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

#ifndef GPSSAMPLE_H
#define GPSSAMPLE_H

#include <QDateTime>

#include "include/object.h"

class TGpsSampleData : public TObjectData
{
	public:
		TGpsSampleData() : TObjectData(OBJECT_TYPE_GPSSAMPLE) { _altitudeValid = false; _speedValid = false; }
		~TGpsSampleData() { }

	public:
		QDateTime _time;
		double _latitude;
		double _longitude;

		bool _altitudeValid;
		float _altitude;
		bool _speedValid;
		float _speed;
};

class TGpsSample : public TObject
{
	public:
		TGpsSample(const QDateTime &time, double latitude, double longitude);
		TGpsSample(const TGpsSample &sample) : TObject(sample, OBJECT_TYPE_GPSSAMPLE) { }
		TGpsSample() : TObject(OBJECT_TYPE_GPSSAMPLE) { }
		~TGpsSample() { }

		TObjectData *createData(TObjectData *data);
		TObjectData *createData();

		void setAltitude(float altitude);
		void setSpeed(float speed);

		double latitude() const { return ((TGpsSampleData *)_data)->_latitude; }
		double longitude() const { return ((TGpsSampleData *)_data)->_longitude; }
		QDateTime time() const { return ((TGpsSampleData *)_data)->_time; }
		bool altitudeValid() const { return ((TGpsSampleData *)_data)->_altitudeValid; }
		float altitude() const { return ((TGpsSampleData *)_data)->_altitude; }
		bool speedValid() const { return ((TGpsSampleData *)_data)->_speedValid; }
		float speed() const { return ((TGpsSampleData *)_data)->_speed; }
};

#endif

