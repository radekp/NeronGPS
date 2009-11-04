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

#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>

#include "include/trace.h"
#include "include/converter.h"
#include "include/tracepainter.h"

TTrace::TTrace()
{
	connect(&_loader, SIGNAL(sendSegment(TTraceSegment *)), this, SLOT(newSegment(TTraceSegment *)), Qt::QueuedConnection);
}

TTrace::~TTrace()
{
	while(_segments.size() > 0) {
		delete _segments.takeFirst();
	}
}

void TTrace::newSegment(TTraceSegment *seg)
{
	if(seg != NULL) {
		addSegment(seg);
		emit signalTraceLoading(this, _segments.size() * SEGMENT_SIZE);
	} else {
		emit signalTraceLoaded(this);
	}
}

bool TTrace::loadLog(const QString &filename)
{
	bool success;

	QMutexLocker locker(&_mutex);

	while(_segments.size() > 0) {
		delete _segments.takeFirst();
	}

	success = _loader.setFile(filename);
	if (success) {
		_name = filename.section('/', -1).section('.', 0);
		_loader.start();
	}
	return success;
}

bool TTrace::loadBin(const QString &filename)
{
	_mutex.lock();

	while(_segments.size() > 0) {
		delete _segments.takeFirst();
	}

	_mutex.unlock();

	QFile file(filename);

	bool success = file.open(QIODevice::ReadOnly);
	if (success) {
		_name = filename.section('/', -1).section('.', 0, 0);
		while(!file.atEnd()) {
			TTraceSegment *seg = new TTraceSegment;
			seg->readBin(file);
			addSegment(seg);
		}
	} else {
		qDebug() << "Error opening file: " << filename;	
	}
	
	return success;
	
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

void TTrace::addSegment(TTraceSegment *seg)
{
	QMutexLocker locker(&_mutex);

	_segments << seg;
		
	if(_segments.size() == 1) {
		_xmin = seg->xmin();
		_xmax = seg->xmax();
		_ymin = seg->ymin();
		_ymax = seg->ymax();
	} else {
		if(seg->xmin() < _xmin) { _xmin = seg->xmin(); } else if(seg->xmax() > _xmax) { _xmax = seg->xmax(); }
		if(seg->ymin() < _ymin) { _ymin = seg->ymin(); } else if(seg->ymax() > _ymax) { _ymax = seg->ymax(); }
	}
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

