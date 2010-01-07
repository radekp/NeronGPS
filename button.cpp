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
#include <QPainter>

#include "include/button.h"

TButton::TButton(QAction *action, const QString &drawString, int xHint, int yHint)
{
	_action = action;
	_xHint = xHint;
	_yHint = yHint;
	_pressed = false;
	_out = false;

	_painter.configure(drawString);
}

TButton::~TButton()
{
}

void TButton::setPosition(int x, int y, int width, int height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

bool TButton::press(int x, int y) 
{
	_pressed = hit(x, y);
	_out = false;

	return _pressed;
}

bool TButton::move(int x, int y) 
{
	bool in = hit(x, y);

	if(in && _out) {
		_pressed = true;
		_out = false;
	} else if(!in && _pressed) {
		_pressed = false;
		_out = true;
	}

	return in;
}

bool TButton::release(int x, int y) 
{
	bool in = hit(x, y);

	if(in && _pressed) {
		_action->trigger();
	}

	_pressed = false;
	_out = false;
	
	return in;
}

void TButton::draw(QPainter &painter, QPen &pen, QPen &disabled, QBrush &brush, QBrush &pressed)
{
	if(!_action->isVisible()) {
		return;
	}

	QPen oldPen = painter.pen();
	QBrush oldBrush = painter.brush();

	if(_pressed) {
		painter.setBrush(pressed);
	} else {
		painter.setBrush(brush);
	}

	if(_action->isEnabled()) {
		painter.setPen(pen);
	} else {
		painter.setPen(disabled);
	}

	_painter.draw(painter, _x, _y, _width, _height);

	painter.setBrush(oldBrush);
	painter.setPen(oldPen);
}

bool TButton::hit(int x, int y)
{
	return (x >= _x) && (x < _x + _width) && (y >= _y) && (y < _y + _height) && _action->isEnabled();
}

