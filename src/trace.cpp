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

#include "include/trace.h"
#include "include/converter.h"
#include "include/tracepainter.h"

TTrace::TTrace()
{
}

TTrace::~TTrace()
{
	while(_segments.size() > 0) {
		delete _segments.takeFirst();
	}
}

void TTrace::setPen(const QPen &pen, int drawDistance)
{
	_pen = pen;
	_drawDistance = drawDistance;
}

int TTrace::getNumberOfSamples()
{
	return ((_segments.size() - 1) * SEGMENT_SIZE) + _segments.last()->size();	
}

void TTrace::addSample(int x, int y)
{
	QMutexLocker locker(&_mutex);

	if(_segments.size() == 0) {
		_segments << new TTraceSegment;
		_xmin = x;
		_xmax = x;
		_ymin = y;
		_ymax = y;
	} else {
		if (_segments.last()->isFull()) {
			_segments << new TTraceSegment;
		}
		if(x < _xmin) {	_xmin = x; } else if(x > _xmax) { _xmax = x; }
		if(y < _ymin) { _ymin = y; } else if(y > _ymax) { _ymax = y; }
	}

	_segments.last()->addSample(x, y);
}

void TTrace::draw(QPainter &painter, TDrawState &drawState)
{
	int i;

	QRect boundaries(0, 0, drawState.width(), drawState.height());

	TTracePainter multi; 
	multi.reset(&painter, boundaries, _drawDistance);

	QPen pen = painter.pen();
	painter.setPen(_pen);

	QMutexLocker locker(&_mutex);

	for (i = 0; i < _segments.size(); i++) {
		int x1 = TConverter::convert(_segments[i]->xmin(), drawState.zoom()) + drawState.translatX();
		int x2 = TConverter::convert(_segments[i]->xmax(), drawState.zoom()) + drawState.translatX();
		int y1 = TConverter::convert(_segments[i]->ymin(), drawState.zoom()) + drawState.translatY();
		int y2 = TConverter::convert(_segments[i]->ymax(), drawState.zoom()) + drawState.translatY();

		if( (x1 < drawState.width()) && (x2 >= 0) && (y1 < drawState.height()) && (y2 >= 0) ) {
			_segments[i]->draw(&multi, drawState.translatX(), drawState.translatY(), drawState.zoom());
		} else {
			_segments[i]->drawEndPoints(&multi, drawState.translatX(), drawState.translatY(), drawState.zoom());
		}
	}

	multi.end();

	painter.setPen(pen);
}

void TTrace::reset()
{
	QMutexLocker locker(&_mutex);

	while(_segments.size() > 0) {
		delete _segments.takeFirst();
	}
}

