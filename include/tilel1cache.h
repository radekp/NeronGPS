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

#ifndef TILEL1CACHE_H
#define TILEL1CACHE_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "tilel2cache.h"
#include "tile.h"
#include "tileref.h"

#define CACHESIZE 32

class TtileClient;

class TTileL1Cache : public QObject
{
	Q_OBJECT
	public:
		TTileL1Cache();
		~TTileL1Cache();

		TTile *getTile(const TTileRef &ref, TTileClient *client);
		void addTile(TTile *tile, TTileClient *client = NULL);
		void addTile(TTile *tile, const QList<TTileClient *> &clients);

		void releaseTile(TTile *tile, TTileClient *client);

	private:
		QMutex _mutex;
		QList<TTile *> _locked;
		QQueue<TTile *> _unlocked;
};

#endif

