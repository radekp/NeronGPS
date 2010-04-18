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
#include <QDir>

#include "desktop/desktop.h"
#include "include/nerongps.h"

TDesktop::TDesktop()
{
        _rootDir = QDir::homePath() + QString("/NeronGPS");
qDebug() << "Home directory: " << _rootDir;
        _menu = menuBar()->addMenu(QString("&Menu"));

	setObjectName("NeronGPS");
	setWindowTitle(tr("NeronGPS", "application header"));
	showMaximized();

	new TNeronGPS(*this);
}

TDesktop::~TDesktop()
{
}

void TDesktop::closeEvent(QCloseEvent *event)
{
	QMessageBox dialog(QMessageBox::Question, "Closing", "Exit?", QMessageBox::Yes | QMessageBox::No);
	if(dialog.exec() == QMessageBox::Yes) {
		event->accept();
	} else {
		event->ignore();
	}
}

void TDesktop::setMainWidget(QWidget *widget)
{
	setCentralWidget(widget);

	widget->setParent(this);
	widget->resize(this->size());
	widget->show();
}

void TDesktop::toggleFullScreen()
{
	if(windowState() & Qt::WindowFullScreen) {
		showMaximized();
	} else {
		showFullScreen();
	}
}

void TDesktop::newForm(QWidget *form)
{
	form->setParent(this);
	form->show();
}

void TDesktop::displayAlwaysOn(bool /*alwaysOn*/)
{
}


