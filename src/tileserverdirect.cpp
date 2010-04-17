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

#include "include/tileserverdirect.h"
#include "include/tilehttpname.h"

TTileServerDirect::TTileServerDirect()
{
	_online = false;

	connect(&_http, SIGNAL(signalNewTile(TTileTransaction *)), this, SLOT(slotNewTile(TTileTransaction *)));
}

TTileServerDirect::~TTileServerDirect()
{
	QMutexLocker locker(&_mutex);

	while(_transactions.size() > 0) {
		delete _transactions.takeFirst();
	}
}

void TTileServerDirect::configure(TSettings &settings, const QString &server, const QString &cache)
{
	QStringList serverNames;
	QString defaultServer;
	bool empty;

	settings.beginGroup(server);
	empty = !settings.contains("httpservers");
	settings.endGroup();

	if(empty) {
		defaultServer = TTileHttpName::initDefault(settings);
	}

	settings.beginGroup(server);
	serverNames = settings.getValue("httpservers", defaultServer).toStringList();
	_online = settings.getValue("startonline", true).toBool();
	settings.endGroup();

	_http.configure(settings, serverNames);
	_container.configure(settings, cache);
}

TTile *TTileServerDirect::load(const TTileRef &ref, TTileClient *client, void *privateData, uint flag)
{
	TTile *tile = _container.get(ref, client, flag);

	if((tile == NULL) && _online && !(flag & TILESERVER_FLAG_DONTLOAD)) {
		newTransaction(ref, client, privateData);
	}

	return tile;
}

void TTileServerDirect::newTransaction(const TTileRef &ref, TTileClient *client, void *privateData)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; (i < _transactions.size()) && (_transactions[i]->ref() != ref); i++);

	if(i == _transactions.size()) {
		TTileTransaction *trans = new TTileTransaction(ref);
		trans->addClient(client, privateData);
		_transactions.append(trans);
		_http.load(trans);
	} else {
		_transactions[i]->addClient(client, privateData);
	}
}

void TTileServerDirect::discardTransactions(TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _transactions.size(); i++) {
		_transactions[i]->suppressClient(client);
	}
}

void TTileServerDirect::slotNewTile(TTileTransaction *trans)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; (i < _transactions.size()) && (_transactions[i] != trans); i++);

	if(i != _transactions.size()) {
		trans->process(_container);
		delete _transactions.takeAt(i);
	} else {
		qDebug() << "Error: unknown transaction";
		if(trans->compressed() != NULL) {
			delete trans->compressed();
		}
	}
}


