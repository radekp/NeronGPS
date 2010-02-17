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

#include <QMessageBox>
#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <QDesktopWidget>
#include <QtopiaServiceRequest>

#include "include/qtmokomain.h"
#include "include/gpssourceabout.h"
#include "include/gpssourcenmea.h"

TQtMokoMain::TQtMokoMain(QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	QWidget::setObjectName("NeronGPS");
	QtopiaApplication::setInputMethodHint(this, QtopiaApplication::AlwaysOff);
	setWindowTitle(tr("NeronGPS", "application header"));

//	_gpsSource = new TGpsSourceNmea(QString("/home/root/nmea_sample.txt"));
//	_gpsSource = new TGpsSourceAbout(QString("/home/root/nmea_sample.txt"));
	_gpsSource = new TGpsSourceAbout();

	_displayAlwaysOn = false;

	showMaximized();

	_neronGPS = new TNeronGPS(this);
}

TQtMokoMain::~TQtMokoMain()
{
	delete _neronGPS;
	delete _gpsSource;
}

void TQtMokoMain::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);
	_displayAlwaysOn = settings.getValue("displayalwayson", false).toBool();
	settings.endGroup();

	if(_displayAlwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}
}

void TQtMokoMain::slotDisplayAlwaysOn(bool alwaysOn)
{
	_displayAlwaysOn = alwaysOn;

	if(_displayAlwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}
}




