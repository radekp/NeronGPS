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
#include <QDir>

#include "include/maptraces.h"
#include "include/converter.h"
#include "include/global.h"

TMapTraces::TMapTraces()
{
}

TMapTraces::~TMapTraces()
{
}

void TMapTraces::configure(TSettings &settings, const QString &section)
{
	QPen pen;

	settings.beginGroup(section);

	_tracePen.setWidth(settings.getValue("tracethickness", 10).toInt());
	_tracePen.setCapStyle(Qt::RoundCap);

	_traceTransparency = settings.getValue("tracetransparency", 192).toInt();
	_tracePoint = settings.getValue("tracepoint", 400).toInt();

	settings.endGroup();
}

void TMapTraces::slotNewTraces(QList<TTrace *> *traces)
{
	if(traces->size() > 0) {
		_mutex.lock();
		_traces += *traces;
		_mutex.unlock();

		int xmin = (*traces)[0]->xmin();
		int xmax = (*traces)[0]->xmax();
		int ymin = (*traces)[0]->ymin();
		int ymax = (*traces)[0]->ymax();

		int i;
		for(i = 1; i < traces->size(); i++) {
			xmin = ((*traces)[i]->xmin() < xmin) ? (*traces)[i]->xmin() : xmin;
			xmax = ((*traces)[i]->xmax() > xmax) ? (*traces)[i]->xmax() : xmax;
			ymin = ((*traces)[i]->ymin() < ymin) ? (*traces)[i]->ymin() : ymin;
			ymax = ((*traces)[i]->ymax() > ymax) ? (*traces)[i]->ymax() : ymax;
		}

		emit signalTraceLoaded(xmin, xmax, ymin, ymax);
	}

	delete traces;
}

void TMapTraces::slotClear()
{
	QMutexLocker locker(&_mutex);

	while(_traces.size() > 0) {
		delete _traces.takeFirst();
	}
}

void TMapTraces::draw(QPainter &painter, TDrawState &drawState)
{
	if(!drawState.moving()) {
		QMutexLocker locker(&_mutex);

		int i;
		for(i = 0; i < _traces.size(); i++) {
			_traces[i]->draw(painter, drawState);
		}
	}
}

