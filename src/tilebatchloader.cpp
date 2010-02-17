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

#include "include/tilebatchloader.h"
#include "include/tile.h"
#include "include/global.h"

TTileBatchLoader::TTileBatchLoader()
{
	_cache = "";
	_tileServer = NULL;
	_nextZoomStart = 0;
	_nextZoomStop = 20;

	connect(this, SIGNAL(finished()), this, SLOT(slotBatchLoadingFinished()));
}

TTileBatchLoader::~TTileBatchLoader()
{
	_stop = true;
	wait();
}

void TTileBatchLoader::setServer(TTileServer *server)
{
	_tileServer = server;
}

void TTileBatchLoader::stop()
{
	_stop = true;
}

void TTileBatchLoader::run()
{
	int i, j, k;

	_loaded = 0;
	_stop = false;
	_time.start();

	for(i = 0; i < _servers.size(); i++) {
		TTileClient *client = new TTileClient(LOADERTHREAD_MAXREQUEST);
		client->setFlag(TILETRANSACTION_FLAG_LOADTOL3);
		client->setCache(_cache);
		connect(client, SIGNAL(signalLoad(int, void *)), this, SLOT(slotTileLoaded(int, void *)));
		_clients.append(client);
	}

	for(k = _zoomStart; (k <= _zoomStop) && (!_stop); k++) {
		int left = _x >> TILESIZE_P2;
		int top = _y >> TILESIZE_P2;
		int restLeft = _x & (TILESIZE - 1);
		int restTop = _y & (TILESIZE - 1);

		_currentZoom = k;
		_currentEndLevel = ((_height + restTop) / TILESIZE + 1) * ((_width + restLeft) / TILESIZE + 1);
		_currentLevel = 0;

		for(j = 0; (j * TILESIZE < _height + restTop) && (!_stop); j++) {
			for(i = 0; (i * TILESIZE < _width + restLeft) && (!_stop); i++) {
				int l;
				for(l = 0; l < _clients.size(); l++) {
					_clients[l]->acquireSemaphore();
				}
				for(l = 0; l < _servers.size(); l++) {
					TTileRef ref(_servers[l], left + i, top + j, k);

					if(!_forceLoad && _tileServer->checkL3(_cache, ref)) {
						_clients[l]->releaseSemaphore();
					} else {
						_tileServer->load(ref, _clients[l], NULL, TILESERVER_FLAG_FORCELOAD);
					}
				}

				_currentLevel ++;
			}
		}

		_x = _x << 1;
		_y = _y << 1;
		_width = _width << 1;
		_height = _height << 1;
	}

	if(!_stop) {
		for(i = 0; i < _clients.size(); i++) {
			for(j = 0; j < LOADERTHREAD_MAXREQUEST; j++) {
				_clients[i]->acquireSemaphore();
			}
		}
	}

	_stop = true;

	while(_clients.size() > 0) {
		TTileClient *client = _clients.takeFirst();
		_tileServer->discardTransactions(client);
		delete client;
	}
}

void TTileBatchLoader::slotTileLoaded(int size, void */*privateData*/)
{
	_loaded += size;

	QMutexLocker locker(&_mutex);

	if(_time.elapsed() > 1000) {
		_time.restart();

		QString purcent = "";
		if((_currentLevel * 100) / _currentEndLevel < 10) {
			purcent += "0";
		}
		purcent += QString::number((_currentLevel * 100) / _currentEndLevel);

		QString size = QString("%1").arg((float)_loaded / (float)(1024 * 1024), 0, 'f', 3);

		tMessage((void *)this) << "L" << _currentZoom << " (" << purcent << "%) - " << size << "MB";
	}
}

void TTileBatchLoader::slotNewLoadingCache(QString cache, int beginZoom, int endZoom, bool forceLoad, QStringList servers)
{
	if(cache != "") {
		if(!isRunning()) {
			emit signalActionState("Start batch", true, true);
		}
	} else {
		emit signalActionState("Start batch", false, false);
	}

	_nextCache = cache;
	_nextZoomStart = beginZoom;
	_nextZoomStop = endZoom;
	_nextServers = servers;
	_nextForceLoad = forceLoad;
}

void TTileBatchLoader::slotStartBatchLoading(int x, int y, int w, int h, int zStart)
{
	if((_nextCache != "") && (_tileServer != NULL) && (!isRunning())) {
		emit signalActionState("Start batch", false, false);
		emit signalActionState("Stop batch", true, true);

		_x = x;
		_y = y;
		_width = w;
		_height = h;
		_zoomStart = _nextZoomStart;
		_zoomStop = _nextZoomStop;
		_servers = _nextServers;
		_forceLoad = _nextForceLoad;
		_cache = _nextCache;
		if(zStart > _zoomStart) {
			_zoomStart = zStart;
		} else if(zStart < _zoomStart) {
			int shift = _zoomStart - zStart;
			_x = _x << shift;
			_y = _y << shift;
			_width = _width << shift;
			_height = _height << shift;
		}

		start();
	}
}

void TTileBatchLoader::slotStopBatchLoading()
{
	if(isRunning()) {
		emit signalActionState("Stop batch", true, false);
	}

	stop();
}

void TTileBatchLoader::slotBatchLoadingFinished()
{
	emit signalActionState("Stop batch", false, false);

	if(_cache != "") {
		emit signalActionState("Start batch", true, true);
	}

	tMessage((void *)this) << "";
}


