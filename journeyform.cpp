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

#include "include/journeyform.h"
#include "include/gpsstatistics.h"

TJourneyForm::TJourneyForm(int samples, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	if(samples < 0) {
		ui.samples->setText("No recording");
	} else {
		ui.samples->setText(QString::number(samples));
	}

	connect(ui.reset, SIGNAL(clicked(bool)), this, SLOT(slotReset(bool)));
}

TJourneyForm::~TJourneyForm()
{
}

void TJourneyForm::slotGpsData(const QWhereaboutsUpdate &update)
{
	ui.location->setText(update.coordinate().toString());
}

void TJourneyForm::slotNewStat(int time, int distance, float speed, int altitude, int altMin, int altMax, int fix)
{
	QString str;
	int hours, minutes, seconds;

	hours = time / 3600;
	minutes = (time - hours * 3600) / 60;
	seconds = (time - hours * 3600 - minutes * 60);
	str = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
	ui.time->setText(str);

	if(distance < 1000) {	
		str = QString::number(distance) + " m";
	} else if (distance < 10000) {
		str = QString("%1 km").arg(((float)distance) / 1000., 0, 'f', 2);
	} else {
		str = QString("%1 km").arg(((float)distance) / 1000., 0, 'f', 1);
	}
	float average = ((float)distance / (float)time) * 3.6;
	if(average < 10) {
		str += QString(", at %1 km/h").arg(average, 0, 'f', 1);
	} else {
		str += QString(", at %1 km/h").arg((int)average);
	}
	ui.distance->setText(str);
	
	if(speed == STAT_INVALID_SPEED) {
		str = "-";
	} else if(speed < 10) {
		str = QString("%1 km/h").arg(speed, 0, 'f', 1);
	} else {
		str = QString("%1 km/h").arg((int)speed);
	}
	ui.speed->setText(str);

	if(altitude == STAT_INVALID_ALTITUDE) {
		str = "-";
	} else {
		str = QString::number(altMin) + " m < " + QString::number(altitude) + " m > " + QString::number(altMax) + " m";
	}
	ui.altitude->setText(str);

	str = QString::number(fix) + "%";
	ui.fix->setText(str);
}

void TJourneyForm::slotRecordInfo(QString /*name*/, int samples)
{
	ui.samples->setText(QString::number(samples));
}

void TJourneyForm::slotReset(bool /*checked*/)
{
	emit signalReset();
}

