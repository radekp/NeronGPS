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

#include <QMutex>
#include <QDateTime>

class TGpsSampleShared
{
	public:
		TGpsSampleShared() { _count = 1; _altitudeValid = false; _speedValid = false; }
		~TGpsSampleShared() { }

		void get() { QMutexLocker locker(&_mutex); _count++; }
		bool release() { QMutexLocker locker(&_mutex); _count--; return _count == 0; }

	public:
		QMutex _mutex;
		int _count;

		QDateTime _time;
		double _latitude;
		double _longitude;

		bool _altitudeValid;
		float _altitude;
		bool _speedValid;
		float _speed;
};

class TGpsSample
{
	public:
		TGpsSample(const QDateTime &time, double latitude, double longitude);
		TGpsSample(const TGpsSample &sample);
		TGpsSample();
		~TGpsSample();

		void setAltitude(float altitude) { _data->_altitudeValid = true; _data->_altitude = altitude; }
		void setSpeed(float speed) { _data->_speedValid = true; _data->_speed = speed; }

		bool isValid() const { return _data != NULL; }

		double latitude() const { return _data->_latitude; }
		double longitude() const { return _data->_longitude; }
		QDateTime time() const { return _data->_time; }
		bool altitudeValid() const { return _data->_altitudeValid; }
		float altitude() const { return _data->_altitude; }
		bool speedValid() const { return _data->_speedValid; }
		float speed() const { return _data->_speed; }

		TGpsSample &operator= (const TGpsSample &sample);

	protected:
		TGpsSampleShared *data() const { if(_data != NULL) _data->get(); return _data; }

	private:
		TGpsSampleShared *_data;

		void release();
};

#endif

