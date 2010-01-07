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

#ifndef TILECLIENT_H
#define TILECLIENT_H

#include <QObject>
#include <QSemaphore>

class TTile;

class TTileClient : public QObject
{
	Q_OBJECT
	public:
		TTileClient(int semaphore = 0);
		~TTileClient();

		void acquireSemaphore(int num = 1) { _semaphore.acquire(num); }
		void releaseSemaphore(int num = 1) { _semaphore.release(num); }

		void triggerTile(TTile *tile, int size, void *privateData);
		void triggerError(void *privateData);

		void setFlag(uint flag) { _flag = flag; }
		void setCache(const QString &name) { _cacheName = name; }

		uint flag() { return _flag; }
		const QString &cache() const { return _cacheName; }

	signals:
		void signalTile(TTile *tile, void *privateData);
		void signalLoad(int size, void *privateData);
		void signalError(void *privateData);

	private:
		QSemaphore _semaphore;
		bool _useSemaphore;
		QString _cacheName;
		uint _flag;
};

#endif

