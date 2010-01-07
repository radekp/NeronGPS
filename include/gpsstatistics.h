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

#ifndef GPSSTATISTICS_H
#define GPSSTATISTICS_H

#include <QObject>
#include <QWhereaboutsUpdate>
#include <QQueue>
#include <QMutex>

#include "include/settings.h"

#define STAT_INVALID_SPEED	(-100000)
#define STAT_INVALID_ALTITUDE	(-100000)

class TGpsStatistics : public QObject
{
	Q_OBJECT
	public:
		TGpsStatistics();
		~TGpsStatistics();

		void configure(TSettings &settings, const QString &section);
		void resend();

	public slots:
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotReset();

	signals:
		void signalStatistics(int time, int distance, float speed, int altitude, int altMin, int altMax, int fix);

	private:
		QMutex _mutex;

		int _accuracy;
		uint _lostTime;

		bool _first;
		uint _beginning;

		double _lastLat;
		double _lastLon;
		uint _lastSampleTime;
		uint _lastTime;

		float _distance;
		float _speed;
		float _altitude;
		float _altMin;
		float _altMax;

		uint _fixLost;
};

#endif

