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

#include "include/object.h"

TObject::TObject(const TObject &obj, int type)
{
	TObjectData *data = obj.data();

	if(type == data->type()) {	
		_data = obj.data();
	} else {
		qDebug() << "Error, object construction with invalid type";
		_data = NULL;
	}
	_type = type;
}

TObject::TObject(int type)
{
	_data = NULL;
	_type = type;
}

TObject::~TObject()
{
	release();
}

void TObject::modify()
{
	if(_data == NULL) {
		_data = createData();
	} else if(!_data->isAlone()) {
		TObjectData *data = createData(_data);
		_data->release();
		_data = data;
	}
}

TObject &TObject::operator= (const TObject &obj)
{
	if(obj.type() == _type) {
		release();
		_data = obj.data();
	} else {
		qDebug() << "Error, '=' operation on 2 objects with different types";
	}

	return *this;
}

void TObject::release()
{
	if(_data != NULL) {
		if(_data->release()) {
			delete _data;
		}
	}
}


