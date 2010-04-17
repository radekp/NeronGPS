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
#include "qtmoko/fullscreen.h"
#include "qtmoko/power.h"
#include "qtmoko/qtmoko.h"
#include "include/nerongps.h"

TQtMoko::TQtMoko(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	_rootDir = QString("/media/card/NeronGPS");

	if(!TPower::gpsGetPower()) {
		qDebug() << "Power-up GPS";
		TPower::gpsSetPower(true);
	} else {
		qDebug() << "GPS already on";
	}

	_gpsSource = new TGpsSourceAbout();

	QtopiaApplication::setInputMethodHint(this, QtopiaApplication::AlwaysOff);
	_menu = QSoftMenuBar::menuFor(this);

	_mainWidget = NULL;
	_fullScreen = NULL;

	setObjectName("NeronGPS");
	setWindowTitle(tr("NeronGPS", "application header"));
	setCentralWidget(&_stack);
	showMaximized();

	new TNeronGPS(*this);
}

TQtMoko::~TQtMoko()
{
	delete _gpsSource;
}

void TQtMoko::closeEvent(QCloseEvent *event)
{
	QWidget *current = _stack.currentWidget();

	if((_mainWidget != NULL) && (current != _mainWidget)) {
		event->ignore();
		delete current;
	} else {
		QMessageBox dialog(QMessageBox::Question, "Closing", "Exit?", QMessageBox::Yes | QMessageBox::No);
		if(dialog.exec() == QMessageBox::Yes) {
			event->accept();
		} else {
			event->ignore();
		}
	}
}

void TQtMoko::setMainWidget(QWidget *widget)
{
	_mainWidget = widget;

	widget->resize(this->size());

	if(_fullScreen != NULL) {
		_fullScreenWidget->addWidget(widget);
	} else {
		_stack.addWidget(widget);
	}

	widget->show();
}

void TQtMoko::toggleFullScreen()
{
	if(_fullScreen == NULL) {
		_fullScreen = new TFullScreen(this);
		_fullScreenWidget = new QStackedWidget(_fullScreen);
		_fullScreen->setCentralWidget(_fullScreenWidget);

		if(_mainWidget != NULL) {
			_stack.removeWidget(_mainWidget);
			_fullScreenWidget->addWidget(_mainWidget);
		}

		_fullScreen->setWindowState(Qt::WindowFullScreen);
	} else {
		if(_mainWidget != NULL) {
			_fullScreenWidget->removeWidget(_mainWidget);
			_stack.addWidget(_mainWidget);
		}

		delete _fullScreen;

		_fullScreen = NULL;
		_fullScreenWidget = NULL;
	}
}

void TQtMoko::newForm(QWidget *form)
{
	connect(form, SIGNAL(destroyed(QObject *)), this, SLOT(slotDestroyWidget(QObject *)));

	form->setWindowState(Qt::WindowMaximized);
	form->show();

	_stack.addWidget(form);
	_stack.setCurrentWidget(form);
}

void TQtMoko::slotDestroyWidget(QObject *obj)
{
	_stack.removeWidget((QWidget *)obj);
}

void TQtMoko::displayAlwaysOn(bool alwaysOn)
{
	if(alwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}
}


