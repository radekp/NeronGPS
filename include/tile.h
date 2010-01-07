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

#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QPixmap>
#include <QList>

#define TILESIZE_P2 8
#define TILESIZE (1 << TILESIZE_P2)

#include "tileref.h"
#include "tilecompressed.h"

class TTileClient;

class TTile : public QPixmap
{
	public:
		TTile();
		~TTile();

		void setRef(const TTileRef &ref);

		void addClient(TTileClient *client);
		void addClients(const QList<TTileClient *> &clients);
		bool removeClient(TTileClient *client);

		const TTileRef &ref() const { return _ref; }

		bool compare(const TTileRef &ref) { return (ref == _ref); }
		TTileCompressed *compress();

	private:
		TTileRef _ref;
		QList<TTileClient *> _clients;
};

#endif

