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

#ifndef TILEBATCHLOADER_H
#define TILEBATCHLOADER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QSemaphore>

#include "include/tileserver.h"

#define LOADERTHREAD_MAXREQUEST		8

class TTileBatchLoader : public QThread
{
	Q_OBJECT
	public:
		TTileBatchLoader();
		~TTileBatchLoader();

		void setServer(TTileServer *server);

		void stop();
		void run();

		int zoomStart() { return _nextZoomStart; }
		int zoomStop() { return _nextZoomStop; }
		QString getBatchLoadingCache() { return _nextCache; }

	public slots:
		void slotTileLoaded(int size, void *privateData);
		void slotNewLoadingCache(QString cache, int beginZoom, int endZoom, bool forceLoad, QStringList servers);
		void slotStartBatchLoading(int x, int y, int w, int h, int zStart);
		void slotStopBatchLoading();
		void slotBatchLoadingFinished();

	signals:
		void signalActionState(const QString &name, bool visible, bool enabled);
	
	private:
		QTime _time;
		QMutex _mutex;
		TTileServer *_tileServer;
		QList<TTileClient *> _clients;
		bool _stop;
		QStringList _nextServers;
		QStringList _servers;
		QString _nextCache;
		QString _cache;
		int _x;
		int _y;
		int _width;
		int _height;
		int _nextZoomStart;
		int _nextZoomStop;
		int _zoomStart;
		int _zoomStop;
		bool _nextForceLoad;
		bool _forceLoad;
		int _currentZoom;
		int _currentEndLevel;
		int _currentLevel;
		uint _loaded;

		void processTile(TTileCompressed *tile);
};

#endif

