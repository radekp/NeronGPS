/*
 * Copyright 2009 Thierry Vuillaume
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

class TGpsData : public QObject
{
	Q_OBJECT
	public:
		TGpsData();
		~TGpsData();

	public slots:
		void slotGpsData(const QWhereaboutsUpdate &update);

	signals:
		void signalGpsData(int x, int y, qreal angle, bool angleValid);

	private:
		bool _lastValid;
		bool _lastSkipped;
		int _lastX;
		int _lastY;

		bool _lastCourseValid;
		qreal _lastCourse;
};

#endif

