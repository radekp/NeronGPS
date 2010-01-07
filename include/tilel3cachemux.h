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

#ifndef TILEL3CACHEMUX_H
#define TILEL3CACHEMUX_H

#include <QObject>
#include <QMutex>
#include <QProcess>

#include "include/tilel3cache.h"

class TTileRef;
class TTileCompressed;
class TSettings;

class TTileL3CacheMux : public QObject
{
	Q_OBJECT
	public:
		TTileL3CacheMux();
		~TTileL3CacheMux();

		void configure(TSettings &settings, const QString &section);

		bool addCache(const QString &name);
		bool addCache();
		bool deleteCache(const QString &name);

		const QStringList &getCacheList() { return _cacheNames; };
		const QString &getCacheDir() { return _dir; };

		bool checkTile(const QString &cacheName, const TTileRef &ref);
		void addTile(TTileCompressed *tile, QString cacheName);
		TTileCompressed *getTile(const TTileRef &ref);

	public slots:
		void slotFinished(int exitCode, QProcess::ExitStatus exitStatus); 

	private:
		QMutex _mutex;
		QList<QProcess *> _processes;

		QString _dir;
		QString _tempName;

		QList<TTileL3Cache *> _caches;
		TTileL3Cache _loader;
		QStringList _cacheNames;
};

#endif

