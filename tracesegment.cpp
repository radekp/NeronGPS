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

#include "include/tracesegment.h"
#include "include/converter.h"

TTraceSegment::TTraceSegment()
{
	_size = 0;
}

TTraceSegment::~TTraceSegment()
{
}

void TTraceSegment::readBin(QFile &file)
{
	_size = file.read((char *)_data, SEGMENT_SIZE * sizeof(point)) / sizeof(point);
	
	if(_size < 0) {
		_size = 0;
		qDebug() << "Error reading segment";
	} 

	_xmin = _xmax = _data[0].x;
	_ymin = _ymax = _data[0].y;

	int i;
	for(i = 1; i < _size; i++) {
		if(_data[i].x < _xmin) { _xmin = _data[i].x; } else if(_data[i].x > _xmax) { _xmax = _data[i].x; }
		if(_data[i].y < _ymin) { _ymin = _data[i].y; } else if(_data[i].y > _ymax) { _ymax = _data[i].y; }
	}
}

void TTraceSegment::readSamples(QFile *file)
{
	QString str;

	str = file->readLine(255);
	_data[0].x = TConverter::prepareX(str.section(',', 1, 1).toDouble());
	_data[0].y = TConverter::prepareY(str.section(',', 0, 0).toDouble());

	_xmin = _xmax = _data[0].x;
	_ymin = _ymax = _data[0].y;

	for(_size = 1; (_size < SEGMENT_SIZE) && (!file->atEnd()); _size++) {
		str = file->readLine(255);
		_data[_size].x =  TConverter::prepareX(str.section(',', 1, 1).toDouble());
		_data[_size].y = TConverter::prepareY(str.section(',', 0, 0).toDouble());
		
		if(_data[_size].x < _xmin) { _xmin = _data[_size].x; } else if(_data[_size].x > _xmax) { _xmax = _data[_size].x; }
		if(_data[_size].y < _ymin) { _ymin = _data[_size].y; } else if(_data[_size].y > _ymax) { _ymax = _data[_size].y; }
	}
}

void TTraceSegment::addSample(int x, int y)
{
	if(_size < SEGMENT_SIZE) {
		_data[_size].x = x;
		_data[_size].y = y;
		_size ++;

		if(x < _xmin) { _xmin = x; } else if(x > _xmax) { _xmax = x; }
		if(y < _ymin) {	_ymin = y; } else if(y > _ymax) { _ymax = y; }
	} else {
		qDebug() << "Attempt to add sample to segment full" ;
	}
}

bool TTraceSegment::isFull()
{
	return (_size == SEGMENT_SIZE);
}

void TTraceSegment::draw(TTracePainter *multi, int translatX, int translatY, int zoom)
{
	int i;

	for (i = 0; i < _size; i++) {
		multi->nextPoint(TConverter::convert(_data[i].x, zoom) + translatX, TConverter::convert(_data[i].y, zoom) + translatY);
	}
}

void TTraceSegment::drawEndPoints(TTracePainter *multi, int translatX, int translatY, int zoom)
{
	if(_size > 0) {
		multi->nextPoint(TConverter::convert(_data[0].x, zoom) + translatX, TConverter::convert(_data[0].y, zoom) + translatY);
	}

	if(_size > 1) {
		multi->nextPoint(TConverter::convert(_data[_size - 1].x, zoom) + translatX, TConverter::convert(_data[_size - 1].y, zoom) + translatY);
	}
}

