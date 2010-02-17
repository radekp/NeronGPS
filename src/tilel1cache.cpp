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

#include "include/tilel1cache.h"
#include "include/tile.h"
#include "include/tileref.h"

TTileL1Cache::TTileL1Cache()
{
}

TTileL1Cache::~TTileL1Cache()
{
}

void TTileL1Cache::releaseTile(TTile *tile, TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	if(tile->removeClient(client)) {
		for(i = 0; (i < _locked.size()) && (_locked[i] != tile); i++);
		if(i != _locked.size()){
			_locked.takeAt(i);
			_unlocked.enqueue(tile);
			if(_locked.size() + _unlocked.size() > CACHESIZE) {
				delete _unlocked.dequeue();
			}
		}
	}
}

TTile *TTileL1Cache::getTile(const TTileRef &ref, TTileClient *client)
{
	TTile *tile = NULL;
	int i;

	QMutexLocker locker(&_mutex);

	for(i = 0; (i < _locked.size()) && !_locked[i]->compare(ref); i++);
	if(i != _locked.size()) {
		tile = _locked[i];
		tile->addClient(client);
	} else {
		for(i = 0; (i < _unlocked.size()) && !_unlocked[i]->compare(ref); i++);
		if(i != _unlocked.size()) {
			tile = _unlocked.takeAt(i);
			tile->addClient(client);
			_locked.append(tile);
		}
	}

	return tile;
}

void TTileL1Cache::addTile(TTile *tile, TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	if(client != NULL) {
		tile->addClient(client);
		_locked.append(tile);
	} else {
		_unlocked.append(tile);
	}
	
	if((_unlocked.size() != 0) && (_locked.size() + _unlocked.size() > CACHESIZE)) {
		delete _unlocked.dequeue();
	}
}

void TTileL1Cache::addTile(TTile *tile, const QList<TTileClient *> &clients)
{
	QMutexLocker locker(&_mutex);

	if(clients.size() == 0) {
		_unlocked.enqueue(tile);
	} else {
		tile->addClients(clients);
		_locked.append(tile);
	}
	
	if((_unlocked.size() != 0) && (_locked.size() + _unlocked.size() > CACHESIZE)) {
		delete _unlocked.dequeue();
	}
}

