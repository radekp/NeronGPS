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

#ifndef GPSCLOCK_H
#define GPSCLOCK_H

#include <QMutex>
#include <QTimer>
#include <QTime>

#include "include/gpssample.h"

class TGpsClock : public QObject
{
	Q_OBJECT
	public:
		TGpsClock();
		~TGpsClock();

	public slots:
		void slotGpsSample(TGpsSample sample);
		void slotTimer();

	signals:
		void signalClock(QDateTime time);

	private:
		QMutex _mutex;

		bool _fix;
		QTimer _timer;
		QDateTime _lastGpsTime;
		QTime _time;
};

#endif

