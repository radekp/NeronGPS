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
		TGpsSampleData() : TObjectData(OBJECT_TYPE_GPSSAMPLE) {
			_altitudeValid = false; _speedValid = false; _courseValid = false; _dopValid = false; _hdopValid = false;
		}
		~TGpsSampleData() { }

	public:
		QDateTime _time;
		double _latitude;
		double _longitude;

		bool _valid;
		bool _timeValid;
		bool _altitudeValid;
		bool _speedValid;
		bool _courseValid;
		bool _dopValid;
		bool _hdopValid;

		float _altitude;
		float _speed;
		float _course;
		float _pdop;
		float _hdop;
		float _vdop;
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

		void setTime(const QDateTime &time);
		void setPosition(double latitude, double longitude);
		void setAltitude(float altitude);
		void setSpeed(float speed);
		void setCourse(float course);
		void setDop(float pdop, float hdop, float vdop);
		void setHdop(float hdop);

		bool valid() const { return ((TGpsSampleData *)_data)->_valid; }
		bool timeValid() const { return ((TGpsSampleData *)_data)->_timeValid; }
		bool altitudeValid() const { return ((TGpsSampleData *)_data)->_altitudeValid; }
		bool speedValid() const { return ((TGpsSampleData *)_data)->_speedValid; }
		bool courseValid() const { return ((TGpsSampleData *)_data)->_courseValid; }
		bool dopValid() const { return ((TGpsSampleData *)_data)->_dopValid; }
		bool hdopValid() const { return ((TGpsSampleData *)_data)->_hdopValid; }

		double latitude() const { return ((TGpsSampleData *)_data)->_latitude; }
		double longitude() const { return ((TGpsSampleData *)_data)->_longitude; }
		QDateTime time() const { return ((TGpsSampleData *)_data)->_time; }
		float altitude() const { return ((TGpsSampleData *)_data)->_altitude; }
		float speed() const { return ((TGpsSampleData *)_data)->_speed; }
		float course() const { return ((TGpsSampleData *)_data)->_course; }
		float pdop() const { return ((TGpsSampleData *)_data)->_pdop; }
		float hdop() const { return ((TGpsSampleData *)_data)->_hdop; }
		float vdop() const { return ((TGpsSampleData *)_data)->_vdop; }
};

#endif

