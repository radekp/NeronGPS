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

#ifndef TILECOMPOSER_H
#define TILECOMPOSER_H

#include <QObject>
#include <QSettings>

#include "include/tile.h"
#include "include/tileserverdirect.h"
#include "include/tilecompotrans.h"

class TTileComposer : public QObject
{
	Q_OBJECT
	public:
		TTileComposer();
		~TTileComposer();

		void configure(TTileServerDirect *server, TSettings &settings, const QString &composer);

		const QString &name() const { return _name; }

		TTile *load(const TTileRef &ref, TTileClient *client, void *privateData = NULL, uint flag = TILESERVER_FLAG_DEFAULT);

		void discardTransactions(TTileClient *client);

	public slots:
		void slotNewTileClient1(TTile *tile, void *privateData);
		void slotErrorClient1(void *privateData);
		void slotNewTileClient2(TTile *tile, void *privateData);
		void slotErrorClient2(void *privateData);

	private:
		QMutex _mutex;

		QString _name;

		TTileServerDirect *_server;
		TTileClient _client1;
		TTileClient _client2;

		QStringList _layers;
		QList<int> _magnifications;
		QList<bool> _antialiasing;

		QList<TTileCompoTrans *> _transactions;

		void processTile(TTile *tile, void *privateData, TTileClient *client);
		void processTransaction(TTileCompoTrans *trans, TTileClient *client);
};

#endif

