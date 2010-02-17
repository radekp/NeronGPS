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
#include <QMessageBox>

#include "include/tileref.h"

TTileRef::TTileRef(const QString &server, int x, int y, int zoom)
{
	_server = server;
	_x = x;
	_y = y;
	_zoom = zoom;
	_state = TTileRef::Valid;

	check();
}

TTileRef::TTileRef(const TTileRef &ref)
{
	_server = ref.server();
	_x = ref.x();
	_y = ref.y();
	_zoom = ref.zoom();
	_state = TTileRef::Valid;

	check();
}

TTileRef::TTileRef()
{
	_state = TTileRef::Empty;
}

TTileRef::~TTileRef()
{
}

TTileRef &TTileRef::operator= (const TTileRef &ref)
{
	_server = ref.server();
	_x = ref.x();
	_y = ref.y();
	_zoom = ref.zoom();
	_state = ref.state();

	return *this;	
}

bool TTileRef::operator== (const TTileRef &ref) const
{
	return (_state == TTileRef::Valid) && (_server == ref.server()) && (_x == ref.x()) && (_y == ref.y()) && (_zoom == ref.zoom());
}

bool TTileRef::operator!= (const TTileRef &ref) const
{
	return (_state != TTileRef::Valid) || (_server != ref.server()) || (_x != ref.x()) || (_y != ref.y()) || (_zoom != ref.zoom());
}

void TTileRef::check()
{
	if((_x < 0) || (_y < 0)) {
		_state = TTileRef::Invalid;
	}

	if(_x > (1 << _zoom) - 1) {
		_state = TTileRef::Invalid;
	}

	if(_y > (1 << _zoom) - 1) {
		_state = TTileRef::Invalid;
	}
}


