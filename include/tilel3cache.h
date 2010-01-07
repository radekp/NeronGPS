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

#ifndef TILEL3CACHE_H
#define TILEL3CACHE_H

#include <QObject>
#include <QMutex>

class TTileRef;
class TTileCompressed;

class TTileL3Cache : public QObject
{
	Q_OBJECT
	public:
		TTileL3Cache();
		~TTileL3Cache();

		void configure(const QString &name, const QString &dir);
		const QString &name() { return _name; }

		bool checkTile(const TTileRef &ref);
		void addTile(TTileCompressed *tile);
		TTileCompressed *getTile(const TTileRef &ref);

	private:
		QMutex _mutex;

		QString _name;
		QString _dir;

		void createDir(const TTileRef &ref);
		QString buildName(const TTileRef &ref);
};

#endif

