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

#include <QTime>
#include <QDateTime>
#include <QtopiaServiceRequest>

#include "include/clockform.h"

TClockForm::TClockForm(QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	ui.gpsTime->setSmallDecimalPoint(true); 
	ui.gpsTime->display("--.--.--");

	ui.systemTime->setSmallDecimalPoint(true); 
	ui.systemTime->display("--.--.--");

	ui.sysTZ->setText("System: " + formatTimeZone(QTimeZone::current()));

	_timer.setInterval(1000);
	_timer.start();

	ui.setTZ->setDisabled(true);
	ui.sync->setDisabled(true);

	connect(ui.sync, SIGNAL(clicked(bool)), this, SLOT(slotSync(bool)));
	connect(ui.setTZ, SIGNAL(clicked(bool)), this, SLOT(slotSetTZ(bool)));
	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

TClockForm::~TClockForm()
{
}

void TClockForm::slotTimer()
{
	QString str = QTime::currentTime().toString(Qt::ISODate);
	ui.systemTime->display(str.replace(':', '.'));

	if(!_lastGPSTimeValidity.isNull() && (_lastGPSTimeValidity.elapsed() < 5000)) {
		QString str = _lastGPSTime.time().toString(Qt::ISODate);
		ui.gpsTime->display(str.replace(':', '.'));
	} else {
		ui.gpsTime->display("--.--.--");
	}
}

void TClockForm::slotClock(QDateTime time)
{
	if(_lastGPSTimeValidity.isNull()) {
		ui.sync->setDisabled(false);
	}

	_lastGPSTime = time;
	_lastGPSTimeValidity.start();
}

void TClockForm::slotTimeZone(QTimeZone timeZone)
{
	_timeZone = timeZone;

	ui.setTZ->setText("Set TZ to " + formatTimeZone(timeZone));
	ui.setTZ->setDisabled(false);
}

void TClockForm::slotSync(bool /*checked*/)
{
	emit signalSync();
}

void TClockForm::slotSetTZ(bool /*checked*/)
{
	QDateTime time = QDateTime::currentDateTime();
	time.setTimeSpec(Qt::LocalTime);

	time = QTimeZone::current().toUtc(time);  
	time = _timeZone.fromUtc(time);

	ui.sysTZ->setText(formatTimeZone(_timeZone));

	QtopiaServiceRequest req("TimeUpdate", "changeSystemTime(uint,QString)");
	req << (uint)time.toTime_t() << _timeZone.id();
	req.send();
}

QString TClockForm::formatTimeZone(const QTimeZone &timeZone)
{
	return timeZone.id();
}

