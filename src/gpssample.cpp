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

TGpsSample::TGpsSample(const QDateTime &time, double latitude, double longitude)
{
	_data = new TGpsSampleShared;
	
	_data->_time = time;
	_data->_longitude = longitude;
	_data->_latitude = latitude;
}

TGpsSample::TGpsSample(const TGpsSample &sample)
{
	_data = sample.data();
}

TGpsSample::TGpsSample()
{
	_data = NULL;
}

TGpsSample::~TGpsSample()
{
	release();
}

TGpsSample &TGpsSample::operator= (const TGpsSample &sample)
{
	release();

	_data = sample.data();

	return *this;
}

void TGpsSample::release()
{
	if(_data != NULL) {
		if(_data->release()) {
			delete _data;
		}
	}
}


