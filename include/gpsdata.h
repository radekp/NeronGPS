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

#ifndef GPSDATA_H
#define GPSDATA_H

#include <QWhereabouts>
#include <QWhereaboutsUpdate>

#include "include/settings.h"

class TGpsData : public QObject
{
	Q_OBJECT
	public:
		TGpsData();
		~TGpsData();

		void configure(TSettings &settings, const QString &section);

	public slots:
		void slotGpsData(const QWhereaboutsUpdate &update);

	signals:
		void signalGpsData(bool noise, int x, int y, qreal angle);

	private:
		int _accuracy;
		int _rotationSpeed;

		float _altitude;
		bool _lastValid;
		bool _lastSkipped;
		int _lastX;
		int _lastY;
		qreal _lastCourse;
		double _lastLat;
		double _lastLon;
};

#endif

