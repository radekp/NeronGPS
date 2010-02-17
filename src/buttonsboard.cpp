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

#include "include/buttonsboard.h"

TButtonsBoard::TButtonsBoard()
{
	_width = 100;
	_height = 100;
	_thickness = 3;
}

TButtonsBoard::~TButtonsBoard()
{
	while(_buttons.size() != 0) {
		delete _buttons.takeLast();
	}
}

void TButtonsBoard::configure(TSettings &settings, const QString &section)
{
	int thickness;

	settings.beginGroup(section);

	_width = settings.getValue("buttonwidth", 100).toInt();
	_height = settings.getValue("buttonheight", 100).toInt();

	thickness = settings.getValue("buttonthickness", 5).toInt();
	_pen.setWidth(thickness);
	_disabledPen.setWidth(thickness);

	_pen.setColor(settings.getColor("buttonenabledcolor", "ffff00c0"));
	_disabledPen.setColor(settings.getColor("buttondisabledcolor", "808080c0"));

	_brush.setColor(settings.getColor("buttonbackgroundcolor", "00000080"));
	_pressedBrush.setColor(settings.getColor("buttonpressedcolor", "000000ff"));
	_brush.setStyle(Qt::SolidPattern);
	_pressedBrush.setStyle(Qt::SolidPattern);

	settings.endGroup();
}

void TButtonsBoard::addButton(QAction *action, const QString &drawString, int xHint, int yHint)
{
	_buttons.append(new TButton(action, drawString, xHint, yHint));
}

bool TButtonsBoard::press(int x, int y) 
{
	bool hit = false;

	int i;
	for(i = 0; i < _buttons.size(); i++) {
		hit = hit || _buttons[i]->press(x, y);
	}

	return hit;
}

bool TButtonsBoard::move(int x, int y) 
{
	bool hit = false;

	int i;
	for(i = 0; i < _buttons.size(); i++) {
		hit = hit || _buttons[i]->move(x, y);
	}

	return hit;
}

bool TButtonsBoard::release(int x, int y) 
{
	bool hit = false;

	int i;
	for(i = 0; i < _buttons.size(); i++) {
		hit = hit || _buttons[i]->release(x, y);
	}

	return hit;
}

void TButtonsBoard::draw(QPainter &painter, TDrawState &drawState)
{
	if(!drawState.moving()) {
		int i;
		for(i = 0; i < _buttons.size(); i++) {
			int x;
			int y;

			if(_buttons[i]->xHint() < 0) {
				x = drawState.width() + _buttons[i]->xHint() * _width;
			} else {
				x = _buttons[i]->xHint() * _width;
			}

			if(_buttons[i]->yHint() < 0) {
				y = drawState.height() + _buttons[i]->yHint() * _height;
			} else {
				y = _buttons[i]->yHint() * _height;
			}

			_buttons[i]->setPosition(x, y, _width, _height);
		}

		for(i = 0; i < _buttons.size(); i++) {
			_buttons[i]->draw(painter, _pen, _disabledPen, _brush, _pressedBrush);
		}
	}
}

