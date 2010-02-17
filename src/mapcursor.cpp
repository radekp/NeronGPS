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

#include "include/mapcursor.h"

TMapCursor::TMapCursor()
{
}

TMapCursor::~TMapCursor()
{
}

void TMapCursor::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	QString shape = settings.getValue("shape", "seg2(50,0,0,100,50,60,100,100)").toString();
	_size = settings.getValue("size", 25).toInt();
	_fixColor = settings.getColor("fixcolor", "00ff0080");
	_lostColor = settings.getColor("lostcolor", "ff000080");

	_shadowLevel = settings.getValue("shadowLevel", 64).toInt();
	_shadowX = settings.getValue("shadowx", 10).toInt();
	_shadowY = settings.getValue("shadowy", 10).toInt();

	settings.endGroup();

	_paint.configure(shape);
}

void TMapCursor::draw(QPainter &painter, TDrawState &drawState)
{
	if(drawState.validPos()) {
		QColor color = (drawState.fix()) ? _fixColor : _lostColor;
		int x = drawState.locationX();
		int y = drawState.locationY();
		qreal angle = drawState.currentAngle();
		int size = drawState.magnify(_size);
		int posX = -size/2;
		int posY = -size;

		QPen pen;
		pen.setCapStyle(Qt::RoundCap);

		painter.save();
		painter.translate(x + _shadowX, y + _shadowY);
		painter.rotate(angle);

		painter.setPen(QPen(Qt::NoPen));
		painter.setBrush(QBrush(QColor(0, 0, 0, _shadowLevel)));
		_paint.draw(painter, posX, posY, size, size);

		painter.restore();

		painter.save();
		painter.translate(x, y);
		painter.rotate(angle);

		pen.setWidth(3);
		pen.setColor(QColor(0, 0, 0));
		painter.setPen(pen);
		painter.setBrush(QBrush(color));
		_paint.draw(painter, posX, posY, size, size);

		pen.setWidth(1);
		pen.setColor(QColor(0xff, 0xff, 0xff));
		painter.setPen(pen);
		painter.setBrush(QBrush(Qt::NoBrush));
		_paint.draw(painter, posX, posY, size, size);

		painter.restore();
	}
}

