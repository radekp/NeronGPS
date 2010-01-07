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

#include "include/painterelement.h"

TPainterElementLine::TPainterElementLine(const QString &params)
{
	_x1 = params.section(',', 0, 0).toInt();
	_y1 = params.section(',', 1, 1).toInt();
	_x2 = params.section(',', 2, 2).toInt();
	_y2 = params.section(',', 3, 3).toInt();
}

TPainterElementLine::~TPainterElementLine()
{
}

void TPainterElementLine::draw(QPainter &painter, int x, int y, int w, int h)
{
	painter.drawLine(x + (_x1 * w) / 100, y + (_y1 * h) / 100, x + (_x2 * w) / 100,	y + (_y2 * h) / 100);
}

TPainterElementTransparentRect::TPainterElementTransparentRect(const QString &params)
{
	_x1 = params.section(',', 0, 0).toInt();
	_y1 = params.section(',', 1, 1).toInt();
	_w = params.section(',', 2, 2).toInt();
	_h = params.section(',', 3, 3).toInt();
}

TPainterElementTransparentRect::~TPainterElementTransparentRect()
{
}

void TPainterElementTransparentRect::draw(QPainter &painter, int x, int y, int w, int h)
{
	QBrush brush = painter.brush();

	painter.setBrush(Qt::transparent);
	painter.drawRect(x + (_x1 * w) / 100, y + (_y1 * h) / 100, (_w * w) / 100, (_h * h) / 100);

	painter.setBrush(brush);
}

TPainterElementOpaqueRectangle::TPainterElementOpaqueRectangle(const QString &params)
{
	_x1 = params.section(',', 0, 0).toInt();
	_y1 = params.section(',', 1, 1).toInt();
	_w = params.section(',', 2, 2).toInt();
	_h = params.section(',', 3, 3).toInt();
}

TPainterElementOpaqueRectangle::~TPainterElementOpaqueRectangle()
{
}

void TPainterElementOpaqueRectangle::draw(QPainter &painter, int x, int y, int w, int h)
{
	painter.drawRect(x + (_x1 * w) / 100, y + (_y1 * h) / 100, (_w * w) / 100, (_h * h) / 100);
}

TPainterElementTransparentEllipse::TPainterElementTransparentEllipse(const QString &params)
{
	_x1 = params.section(',', 0, 0).toInt();
	_y1 = params.section(',', 1, 1).toInt();
	_w = params.section(',', 2, 2).toInt();
	_h = params.section(',', 3, 3).toInt();
}

TPainterElementTransparentEllipse::~TPainterElementTransparentEllipse()
{
}

void TPainterElementTransparentEllipse::draw(QPainter &painter, int x, int y, int w, int h)
{
	QBrush brush = painter.brush();

	painter.setBrush(Qt::transparent);
	painter.drawEllipse(x + (_x1 * w) / 100, y + (_y1 * h) / 100, (_w * w) / 100, (_h * h) / 100);

	painter.setBrush(brush);
}

TPainterElementOpaqueEllipse::TPainterElementOpaqueEllipse(const QString &params)
{
	_x1 = params.section(',', 0, 0).toInt();
	_y1 = params.section(',', 1, 1).toInt();
	_w = params.section(',', 2, 2).toInt();
	_h = params.section(',', 3, 3).toInt();
}

TPainterElementOpaqueEllipse::~TPainterElementOpaqueEllipse()
{
}

void TPainterElementOpaqueEllipse::draw(QPainter &painter, int x, int y, int w, int h)
{
	painter.drawEllipse(x + (_x1 * w) / 100, y + (_y1 * h) / 100, (_w * w) / 100, (_h * h) / 100);
}

TPainterElementSegment::TPainterElementSegment(const QString &params)
{
	bool stop = false;	
	int i;

	for(i = 0; !stop; i += 2) {
		QString xString = params.section(',', i, i);
		QString yString = params.section(',', i + 1, i + 1);

		if((xString == "") || (yString == "")) {
			stop = true;
		} else {
			bool ok1, ok2;

			int x = xString.toInt(&ok1);
			int y = yString.toInt(&ok2);

			if(ok1 && ok2) {
				_points.append(x);
				_points.append(y);
			} else {
				stop = true;
			}
		}
	}
}

TPainterElementSegment::~TPainterElementSegment()
{
}

void TPainterElementSegment::draw(QPainter &painter, int x, int y, int w, int h)
{
	QPolygon polygon;

	int i;
	for(i = 0; i < _points.size(); i += 2) {
		polygon << QPoint(x + (_points[i] * w) / 100, y + (_points[i + 1] * h) / 100);
	}

	painter.drawPolyline(polygon);
}

TPainterElementPolygon::TPainterElementPolygon(const QString &params)
{
	bool stop = false;	
	int i;

	for(i = 0; !stop; i += 2) {
		QString xString = params.section(',', i, i);
		QString yString = params.section(',', i + 1, i + 1);

		if((xString == "") || (yString == "")) {
			stop = true;
		} else {
			bool ok1, ok2;

			int x = xString.toInt(&ok1);
			int y = yString.toInt(&ok2);

			if(ok1 && ok2) {
				_points.append(x);
				_points.append(y);
			} else {
				stop = true;
			}
		}
	}
}

TPainterElementPolygon::~TPainterElementPolygon()
{
}

void TPainterElementPolygon::draw(QPainter &painter, int x, int y, int w, int h)
{
	QPolygon polygon;

	int i;
	for(i = 0; i < _points.size(); i += 2) {
		polygon << QPoint(x + (_points[i] * w) / 100, y + (_points[i + 1] * h) / 100);
	}
	painter.drawPolygon(polygon);
}

