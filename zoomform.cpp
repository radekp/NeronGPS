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

#include "include/zoomform.h"

TZoomForm::TZoomForm(int zoom, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	switch(zoom) {
		case 1:	ui.zoom1->setDisabled(true); break;
		case 2:	ui.zoom2->setDisabled(true); break;
		case 3:	ui.zoom3->setDisabled(true); break;
		case 4:	ui.zoom4->setDisabled(true); break;
		case 5:	ui.zoom5->setDisabled(true); break;
		case 6:	ui.zoom6->setDisabled(true); break;
		case 7:	ui.zoom7->setDisabled(true); break;
		case 8:	ui.zoom8->setDisabled(true); break;
		case 9:	ui.zoom9->setDisabled(true); break;
		case 10: ui.zoom10->setDisabled(true); break;
		case 11: ui.zoom11->setDisabled(true); break;
		case 12: ui.zoom12->setDisabled(true); break;
		case 13: ui.zoom13->setDisabled(true); break;
		case 14: ui.zoom14->setDisabled(true); break;
		case 15: ui.zoom15->setDisabled(true); break;
		case 16: ui.zoom16->setDisabled(true); break;
		case 17: ui.zoom17->setDisabled(true); break;
		case 18: ui.zoom18->setDisabled(true); break;
		case 19: ui.zoom19->setDisabled(true); break;
		case 20: ui.zoom20->setDisabled(true); break;
	}

	connect(ui.zoom1, SIGNAL(clicked(bool)), this, SLOT(slotZoom1Clicked(bool)));
	connect(ui.zoom2, SIGNAL(clicked(bool)), this, SLOT(slotZoom2Clicked(bool)));
	connect(ui.zoom3, SIGNAL(clicked(bool)), this, SLOT(slotZoom3Clicked(bool)));
	connect(ui.zoom4, SIGNAL(clicked(bool)), this, SLOT(slotZoom4Clicked(bool)));
	connect(ui.zoom5, SIGNAL(clicked(bool)), this, SLOT(slotZoom5Clicked(bool)));
	connect(ui.zoom6, SIGNAL(clicked(bool)), this, SLOT(slotZoom6Clicked(bool)));
	connect(ui.zoom7, SIGNAL(clicked(bool)), this, SLOT(slotZoom7Clicked(bool)));
	connect(ui.zoom8, SIGNAL(clicked(bool)), this, SLOT(slotZoom8Clicked(bool)));
	connect(ui.zoom9, SIGNAL(clicked(bool)), this, SLOT(slotZoom9Clicked(bool)));
	connect(ui.zoom10, SIGNAL(clicked(bool)), this, SLOT(slotZoom10Clicked(bool)));
	connect(ui.zoom11, SIGNAL(clicked(bool)), this, SLOT(slotZoom11Clicked(bool)));
	connect(ui.zoom12, SIGNAL(clicked(bool)), this, SLOT(slotZoom12Clicked(bool)));
	connect(ui.zoom13, SIGNAL(clicked(bool)), this, SLOT(slotZoom13Clicked(bool)));
	connect(ui.zoom14, SIGNAL(clicked(bool)), this, SLOT(slotZoom14Clicked(bool)));
	connect(ui.zoom15, SIGNAL(clicked(bool)), this, SLOT(slotZoom15Clicked(bool)));
	connect(ui.zoom16, SIGNAL(clicked(bool)), this, SLOT(slotZoom16Clicked(bool)));
	connect(ui.zoom17, SIGNAL(clicked(bool)), this, SLOT(slotZoom17Clicked(bool)));
	connect(ui.zoom18, SIGNAL(clicked(bool)), this, SLOT(slotZoom18Clicked(bool)));
	connect(ui.zoom19, SIGNAL(clicked(bool)), this, SLOT(slotZoom19Clicked(bool)));
	connect(ui.zoom20, SIGNAL(clicked(bool)), this, SLOT(slotZoom20Clicked(bool)));
}

TZoomForm::~TZoomForm()
{
}

void TZoomForm::slotZoom1Clicked(bool) { hide(); emit signalZoom(1); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom2Clicked(bool) { hide(); emit signalZoom(2); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom3Clicked(bool) { hide(); emit signalZoom(3); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom4Clicked(bool) { hide(); emit signalZoom(4); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom5Clicked(bool) { hide(); emit signalZoom(5); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom6Clicked(bool) { hide(); emit signalZoom(6); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom7Clicked(bool) { hide(); emit signalZoom(7); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom8Clicked(bool) { hide(); emit signalZoom(8); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom9Clicked(bool) { hide(); emit signalZoom(9); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom10Clicked(bool) { hide(); emit signalZoom(10); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom11Clicked(bool) { hide(); emit signalZoom(11); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom12Clicked(bool) { hide(); emit signalZoom(12); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom13Clicked(bool) { hide(); emit signalZoom(13); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom14Clicked(bool) { hide(); emit signalZoom(14); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom15Clicked(bool) { hide(); emit signalZoom(15); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom16Clicked(bool) { hide(); emit signalZoom(16); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom17Clicked(bool) { hide(); emit signalZoom(17); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom18Clicked(bool) { hide(); emit signalZoom(18); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom19Clicked(bool) { hide(); emit signalZoom(19); QTimer::singleShot(1, this, SLOT(slotKill())); }
void TZoomForm::slotZoom20Clicked(bool) { hide(); emit signalZoom(20); QTimer::singleShot(1, this, SLOT(slotKill())); }

void TZoomForm::slotKill()
{
	delete this;
}

