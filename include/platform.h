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

#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMenu>
#include <QMainWindow>

#include "include/gpssource.h"
#include "include/settings.h"

class TPlatform
{
	public:
		TPlatform();
		~TPlatform();

		void configure(QMainWindow *mainWindow);

		TGpsSource *gpsSource() { return _gpsSource; }
		QMenu *menu() { return _menu; }
		const QString &rootDir() { return _rootDir; }
		void displayAlwaysOn(bool alwaysOn);

	private:
		TGpsSource *_gpsSource;
		QMenu *_menu;
		QString _rootDir;
};

#endif

