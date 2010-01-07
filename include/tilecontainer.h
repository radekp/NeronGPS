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

#ifndef TILECONTAINER_H
#define TILECONTAINER_H

#include "include/tilel3cachemux.h"
#include "include/tilel1cache.h"
#include "include/tilel2cache.h"
#include "include/settings.h"

class TTileContainer
{
	public:
		TTileContainer();
		~TTileContainer();

		void configure(TSettings &settings, const QString &cache);

		const QStringList &getCacheList() { return _l3Mux.getCacheList(); }
		const QString &getCacheDir() { return _l3Mux.getCacheDir(); }

		bool addCache() { return _l3Mux.addCache(); }
		bool deleteCache(QString name) { return _l3Mux.deleteCache(name); }

		bool checkL3(const QString &cache, const TTileRef &ref) { return _l3Mux.checkTile(cache, ref); }

		TTile *get(const TTileRef &ref, TTileClient *client, uint flag);
		void release(TTile *tile, TTileClient *client);
		void addToL1(TTile *tile, const QList<TTileClient *> &clients);
		void addToL2(TTileCompressed *compressed);
		void addToL3(const QStringList &caches, TTileCompressed *compressed);

	private:
		TTileL1Cache _l1Cache;
		TTileL2Cache _l2Cache;
		TTileL3CacheMux _l3Mux;
};

#endif

