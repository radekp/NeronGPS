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

#include <QPainter>

#include "include/tilecomposer.h"
#include "include/tilehttpname.h"

TTileComposer::TTileComposer()
{
	_client1.setFlag(TILETRANSACTION_FLAG_NOCACHING);
	_client2.setFlag(TILETRANSACTION_FLAG_NOL1CACHE);

	connect(&_client1, SIGNAL(signalTile(TTile *, void *)), this, SLOT(slotNewTileClient1(TTile *, void *)));
	connect(&_client1, SIGNAL(signalError(void *)), this, SLOT(slotErrorClient1(void *)));
	connect(&_client2, SIGNAL(signalTile(TTile *, void *)), this, SLOT(slotNewTileClient2(TTile *, void *)));
	connect(&_client2, SIGNAL(signalError(void *)), this, SLOT(slotErrorClient2(void *)));
}

TTileComposer::~TTileComposer()
{
	QMutexLocker locker(&_mutex);

	while(_transactions.size() > 0) {
		delete _transactions.takeFirst();
	}
}

void TTileComposer::configure(TTileServerDirect *server, TSettings &settings, const QString &composer)
{
	settings.beginGroup(composer);
	char i = '1';
	while(settings.contains(QString("layer") + i)) {
		_layers << settings.getValue(QString("layer") + i, "").toString();
		_magnifications << settings.getValue(QString("magnification") + i, 0).toInt();
		_antialiasing << settings.getValue(QString("antialiasing") + i, false).toBool();
		i++;
	}
	settings.endGroup();

	_server = server;
	_name = TTileHttpName::createComposerName(composer);

	qDebug() << "Add composer" << _name << "with" << _layers.size() << "layers:" << _layers; 
}

TTile *TTileComposer::load(const TTileRef &ref, TTileClient *client, void *privateData, uint flag)
{
	int i;

	_mutex.lock();
	for(i = 0; (i < _transactions.size()) && (_transactions[i]->ref() != ref); i++);
	if(i < _transactions.size()) {
		_transactions[i]->addClient(client, privateData);
		_mutex.unlock();
		return NULL;
	}
	_mutex.unlock();

	TTile *tile = _server->load(ref, client, privateData, flag | TILESERVER_FLAG_DONTLOAD);

	if((tile == NULL) && !(flag & TILESERVER_FLAG_DONTLOAD)) {
		TTileCompoTrans *trans = new TTileCompoTrans(ref);
		trans->addClient(client, privateData);

		_mutex.lock();
		_transactions.append(trans);
		_mutex.unlock();

		TTileClient *tmpClient;
		if(client->flag() & TILETRANSACTION_FLAG_DONTFORCECOMPRESS) {
			tmpClient = &_client2;
		} else {
			tmpClient = &_client1;
		}

		for(i = 0; i < _layers.size(); i++) {
			int magnification = _magnifications[i];

			TTileRef tmpRef(_layers[i], ref.x() >> magnification, ref.y() >> magnification, ref.zoom() - magnification);
			TTile *tile = _server->load(tmpRef, tmpClient, (void *)trans);
			if(tile != NULL) {
				trans->addTile(tile);
			}
		}

		if(trans->tiles().size() == _layers.size()) {
			processTransaction(trans, tmpClient);
		}
	}

	return tile;
}

void TTileComposer::discardTransactions(TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _transactions.size(); i++) {
		_transactions[i]->suppressClient(client);
	}
}

void TTileComposer::slotNewTileClient1(TTile *tile, void *privateData)
{
	processTile(tile, privateData, &_client1);
}

void TTileComposer::slotErrorClient1(void *privateData)
{
	processTile(NULL, privateData, &_client1);
}

void TTileComposer::slotNewTileClient2(TTile *tile, void *privateData)
{
	processTile(tile, privateData, &_client2);
}

void TTileComposer::slotErrorClient2(void *privateData)
{
	processTile(NULL, privateData, &_client2);
}

void TTileComposer::processTile(TTile *tile, void *privateData, TTileClient *client)
{
	TTileCompoTrans *trans = (TTileCompoTrans *)privateData;

	trans->addTile(tile);

	if(trans->tiles().size() == _layers.size()) {
		processTransaction(trans, client);
	}
}

void TTileComposer::processTransaction(TTileCompoTrans *trans, TTileClient *client)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; (i < _transactions.size()) && (_transactions[i] != trans); i++);

	if(i < _transactions.size()) {
		_transactions.takeAt(i);

		for(i = 0; (i < trans->tiles().size()) && (trans->tiles()[i] != NULL); i++);

		if((i == trans->tiles().size()) && (trans->getClients().size() != 0)) {
			QList<TTile *> tmpList;

			for(i = 0; i < _layers.size(); i++) {
				int j;
				for(j = 0; (j < trans->tiles().size()) && (trans->tiles()[j]->ref().server() != _layers[i]); j++);

				if(j < trans->tiles().size()) {
					tmpList.append(trans->tiles()[j]);
				} else {
					qDebug() << "Error, missing layer";
				}
			}

			TTile *tile = new TTile;

			QPainter *painter = new QPainter(tile);
			for(i = 0; i < tmpList.size(); i++) {
				int magnification = _magnifications[i];

				int x = ((tmpList[i]->ref().x() << magnification) - trans->ref().x()) * TILESIZE;
				int y = ((tmpList[i]->ref().y() << magnification) - trans->ref().y()) * TILESIZE;

				painter->setRenderHints(QPainter::SmoothPixmapTransform, _antialiasing[i]);
				painter->drawPixmap(x, y, TILESIZE << magnification, TILESIZE << magnification, *tmpList[i]);
			}
			delete painter;

			trans->setTile(tile);
		}

		trans->process(_server->container());
	} else {
		qDebug() << "Error, unknown transaction";
	}

	for(i = 0; i < trans->tiles().size(); i++) {
		if(trans->tiles()[i] != NULL) {
			_server->releaseTile(trans->tiles()[i], client);
		}
	}

	delete trans;
}

