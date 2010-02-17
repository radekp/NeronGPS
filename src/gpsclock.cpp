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

#include "include/gpsclock.h"
#include "include/converter.h"

TGpsClock::TGpsClock()
{
	_fix = false;

	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

TGpsClock::~TGpsClock()
{
}

void TGpsClock::slotGpsSample(TGpsSample sample)
{
	QMutexLocker locker(&_mutex);

	_lastGpsTime = sample.time();
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

	signalClock(_lastGpsTime.addMSecs(_time.elapsed()));
}

