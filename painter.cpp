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

#include "include/painter.h"

TPainter::TPainter()
{
}

TPainter::~TPainter()
{
}

void TPainter::configure(const QString &drawString)
{
	QStringList list = drawString.split('/');

	int i;
	for(i = 0; i < list.size(); i++) {
		QString header = list[i].section('(', 0, 0);
		QString params = list[i].section('(', -1);
		params.remove(')');

		if(header == "line") {
			_elements.append(new TPainterElementLine(params));
		} else if(header == "rect1") {
			_elements.append(new TPainterElementTransparentRect(params));
		} else if(header == "rect2") {
			_elements.append(new TPainterElementOpaqueRectangle(params));
		} else if(header == "ell1") {
			_elements.append(new TPainterElementTransparentEllipse(params));
		} else if(header == "ell2") {
			_elements.append(new TPainterElementOpaqueEllipse(params));
		} else if(header == "seg1") {
			_elements.append(new TPainterElementSegment(params));
		} else if(header == "seg2") {
			_elements.append(new TPainterElementPolygon(params));
		} else {
			qDebug() << "Error: unknow painter element, " << header;
		}
	}
}

void TPainter::draw(QPainter &painter, int x, int y, int w, int h)
{
	int i;

	for(i = 0; i < _elements.size(); i++) {
		_elements[i]->draw(painter, x, y, w, h);
	}
}


