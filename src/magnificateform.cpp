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
#include <QtDebug>

#include <QTimer>

#include "include/magnificateform.h"

TMagnificateForm::TMagnificateForm(int magnification, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	switch(magnification) {
		case 0:
			ui.x1->setDisabled(true);
			break;
			
		case 1:
			ui.x2->setDisabled(true);
			break;
			
		case 2:
			ui.x4->setDisabled(true);
			break;
			
	}

	connect(ui.x1, SIGNAL(clicked(bool)), this, SLOT(slotX1Clicked(bool)));
	connect(ui.x2, SIGNAL(clicked(bool)), this, SLOT(slotX2Clicked(bool)));
	connect(ui.x4, SIGNAL(clicked(bool)), this, SLOT(slotX4Clicked(bool)));
}

TMagnificateForm::~TMagnificateForm()
{
}

void TMagnificateForm::slotX1Clicked(bool /*checked*/)
{
	hide();
	emit signalMagnification(0);
	QTimer::singleShot(1, this, SLOT(slotKill()));
}

void TMagnificateForm::slotX2Clicked(bool /*checked*/)
{
	hide();
	emit signalMagnification(1);
	QTimer::singleShot(1, this, SLOT(slotKill()));
}

void TMagnificateForm::slotX4Clicked(bool /*checked*/)
{
	hide();
	emit signalMagnification(2);
	QTimer::singleShot(1, this, SLOT(slotKill()));
}

void TMagnificateForm::slotKill()
{
	delete this;
}

