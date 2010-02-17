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

#include "include/tracepainter.h"

TTracePainter::TTracePainter()
{
}

TTracePainter::~TTracePainter()
{
}

void TTracePainter::reset(QPainter *painter, QRect &boundaries, int distance)
{
	_painter = painter;
	_first = true;
	_drawing = false;
	_distance = distance;
	_polyLine.clear();
	_boundaries = boundaries;
}

void TTracePainter::nextPoint(int x, int y)
{
	if(_first) {
		_first = false;
		_x = x; _y = y;
		if(_boundaries.contains(x, y)) {
			_polyLine << QPoint(x, y);
		}
	} else if((x - _x) * (x - _x) + (y - _y) * (y - _y) > _distance) {
		if(_drawing) {
			if(_boundaries.contains(x, y)) {
				_polyLine << QPoint(x, y);
			} else {
				_polyLine << QPoint(x, y);
				draw();
				_drawing = false;
			}
		} else {
			if(_boundaries.contains(x, y)) {
				_polyLine << QPoint(_x, _y);
				_polyLine << QPoint(x, y);
				_drawing = true;
			}
		}
		
		_x = x; _y = y;
	}

	_xx = x; _yy = y;
}

void TTracePainter::end()
{
	if((!_first) && ((_xx != _x) || (_yy != _y)) && (_drawing || _boundaries.contains(_xx, _yy))) {
		_polyLine << QPoint(_xx, _yy);
	}

	draw();

	_first = true;
	_drawing = false;
}

void TTracePainter::draw()
{
	_painter->drawPolyline(_polyLine);
	_polyLine.clear();
}

