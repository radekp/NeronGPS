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
#include <QPen>
#include <QBrush>
#include <QPainter>

#include "include/locationpointer.h"

TLocationPointer::TLocationPointer()
{
}

TLocationPointer::~TLocationPointer()
{
}

void TLocationPointer::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	QString shape = settings.getValue("shape", "seg2(50,0,0,100,50,80,100,100)").toString();
	_fixBorder = settings.getColor("fixbordercolor", "004000c0");
	_fixFill = settings.getColor("fixfillcolor", "00ff0080");
	_lostBorder = settings.getColor("lostbordercolor", "400000c0");
	_lostFill = settings.getColor("lostfillcolor", "ff000080");

	settings.endGroup();

	_painter.configure(shape);
}

void TLocationPointer::draw(QPainter &painter, TDrawState &drawState)
{
	if(drawState.validPos()) {
		QColor colorDraw;
		QColor colorFill;

		if(drawState.fix()) {
			colorDraw = _fixBorder;
			colorFill = _fixFill;
		} else {
			colorDraw = _lostBorder;
			colorFill = _lostFill;
		}

		QPen pen;
		pen.setWidth(1);
		pen.setCapStyle(Qt::RoundCap);
		pen.setColor(colorDraw);
		QBrush brush(colorFill);

		painter.save();
		painter.translate(drawState.locationX(), drawState.locationY());
		painter.rotate(drawState.validAngle() ? drawState.currentAngle() : 0);

		painter.setPen(pen);
		painter.setBrush(brush);

		int size = drawState.magnify(20);
		_painter.draw(painter, -size/2, -size, size, size);

		painter.restore();
	}
}

