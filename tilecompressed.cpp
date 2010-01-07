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

#include "include/tilecompressed.h"
#include "include/tile.h"

TTileCompressed::TTileCompressed()
{
}

TTileCompressed::TTileCompressed(const QByteArray &array) : QByteArray(array)
{
}

TTileCompressed::~TTileCompressed()
{
}

void TTileCompressed::setRef(const TTileRef &ref)
{
	_ref = ref;
}

TTile *TTileCompressed::uncompress()
{
	TTile *tile = new TTile();

	tile->setRef(_ref);

	if(!tile->loadFromData(*this, "PNG")) {
		if(!tile->loadFromData(*this, "JPG")) {
			delete tile;
			tile = NULL;
			qDebug() << "Error uncompressing tile: " << _ref.x() << _ref.y() << _ref.zoom();
		}
	}

	return tile;
}

