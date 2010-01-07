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

#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>

#include "include/tilecontainer.h"
#include "include/tilehttpname.h"
#include "include/tileserver.h"

TTileContainer::TTileContainer()
{
}

TTileContainer::~TTileContainer()
{
}

void TTileContainer::configure(TSettings &settings, const QString &cache)
{
	_l3Mux.configure(settings, cache);
}


void TTileContainer::addToL1(TTile *tile, const QList<TTileClient *> &clients)
{
	_l1Cache.addTile(tile, clients);
}

void TTileContainer::addToL2(TTileCompressed *compressed)
{
	_l2Cache.addTile(compressed);
}

void TTileContainer::addToL3(const QStringList &caches, TTileCompressed *compressed)
{
	int i;
	for(i = 0; i < caches.size(); i++) {
		_l3Mux.addTile(compressed, caches[i]);
	}
}

TTile *TTileContainer::get(const TTileRef &ref, TTileClient *client, uint flag)
{
	TTile *tile = NULL;

	if(!(flag & TILESERVER_FLAG_DONTGETFROML1)) {
		tile = _l1Cache.getTile(ref, client);
	}

	if(tile == NULL) {
		TTileCompressed *compressed = NULL;
		bool cached = false;

		if(!(flag & TILESERVER_FLAG_DONTGETFROML2)) {
			compressed = _l2Cache.getTile(ref);
			cached = true;
		}

		if(compressed == NULL) {
			if(!(flag & TILESERVER_FLAG_DONTGETFROML3)) {
				compressed = _l3Mux.getTile(ref);
			}

			if((compressed != NULL) && !(client->flag() & TILETRANSACTION_FLAG_NOL2CACHE)) {
				_l2Cache.addTile(compressed);
				cached = true;
			}
		}

		if((compressed != NULL) && (!(client->flag() & TILETRANSACTION_FLAG_DONTGETTILE) || !(client->flag() & TILETRANSACTION_FLAG_NOL1CACHE))) {
			tile = compressed->uncompress();

			if(tile != NULL) {
				if(!(client->flag() & TILETRANSACTION_FLAG_DONTGETTILE)) {
					_l1Cache.addTile(tile, client);
				} else {
					_l1Cache.addTile(tile);
					tile = NULL;
				}
			}
		}

		if((compressed != NULL) && !cached) {
			delete compressed;
		}
	} else if (!(client->flag() & TILETRANSACTION_FLAG_NOL2CACHE)) {
		_l2Cache.refresh(ref);
	}

	return tile;
}

void TTileContainer::release(TTile *tile, TTileClient *client)
{
	_l1Cache.releaseTile(tile, client);
}


