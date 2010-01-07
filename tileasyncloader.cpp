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

#include "include/tileasyncloader.h"

TTileAsyncLoader::TTileAsyncLoader()
{
	_timer.setSingleShot(true);
	_stopped = 0;

	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotLoadNext()));
}

TTileAsyncLoader::~TTileAsyncLoader()
{
	QMutexLocker locker(&_mutex);

	while(_queue.size() > 0) {
		delete _queue.takeFirst();
	}
}

void TTileAsyncLoader::queue(TTileServer *server, const TTileRef &ref, TTileClient *client, uint priority, void *privateData, uint flag)
{
	QMutexLocker locker(&_mutex);

	TTileAsyncLoaderQueueElement *element = new TTileAsyncLoaderQueueElement;

	element->server = server;
	element->client = client;
	element->ref = ref;
	element->privateData = privateData;
	element->priority = priority;
	element->flag = flag;

	int i;
	for(i = 0; (i < _queue.size()) && (_queue[i]->priority < priority); i++);

	_queue.insert(i, element);

	if(!_stopped) {
		_timer.start(TILEASYNCLOADER_QUEUE_DELAY);
	}
}

void TTileAsyncLoader::slotLoadNext()
{
	QMutexLocker locker(&_mutex);

	if(_queue.size() != 0) {
		TTile *tile;
		TTileAsyncLoaderQueueElement *element = _queue.takeFirst();
		tile = element->server->load(element->ref, element->client, element->privateData, element->flag);
		if(tile != NULL) {
			element->client->triggerTile(tile, 0, element->privateData);
		}
		delete element;

		if((_queue.size() > 0) && !_stopped) {
			_timer.start(TILEASYNCLOADER_QUEUE_DELAY);
		}
	}
}

void TTileAsyncLoader::stop()
{
	QMutexLocker locker(&_mutex);

	_stopped ++;

	_timer.stop();
}

void TTileAsyncLoader::start(int ms)
{
	QMutexLocker locker(&_mutex);

	_stopped --;

	if((!_stopped) && (_queue.size() > 0)) {
		_timer.start(ms);
	}
}

void TTileAsyncLoader::flush(TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _queue.size(); i++) {
		if(_queue[i]->client == client) {
			delete _queue.takeAt(i);
			i--;
		}
	}

	if(_queue.size() == 0) {
		_timer.stop();
	}
}


