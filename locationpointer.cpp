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

	QString locationShape = settings.getValue("locationshape", "seg2(50,0,0,100,50,60,100,100)").toString();
	_locationSize = settings.getValue("locationsize", 20).toInt();
	_fixColor = settings.getColor("fixcolor", "00ff0080");
	_lostColor = settings.getColor("lostcolor", "ff000080");

	QString driveShape = settings.getValue("driveshape", "ell2(0,0,100,100)/line(0,50,100,50)/line(50,0,50,100)").toString();
	_driveSize = settings.getValue("drivesize", 20).toInt();
	_driveColor = settings.getColor("drivecolor", "00ff0080");

	_shadowLevel = settings.getValue("shadowLevel", 64).toInt();
	_shadowX = settings.getValue("shadowx", 10).toInt();
	_shadowY = settings.getValue("shadowy", 10).toInt();


	settings.endGroup();

	_locationPaint.configure(locationShape);
	_drivePaint.configure(driveShape);
}

void TLocationPointer::draw(QPainter &painter, TDrawState &drawState)
{
	if(drawState.driveOn()) {
		int x = drawState.driveLocationX();
		int y = drawState.driveLocationY();
		int size = drawState.magnify(_driveSize);
		int posX = -size/2;
		int posY = -size/2;
		drawPointer(painter, _drivePaint, _driveColor, 0, x, y, posX, posY, size);
	}

	if(drawState.validPos()) {
		QColor color = (drawState.fix()) ? _fixColor : _lostColor;
		int x = drawState.locationX();
		int y = drawState.locationY();
		qreal angle = drawState.currentAngle();
		int size = drawState.magnify(_locationSize);
		int posX = -size/2;
		int posY = -size;
		drawPointer(painter, _locationPaint, color, angle, x, y, posX, posY, size);
	}
}

void TLocationPointer::drawPointer(QPainter &painter, TPainter &paint, QColor &color, qreal angle, int x, int y, int posX, int posY, int size)
{
	QPen pen;
	pen.setCapStyle(Qt::RoundCap);

	painter.save();
	painter.translate(x + _shadowX, y + _shadowY);
	painter.rotate(angle);

	painter.setPen(QPen(Qt::NoPen));
	painter.setBrush(QBrush(QColor(0, 0, 0, _shadowLevel)));
	paint.draw(painter, posX, posY, size, size);

	painter.restore();

	painter.save();
	painter.translate(x, y);
	painter.rotate(angle);

	pen.setWidth(3);
	pen.setColor(QColor(0, 0, 0));
	painter.setPen(pen);
	painter.setBrush(QBrush(color));
	paint.draw(painter, posX, posY, size, size);

	pen.setWidth(1);
	pen.setColor(QColor(0xff, 0xff, 0xff));
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::NoBrush));
	paint.draw(painter, posX, posY, size, size);

	painter.restore();
}

