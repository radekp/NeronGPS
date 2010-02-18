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

#include <QMenuBar>
#include <QDir>

#include "include/platform.h"
#include "include/gpssourcenmea.h"

TPlatform::TPlatform()
{
        _rootDir = QDir::homePath() + QString("/NeronGPS");

//	_gpsSource = new TGpsSourceNmea(QString("/home/root/nmea_sample.txt"));
        _gpsSource = new TGpsSourceNmea();
}

TPlatform::~TPlatform()
{
    delete _gpsSource;
}

void TPlatform::configure(QMainWindow *mainWindow)
{
        _menu = mainWindow->menuBar()->addMenu(QString("&Menu"));
}

void TPlatform::displayAlwaysOn(bool /*alwaysOn*/)
{
}

