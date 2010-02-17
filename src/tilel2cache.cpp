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
#include <QDir>

#include "include/tilel2cache.h"

TTileL2Cache::TTileL2Cache()
{
}

TTileL2Cache::~TTileL2Cache()
{
}

void TTileL2Cache::addTile(TTileCompressed *tile)
{
	QMutexLocker locker(&_mutex);

	_tiles.enqueue(tile);

	while(_tiles.size() > MEMORYCACHESIZE) {
		delete _tiles.dequeue();
	}
}

void TTileL2Cache::refresh(const TTileRef &ref)
{
	int i;

	QMutexLocker locker(&_mutex);

	for(i = 0; (i < _tiles.size()) && (!_tiles[i]->compare(ref)); i++);

	if(i != _tiles.size()) {
		_tiles.enqueue(_tiles.takeAt(i));
	}
}

TTileCompressed *TTileL2Cache::getTile(const TTileRef &ref)
{
	TTileCompressed *tile = NULL;
	int i;

	QMutexLocker locker(&_mutex);

	for(i = 0; (i < _tiles.size()) && (!_tiles[i]->compare(ref)); i++);

	if(i != _tiles.size()) {
		tile = _tiles.takeAt(i);
		_tiles.enqueue(tile);
	}

	return tile;
}

