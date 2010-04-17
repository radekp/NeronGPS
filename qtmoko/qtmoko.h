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

#ifndef QTMOKO_H
#define QTMOKO_H

#include <QMenu>
#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>

#include "include/gpssource.h"
#include "include/settings.h"
#include "include/platform.h"
#include "qtmoko/fullscreen.h"

class TQtMoko : public QMainWindow, public TPlatform
{
	Q_OBJECT

	public:
		TQtMoko(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~TQtMoko();

		TGpsSourcePlugin *gpsSource() { return _gpsSource; }
		QMenu *menu() { return _menu; }
		const QString &rootDir() { return _rootDir; }

		void displayAlwaysOn(bool alwaysOn);
		void setMainWidget(QWidget *widget);
		void newForm(QWidget *widget);
		void toggleFullScreen();

	public slots:
		void slotDestroyWidget(QObject *obj);

	protected:
		void closeEvent(QCloseEvent *);

	private:
		QWidget *_mainWidget;
		QStackedWidget _stack;
		QStackedWidget *_fullScreenWidget;
		TFullScreen *_fullScreen;

		TGpsSourcePlugin *_gpsSource;
		QMenu *_menu;
		QString _rootDir;
};

#endif

