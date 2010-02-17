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
#include <QPen>
#include <QBrush>
#include <QPainter>

#include "include/mapcross.h"

TMapCross::TMapCross()
{
}

TMapCross::~TMapCross()
{
}

void TMapCross::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	QString shape = settings.getValue("crossshape", "line(0,50,100,50)/line(50,0,50,100)").toString();
	_width = settings.getValue("crosswidth", 20).toInt();
	_height = settings.getValue("crossheight", 20).toInt();

	settings.endGroup();

	_painter.configure(shape);
}

void TMapCross::draw(QPainter &painter, TDrawState &drawState)
{
	if(!drawState.autoOn()) {
		QPen pen;
		pen.setWidth(1);
		pen.setCapStyle(Qt::RoundCap);
		pen.setColor(QColor(0, 0, 0));
		QBrush brush(QColor(0, 0, 0, 32));

		painter.setPen(pen);
		painter.setBrush(brush);

		_painter.draw(painter, (drawState.width() - _width) / 2, (drawState.height() - _height) / 2, _width, _height);

		pen.setColor(QColor(0xff, 0xff, 0xff));
		painter.setPen(pen);
		_painter.draw(painter, (drawState.width() - _width) / 2 + 1, (drawState.height() - _height) / 2 + 1, _width, _height);
	}
}

