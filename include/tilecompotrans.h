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

#ifndef TILECOMPOTRANS_H
#define TILECOMPOTRANS_H

#include <QList>
#include <QMutex>
#include <QBuffer>

#include "include/tiletransaction.h"
#include "include/tile.h"

class TTileCompoTrans : public TTileTransaction
{
	public:
		TTileCompoTrans(const TTileRef &ref);
		~TTileCompoTrans();

		void addTile(TTile *tile) { _tiles.append(tile); }
		QList<TTile *> &tiles() { return _tiles; }

	private:
		QList<TTile *> _tiles;
};

#endif

