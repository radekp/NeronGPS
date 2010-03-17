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

#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_TYPE_GPSSAMPLE	(0)
#define OBJECT_TYPE_SATDATA	(1)

#include <QMutex>
#include <QDateTime>

class TObjectData
{
	public:
		TObjectData(int type) { _count = 1; _type = type; }
		~TObjectData() { }

		int type() { return _type; }
		void get() { QMutexLocker locker(&_mutex); _count++; }
		bool release() { QMutexLocker locker(&_mutex); _count--; return _count == 0; }
		bool isAlone() { return _count == 1; }

	public:
		QMutex _mutex;
		int _count;
		int _type;
};

class TObject
{
	public:
		TObject(const TObject &obj, int type);
		TObject(int type);
		virtual ~TObject();

		void modify();
		int type() const { return _type; }
		bool isValid() const { return _data != NULL; }
		TObject &operator= (const TObject &obj);

		virtual TObjectData *createData(TObjectData *data) = 0;
		virtual TObjectData *createData() = 0;

	protected:
		TObjectData *_data;
		TObjectData *data() const { if(_data != NULL) _data->get(); return _data; }

	private:
		int _type;

		void release();
};

#endif

