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

#ifndef TRACEPAINTER_H
#define TRACEPAINTER_H

#include <QObject>
#include <QList>
#include <QPainter>

class TTracePainter : public QObject
{
	Q_OBJECT
	public:
		TTracePainter();
		~TTracePainter();

		void reset(QPainter *painter, QRect &boundaries, int distance);
		void nextPoint(int x, int y);
		void end();

	private:
		QRect _boundaries;
		QPolygon _polyLine;
		QPainter *_painter;
		bool _first;
		bool _drawing;
		int _distance;
		int _x, _y;
		int _xx, _yy;

		void draw();
};

#endif

