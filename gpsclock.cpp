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
#include <QMessageBox>
#include <QtopiaServiceRequest>
#include <QTimeZone>

#include "include/gpsclock.h"
#include "include/converter.h"

TGpsClock::TGpsClock()
{
	_fix = false;
	_sync = false;

	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

TGpsClock::~TGpsClock()
{
}

void TGpsClock::resend()
{
	if(_timeZone.isValid()) {
		emit signalTimeZone(_timeZone);
	}
}

void TGpsClock::slotGpsData(const QWhereaboutsUpdate &update)
{
	QMutexLocker locker(&_mutex);

	if(!_timeZone.isValid()) {
		computeTimeZone(update);
	}

	_lastGpsTime = update.updateDateTime();
	_time.start();

	if(!_fix) {
		_fix = true;
		_timer.setInterval(1000);
		_timer.start();
	}
}

void TGpsClock::slotTimer()
{
	QMutexLocker locker(&_mutex);

	if(_sync) {
		_sync = false;

		QDateTime time = _lastGpsTime.addMSecs(_time.elapsed() - 500);
		time.setTimeSpec(Qt::UTC);

		QtopiaServiceRequest req("TimeUpdate", "changeSystemTime(uint,QString)");
		req << (uint)time.toTime_t() << QTimeZone::current().id();
		req.send();
	}

	signalClock(_lastGpsTime.addMSecs(_time.elapsed()));
}

void TGpsClock::slotSync()
{
	QMutexLocker locker(&_mutex);

	_sync = true;
}

void TGpsClock::computeTimeZone(const QWhereaboutsUpdate &update)
{
	double lat = update.coordinate().latitude();
	double lon = update.coordinate().longitude();

	QStringList ids = QTimeZone::ids();

	int i;
	float distance = 0;
	for(i = 0; i < ids.size(); i++) {
		QTimeZone tmpZone(ids[i].toAscii().data());
		float dist = TConverter::distance(lat, lon, ((double)tmpZone.latitude()) / 3600, ((double)tmpZone.longitude()) / 3600, 0);

		if(!_timeZone.isValid() || (dist < distance)) {
			distance = dist;
			_timeZone = tmpZone;
		}
	}

	if(_timeZone.isValid()) {
		emit signalTimeZone(_timeZone);
	}
}

