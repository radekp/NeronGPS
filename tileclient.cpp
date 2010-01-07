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

#include "include/tileclient.h"
#include "include/tiletransaction.h"

TTileClient::TTileClient(int semaphore)
{
	_flag = TILETRANSACTION_FLAG_DEFAULT;
	_cacheName = "";

	if(semaphore == 0) {
		_useSemaphore = false;
	} else {
		_useSemaphore = true;
		_semaphore.release(semaphore);
	}
}

TTileClient::~TTileClient()
{
}

void TTileClient::triggerTile(TTile *tile, int size, void *privateData)
{
	if(_flag & TILETRANSACTION_FLAG_DONTGETTILE) {
		emit signalLoad(size, privateData);
	} else {
		emit signalTile(tile, privateData);
	}

	if(_useSemaphore) {
		_semaphore.release();	
	}
}

void TTileClient::triggerError(void *privateData)
{
	emit signalError(privateData);

	if(_useSemaphore) {
		_semaphore.release();	
	}
}

