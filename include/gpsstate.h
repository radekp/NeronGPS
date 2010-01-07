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

#ifndef GPSSTATE_H
#define GPSSTATE_H

#include <QMutex>
#include <QTimer>
#include <QWhereabouts>
#include <QWhereaboutsUpdate>

#define GPSSTATE_TIMEOUT	5

class TGpsState : public QObject
{
	Q_OBJECT
	public:
		TGpsState();
		~TGpsState();

	public slots:
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotGpsState(QWhereabouts::State state);
		void slotTimer();
		void slotSingleShot();

	signals:
		void signalGpsTime(int time);
		void signalGpsState(bool fix);

	private:
		QMutex _mutex;
		bool _fix;
		QTimer _singleShot;
		QTimer _timer;
		QTime _time;

		void goFix();
		void goNoFix();
};

#endif

