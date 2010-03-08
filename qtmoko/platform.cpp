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

#include <QAction>
#include <QMenu>
#include <QtopiaApplication>
#include <QMainWindow>
#include <QSoftMenuBar>

#include <QMessageBox>
#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <QDesktopWidget>
#include <QtopiaServiceRequest>

#include "qtmoko/gpssourceabout.h"
#include "qtmoko/power.h"
#include "include/platform.h"
#include "include/gpssourcenmea.h"
#include "include/gpssourcegpsd.h"

TPlatform::TPlatform()
{
	_rootDir = QString("/media/card/NeronGPS");

	if(!TPower::gpsGetPower()) {
		qDebug() << "Power-up GPS";
		TPower::gpsSetPower(true);
	} else {
		qDebug() << "GPS already on";
	}

	_gpsSource = new TGpsSourceAbout();
}

TPlatform::~TPlatform()
{
	delete _gpsSource;
}

void TPlatform::configure(QMainWindow *mainWindow)
{
	QtopiaApplication::setInputMethodHint(mainWindow, QtopiaApplication::AlwaysOff);
	_menu = QSoftMenuBar::menuFor(mainWindow);
}

void TPlatform::displayAlwaysOn(bool alwaysOn)
{
	if(alwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}
}


