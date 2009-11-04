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

#ifndef TRACE_H
#define TRACE_H

#include <QObject>
#include <QList>
#include <QPainterPath>

#include "include/traceloader.h"
#include "include/tracesegment.h"
#include "include/drawstate.h"

class TTrace : public QObject
{
	Q_OBJECT
	public:
		TTrace();
		~TTrace();

		bool loadLog(const QString &filename);
		bool loadBin(const QString &filename);
		void setPen(const QPen &pen, int drawDistance);
		int getNumberOfSamples();
		void addSample(int x, int y);
		void draw(QPainter &painter, TDrawState &drawState);
		void reset();

		const QString &name() { return _name; }

		int xmin() { return _xmin; }
		int xmax() { return _xmax; }
		int ymin() { return _ymin; }
		int ymax() { return _ymax; }

	public slots:
		void newSegment(TTraceSegment *seg);

	signals:
		void signalTraceLoading(TTrace *trace, int samples);
		void signalTraceLoaded(TTrace *trace);

	private:
		QMutex _mutex;
		int _xmin, _xmax, _ymin, _ymax;
		QString _name;
		QList<TTraceSegment *> _segments;
		TTraceLoader _loader;
		QPen _pen;
		int _drawDistance;

		void addSegment(TTraceSegment *seg);
};

#endif

