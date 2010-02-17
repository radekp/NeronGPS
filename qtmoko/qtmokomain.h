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

#ifndef QTMOKOMAIN_H
#define QTMOKOMAIN_H

#include <QMainWindow>
#include <QMenu>
#include <QSoftMenuBar>

#include "include/nerongps.h"
#include "include/gpssource.h"
#include "include/platform.h"

class TQtMokoMain : public QWidget, public TPlatform
{
	Q_OBJECT
	public:
		TQtMokoMain(QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TQtMokoMain();

		void configure(TSettings &settings, const QString &section);

		QWidget *mainWindow() { return this; }
		TGpsSource *gpsSource() { return _gpsSource; }
		QMenu *menu() { return QSoftMenuBar::menuFor(this); }
		bool displayAlwaysOn() { return _displayAlwaysOn; }

	public slots:
		void slotDisplayAlwaysOn(bool alwaysOn);

	private:
		TGpsSource *_gpsSource;
		TNeronGPS *_neronGPS;
		bool _displayAlwaysOn;
};

#endif

