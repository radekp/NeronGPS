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

#ifndef TILEASYNCLOADER_H
#define TILEASYNCLOADER_H

#include <QObject>
#include <QSettings>
#include <QTimer>

#include "include/tileserver.h"
#include "include/tileclient.h"
#include "include/tileref.h"

#define TILEASYNCLOADER_QUEUE_DELAY	10

struct TTileAsyncLoaderQueueElement {
	TTileServer *server;
	TTileClient *client;
	TTileRef ref;
	void *privateData;
	uint priority;
	uint flag;
};

class TTileAsyncLoader : public QObject
{
	Q_OBJECT
	public:
		TTileAsyncLoader();
		~TTileAsyncLoader();

		void queue(TTileServer *server, const TTileRef &ref, TTileClient *client, uint priority = 0, void *privateData = 0, uint flag = TILESERVER_FLAG_DEFAULT);
		void stop();
		void start(int ms);
		void flush(TTileClient *client);

	public slots:
		void slotLoadNext();

	private:
		QMutex _mutex;
		QTimer _timer;

		QList<TTileAsyncLoaderQueueElement *> _queue;
		int _stopped;
};

#endif

