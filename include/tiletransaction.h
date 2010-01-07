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

#ifndef TILETRANSACTION_H
#define TILETRANSACTION_H

#include <QList>
#include <QMutex>
#include <QBuffer>

#include "include/tileclient.h"
#include "include/tileref.h"
#include "include/tile.h"
#include "include/tilecompressed.h"
#include "include/tilecontainer.h"

#define TILETRANSACTION_FLAG_DONTGETTILE	0x0001
#define TILETRANSACTION_FLAG_NOL1CACHE		0x0002
#define TILETRANSACTION_FLAG_NOL2CACHE		0x0004
#define TILETRANSACTION_FLAG_NOL3CACHE		0x0008
#define TILETRANSACTION_FLAG_DONTFORCECOMPRESS	0x0010

#define TILETRANSACTION_FLAG_DEFAULT		TILETRANSACTION_FLAG_DONTFORCECOMPRESS

#define TILETRANSACTION_FLAG_NOACTION		(TILETRANSACTION_FLAG_DONTGETTILE | TILETRANSACTION_FLAG_NOL1CACHE | TILETRANSACTION_FLAG_NOL2CACHE | TILETRANSACTION_FLAG_NOL3CACHE)
#define TILETRANSACTION_FLAG_NOCACHING		(TILETRANSACTION_FLAG_NOL1CACHE | TILETRANSACTION_FLAG_NOL2CACHE | TILETRANSACTION_FLAG_NOL3CACHE)
#define TILETRANSACTION_FLAG_LOADTOL3		(TILETRANSACTION_FLAG_DONTGETTILE | TILETRANSACTION_FLAG_NOL1CACHE | TILETRANSACTION_FLAG_NOL2CACHE)

class TTileTransaction
{
	public:
		TTileTransaction(const TTileRef &ref);
		~TTileTransaction();

		void addClient(TTileClient *client, void *privateData);
		void suppressClient(TTileClient *client);

		void setCompressed(TTileCompressed *compressed) { compressed->setRef(ref()); _compressed = compressed; }
		void setTile(TTile *tile) { tile->setRef(ref()); _tile = tile; }

		const QList<TTileClient *> &getClients() const { return _clients; }

		const TTileRef &ref() const { return _ref; }
		TTileCompressed *compressed() { return _compressed; }
		TTile *tile() { return _tile; }

		void process(TTileContainer &container);

	private:
		QMutex _mutex;

		TTileRef _ref;
		TTileCompressed *_compressed;
		TTile *_tile;

		QList<TTileClient *> _clients;
		QList<void *> _privates;

		QStringList _l3Caches;

		QList<TTileClient *> getTileReceivers();
		const QStringList computeL3Caches();
		uint computeFlag();
		void triggerTile(TTile *tile, int size);
		void triggerError();
};

#endif

