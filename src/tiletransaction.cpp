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

#include "include/tiletransaction.h"

TTileTransaction::TTileTransaction(const TTileRef &ref)
{
	_ref = ref;
	_compressed = NULL;
	_tile = NULL;
}

TTileTransaction::~TTileTransaction()
{
}

void TTileTransaction::addClient(TTileClient *client, void *privateData)
{
	QMutexLocker locker(&_mutex);

	_clients.append(client);
	_privates.append(privateData);
}

void TTileTransaction::suppressClient(TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _clients.size(); i++) {
		if(_clients[i] == client) {
			_clients.takeAt(i);
			_privates.takeAt(i);
			i--;
		}
	}
}

QList<TTileClient *> TTileTransaction::getTileReceivers()
{
	QList<TTileClient *> tmp;

	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _clients.size(); i++) {
		if(!(_clients[i]->flag() & TILETRANSACTION_FLAG_DONTGETTILE)) {
			tmp.append(_clients[i]);
		}
	}

	return tmp;
}

void TTileTransaction::process(TTileContainer &container)
{
	if((_compressed == NULL) && (_tile == NULL)) {
		triggerError();
		return;
	}

	QStringList caches = computeL3Caches();
	uint flag = computeFlag();
	int size = 0;

	if((_compressed == NULL) && !(flag & TILETRANSACTION_FLAG_DONTFORCECOMPRESS) && ( (caches.size() > 0) || !(flag & TILETRANSACTION_FLAG_NOL2CACHE) )) {
		_compressed = _tile->compress();
	}

	if((caches.size() > 0) && (_compressed != NULL)) {
		container.addToL3(caches, _compressed);
	}

	if(!(flag & TILETRANSACTION_FLAG_NOL2CACHE) && (_compressed != NULL)) {
		container.addToL2(_compressed);
	}

	if(_compressed != NULL) {
		size = _compressed->size();
	}

	if(!(flag & TILETRANSACTION_FLAG_DONTGETTILE) || !(flag & TILETRANSACTION_FLAG_NOL1CACHE)) {
		if(_tile == NULL) {
			_tile = _compressed->uncompress();
		}
		if(_tile != NULL) {
			container.addToL1(_tile, getTileReceivers());
		}
	} else if(_tile != NULL) {
		delete _tile;
	}

	if(!(flag & TILETRANSACTION_FLAG_DONTGETTILE)) {
		if(_tile != NULL) {
			triggerTile(_tile, size);
		} else {
			triggerError();
		}
	} else {
		triggerTile(NULL, size);
	}

	if((flag & TILETRANSACTION_FLAG_NOL2CACHE) && (_compressed != NULL)){
		delete _compressed;
		_compressed = NULL;
	} 
}

uint TTileTransaction::computeFlag()
{
	QMutexLocker locker(&_mutex);

	uint flag = (uint)-1;
	int i;
	for(i = 0; i < _clients.size(); i++) {
		flag &= _clients[i]->flag();
	}

	return flag;
}

const QStringList TTileTransaction::computeL3Caches()
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _clients.size(); i++) {
		if(!(_clients[i]->flag() & TILETRANSACTION_FLAG_NOL3CACHE)) {
			if(_l3Caches.indexOf(_clients[i]->cache()) == -1) {
				_l3Caches.append(_clients[i]->cache());
			}
		}
	}

	return _l3Caches;
}

void TTileTransaction::triggerTile(TTile *tile, int size)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _clients.size(); i++) {
		_clients[i]->triggerTile(tile, size, _privates[i]);
	}
}

void TTileTransaction::triggerError()
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _clients.size(); i++) {
		_clients[i]->triggerError(_privates[i]);
	}
}

