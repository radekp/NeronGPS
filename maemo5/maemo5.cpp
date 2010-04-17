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
#include <QMessageBox>
#include <QCloseEvent>

#include "maemo5/maemo5.h"
#include "maemo5/gpssourcelibloc.h"
#include "include/nerongps.h"

TMaemo5::TMaemo5()
{
        _rootDir = QString("/media/mmc1/NeronGPS");
        _gpsSource = new TGpsSourceLibloc;
        _menu = menuBar()->addMenu(QString("&Menu"));

	setObjectName("NeronGPS");
	setWindowTitle(tr("NeronGPS", "application header"));
	setAttribute(Qt::WA_Maemo5StackedWindow);
	showFullScreen();

	new TNeronGPS(*this);
}

TMaemo5::~TMaemo5()
{
	delete _gpsSource;
}

void TMaemo5::closeEvent(QCloseEvent *event)
{
	QMessageBox dialog(QMessageBox::Question, "Closing", "Exit?", QMessageBox::Yes | QMessageBox::No);
	if(dialog.exec() == QMessageBox::Yes) {
		event->accept();
	} else {
		event->ignore();
	}
}

void TMaemo5::setMainWidget(QWidget *widget)
{
	setCentralWidget(widget);

	widget->setParent(this);
	widget->resize(this->size());
	widget->show();
}

void TMaemo5::toggleFullScreen()
{
	if(windowState() & Qt::WindowFullScreen) {
		showNormal();
	} else {
		showFullScreen();
	}
}

void TMaemo5::newForm(QWidget *form)
{
	connect(form, SIGNAL(destroyed(QObject *)), this, SLOT(slotDestroyWidget(QObject *)));

	form->setParent(_forms.isEmpty() ? this : _forms.last());
	form->setAttribute(Qt::WA_Maemo5StackedWindow);
	form->setWindowFlags(form->windowFlags() | Qt::Window);
	form->setWindowState(Qt::WindowMaximized);
	form->show();

	_forms.append(form);
}

void TMaemo5::slotDestroyWidget(QObject *obj)
{
	_forms.removeAll((QWidget *)obj);
}

void TMaemo5::displayAlwaysOn(bool /*alwaysOn*/)
{
}


