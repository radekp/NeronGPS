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

#ifndef TRACESEGMENT_H
#define TRACESEGMENT_H

#include <QObject>
#include <QFile>
#include <QRect>
#include <QPainterPath>

#include "tracepainter.h"

#define SEGMENT_SIZE 256

struct point {
	int x;
	int y;
};

class TTraceSegment : public QObject
{
	Q_OBJECT
	public:
		TTraceSegment();
		~TTraceSegment();

		void readBin(QFile &file);
		void readSamples(QFile *file);
		void addSample(int x, int y);
		bool isFull();
		void draw(TTracePainter *multi, int translatX, int translatY, int zoom);
		void drawEndPoints(TTracePainter *multi, int translatX, int translatY, int zoom);

		int size(void) { return _size; }
		int xmin(void) { return _xmin; }
		int xmax(void) { return _xmax; }
		int ymin(void) { return _ymin; }
		int ymax(void) { return _ymax; }

	private:
		int _xmin, _xmax, _ymin, _ymax, _size;
		point _data[SEGMENT_SIZE];
};

#endif

