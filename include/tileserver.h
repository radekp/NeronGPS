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

#ifndef TILESERVER_H
#define TILESERVER_H

#include <QObject>
#include <QSettings>

#include "include/tileserverdirect.h"

class TTileComposer;

class TTileServer : public QObject
{
	Q_OBJECT
	public:
		TTileServer();
		~TTileServer();

		void configure(TSettings &settings, const QString &server, const QString &cache);

		const QStringList &getCacheList() { return _server.getCacheList(); }
		const QString &getCacheDir() { return _server.getCacheDir(); }
		const QStringList &getServerList() { return _serverNames; }

		bool checkL3(const QString &cache, const TTileRef &ref) { return _server.checkL3(cache, ref); }

		void releaseTile(TTile *tile, TTileClient *client) { _server.releaseTile(tile, client); }

		TTile *load(const TTileRef &ref, TTileClient *client, void *privateData = NULL, uint flag = TILESERVER_FLAG_DEFAULT);
		void discardTransactions(TTileClient *client);

	public slots:
		void slotAddCache();
		void slotDeleteCache(QString name);

	signals:
		void signalNewCacheList(QStringList cacheList);

	private:
		QList<TTileComposer *> _composers;
		TTileServerDirect _server;

		QStringList _serverNames;
};

#endif

