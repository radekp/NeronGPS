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

#ifndef TILEHTTPMUX_H
#define TILEHTTPMUX_H

#include <QObject>
#include <QHttp>
#include <QMutex>
#include <QFile>

#include "include/tilehttpwheel.h"
#include "include/settings.h"
#include "include/tileref.h"
#include "include/tilehttptrans.h"

class TTileHttpMux : public QObject
{
	Q_OBJECT
	public:
		TTileHttpMux();
		~TTileHttpMux();

		void configure(TSettings &settings, const QStringList &sections);

		void load(TTileHttpTrans *trans);

		const QStringList &serverNames() { return _serverNames; }

	signals:
		void signalNewTile(TTileHttpTrans *trans);

	public slots:
		void slotNewTile(TTileHttpTrans *trans);

	private:
		QStringList _serverNames;
		QList<TTileHttpWheel *> _wheels;
};

#endif

