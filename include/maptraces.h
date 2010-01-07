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

#ifndef MAPTRACES_H
#define MAPTRACES_H

#include <QObject>
#include <QList>
#include <QPainterPath>
#include <QMutex>
#include <QWhereaboutsUpdate>

#include "include/trace.h"
#include "include/drawstate.h"
#include "include/mapdrawlist.h"

class TMapTraces : public TMapDrawListElement
{
	Q_OBJECT
	public:
		TMapTraces();
		~TMapTraces();

		void configure(TSettings &settings, const QString &section);
		void draw(QPainter &painter, TDrawState &drawState);

	public slots:
		void slotNewTraces(QList<TTrace *> *traces);
		void slotClear();

	signals:
		void signalTraceLoaded(int xmin, int xmax, int ymin, int ymax);

	private:
		QMutex _mutex;

		QList<TTrace *> _traces;

		QString _traceDir;
		QPen _tracePen;
		int _tracePoint;
		int _traceTransparency;
};

#endif

