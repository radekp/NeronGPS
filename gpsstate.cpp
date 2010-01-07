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

#include "include/gpsstate.h"
#include "include/global.h"

TGpsState::TGpsState()
{
	_fix = false;
	_time.start();

	_timer.setInterval(1000);
	_timer.start();
	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));

	_singleShot.setInterval(GPSSTATE_TIMEOUT * 1000);
	_singleShot.setSingleShot(true);
	connect(&_singleShot, SIGNAL(timeout()), this, SLOT(slotSingleShot()));
}

TGpsState::~TGpsState()
{
}

void TGpsState::slotTimer()
{
	QMutexLocker locker(&_mutex);

	if(!_fix) {
		int time = _time.elapsed() / 1000;

		emit signalGpsTime(time);

		QString str;
		int hours = time / 3600;
		int minutes = (time - hours * 3600) / 60;
		int seconds = (time - hours * 3600 - minutes * 60);

		str = ((hours < 10) ? "0" : "") + QString::number(hours) + ":";
		str += ((minutes < 10) ? "0" : "") + QString::number(minutes) + ":";
		str += ((seconds < 10) ? "0" : "") + QString::number(seconds);

		tMessage((void *)this) << "Wait for fix: " << str;
	}
}

void TGpsState::slotSingleShot()
{
	QMutexLocker locker(&_mutex);

	goNoFix();
}

void TGpsState::slotGpsData(const QWhereaboutsUpdate &/*update*/)
{
	QMutexLocker locker(&_mutex);

	goFix();
}

void TGpsState::slotGpsState(QWhereabouts::State state)
{
	QMutexLocker locker(&_mutex);

	if(state != QWhereabouts::PositionFixAcquired) {
		goNoFix();
	}
}

void TGpsState::goFix()
{
	if(!_fix) {
		_fix = true;
		_timer.stop();
		emit signalGpsState(true);
		tUserLog() << "GPS fix";
		tMessage((void *)this) << "";
	}

	_singleShot.start();
}

void TGpsState::goNoFix()
{
	if(_fix) {
		_fix = false;
		_singleShot.stop();
		_time.start();
		_timer.start();		
		emit signalGpsState(false);
		tUserLog() << "GPS fix lost";
		emit signalGpsTime(0);
	}
}

