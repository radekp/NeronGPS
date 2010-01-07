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

#ifndef TILEL2CACHE_H
#define TILEL2CACHE_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "include/tilecompressed.h"
#include "include/tileref.h"

#define MEMORYCACHESIZE 64

class TTileL2Cache : public QObject
{
	Q_OBJECT
	public:
		TTileL2Cache();
		~TTileL2Cache();

		void refresh(const TTileRef &ref);
		void addTile(TTileCompressed *tile);
		TTileCompressed *getTile(const TTileRef &ref);

	private:
		QMutex _mutex;
		QQueue<TTileCompressed *> _tiles;
};

#endif

