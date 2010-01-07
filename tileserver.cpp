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

#include "include/tileserver.h"
#include "include/tilecomposer.h"
#include "include/tilehttpname.h"

TTileServer::TTileServer()
{
}

TTileServer::~TTileServer()
{
	while(_composers.size() > 0) {
		delete _composers.takeFirst();
	}
}

void TTileServer::configure(TSettings &settings, const QString &server, const QString &cache)
{
	QStringList composers;

	_server.configure(settings, server, cache);
	_serverNames = _server.getServerList();

	settings.beginGroup(server);
	composers = settings.getValue("composers", "").toStringList();
	settings.endGroup();

	int i;
	for(i = 0; i < composers.size(); i++) {
		if(composers[i] != "") {
			TTileComposer *composer = new TTileComposer;
			composer->configure(&_server, settings, composers[i]);
			_composers.append(composer);
			_serverNames << QString(composer->name());
		}
	}
}

void TTileServer::slotAddCache()
{
	if(_server.addCache()) {
		emit signalNewCacheList(_server.getCacheList());
	}
}

void TTileServer::slotDeleteCache(QString name)
{
	if(_server.deleteCache(name)) {
		emit signalNewCacheList(_server.getCacheList());
	}
}

TTile *TTileServer::load(const TTileRef &ref, TTileClient *client, void *privateData, uint flag)
{
	TTile *tile = NULL;

	if(TTileHttpName::isComposer(ref.server())) {
		int i;
		for(i = 0; (i < _composers.size()) && (_composers[i]->name() != ref.server()); i++);

		if(i != _composers.size()) {
			tile = _composers[i]->load(ref, client, privateData, flag);
		} else {
			qDebug() << "Error, unknown composer : " << ref.server();
		}
	} else {
		tile = _server.load(ref, client, privateData, flag);
	}

	return tile;
}

void TTileServer::discardTransactions(TTileClient *client)
{
	int i;
	for(i = 0; i < _composers.size(); i++) {
		_composers[i]->discardTransactions(client);
	}

	_server.discardTransactions(client);
}


