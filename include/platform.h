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

#include <QObject>
#include <QWidget>
#include <QMenu>

#include "include/gpssource.h"
#include "include/settings.h"

class TPlatform : public QObject
{
	public:
		virtual void configure(TSettings &settings, const QString &section) = 0;

		virtual QWidget *mainWindow() = 0;
		virtual TGpsSource *gpsSource() = 0;
		virtual QMenu *menu() = 0;
		virtual bool displayAlwaysOn() = 0;

	public slots:
		virtual void slotDisplayAlwaysOn(bool alwaysOn) = 0;
};

#endif

