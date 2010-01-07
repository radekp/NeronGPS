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

#ifndef TILESERVERDIRECT_H
#define TILESERVERDIRECT_H

#include <QObject>
#include <QSettings>

#include "include/tilel1cache.h"
#include "include/tilel2cache.h"
#include "include/tilel3cachemux.h"
#include "include/tileref.h"
#include "include/tileclient.h"
#include "include/tilehttpmux.h"
#include "include/tilehttptrans.h"
#include "include/tilecontainer.h"

#define TILESERVER_FLAG_DONTGETFROML1	0x0001
#define TILESERVER_FLAG_DONTGETFROML2	0x0002
#define TILESERVER_FLAG_DONTGETFROML3	0x0004
#define TILESERVER_FLAG_DONTLOAD	0x0008

#define TILESERVER_FLAG_DEFAULT		0x0000

#define TILESERVER_FLAG_FORCELOAD	(TILESERVER_FLAG_DONTGETFROML1 | TILESERVER_FLAG_DONTGETFROML2 | TILESERVER_FLAG_DONTGETFROML3)
#define TILESERVER_FLAG_GETFROML1	(TILESERVER_FLAG_DONTGETFROML2 | TILESERVER_FLAG_DONTGETFROML3 | TILESERVER_FLAG_DONTLOAD)

class TTileServerDirect : public QObject
{
	Q_OBJECT
	public:
		TTileServerDirect();
		~TTileServerDirect();

		void configure(TSettings &settings, const QString &server, const QString &cache);

		const QStringList &getServerList() { return _http.serverNames(); }

		bool addCache() { return _container.addCache(); }
		bool deleteCache(QString name) { return _container.deleteCache(name); }
		const QStringList &getCacheList() { return _container.getCacheList(); }
		const QString &getCacheDir() { return _container.getCacheDir(); }
		bool checkL3(const QString &cache, const TTileRef &ref) { return _container.checkL3(cache, ref); }
		TTileContainer &container() { return _container; }

		TTile *load(const TTileRef &ref, TTileClient *client, void *privateData = NULL, uint flag = TILESERVER_FLAG_DEFAULT);

		void releaseTile(TTile *tile, TTileClient *client) { _container.release(tile, client); }
		void discardTransactions(TTileClient *client);

	public slots:
		void slotNewTile(TTileHttpTrans *trans);

	private:
		QMutex _mutex;

		QList<TTileHttpTrans *> _transactions; 

		TTileHttpMux _http;
		TTileContainer _container;

		TTile *uncompressTile(TTileCompressed *compressedTile);
		void newTransaction(const TTileRef &ref, TTileClient *client, void *privateData);
};

#endif

