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

#include <QBuffer>

#include "include/tile.h"
#include "include/tileref.h"

TTile::TTile() : QPixmap(TILESIZE, TILESIZE)
{
}

TTile::~TTile()
{
}

void TTile::setRef(const TTileRef &ref)
{
	_ref = ref;
}

void TTile::addClient(TTileClient *client)
{
	_clients += client;
} 

void TTile::addClients(const QList<TTileClient *> &clients)
{
	_clients += clients;
} 

bool TTile::removeClient(TTileClient *client)
{
	if(!_clients.removeOne(client)) {
		qDebug() << "Error, remove an unknown client from Tile";
	}

	return _clients.size() == 0;
}

TTileCompressed *TTile::compress()
{
	TTileCompressed *compressed = new TTileCompressed();
	compressed->setRef(_ref);

	QBuffer buffer((QByteArray *)compressed);
	buffer.open(QIODevice::WriteOnly);

	if(!save(&buffer, "PNG", 25)) {
		delete compressed;
		compressed = NULL;
	}

	return compressed;	
}

