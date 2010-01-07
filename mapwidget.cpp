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
#include <QPainter>
#include <QFontMetrics>
#include <QDragMoveEvent>

#include "include/mapwidget.h"
#include "include/tile.h"
#include "include/converter.h"

TMapWidget::TMapWidget(TDrawState *drawState, TMapDrawList *drawList, TButtonsBoard *buttons, TMessageBoard *messages, QWidget *parent) : QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	_drawState = drawState;
	_drawList = drawList;
	_buttons = buttons;
	_messages = messages;

	update();
}

void TMapWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	if(!_drawState->moving()) {
		painter.setRenderHint(QPainter::Antialiasing);
	}

	_drawList->draw(painter, width(), height());
	_buttons->draw(painter, *_drawState);
	_messages->draw(painter, *_drawState);
}

void TMapWidget::mousePressEvent(QMouseEvent *event)
{
	if(!_buttons->press(event->x(), event->y())) {
		_drawState->slotMoving(true);
		_mouseX = event->x();
		_mouseY = event->y();
		_drawState->slotAutoOff();
	}
}

void TMapWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(_drawState->moving()) {
		int offsetX = TConverter::convertBack(_mouseX - event->x(), _drawState->zoom());
		int offsetY = TConverter::convertBack(_mouseY - event->y(), _drawState->zoom());
		_drawState->slotMove(offsetX, offsetY);

		_mouseX = event->x();
		_mouseY = event->y();
	} else {
		_buttons->move(event->x(), event->y());
		update();
	}
}

void TMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (_drawState->moving()) {
		_drawState->slotMoving(false);
		int offsetX = TConverter::convertBack(_mouseX - event->x(), _drawState->zoom());
		int offsetY = TConverter::convertBack(_mouseY - event->y(), _drawState->zoom());
		_drawState->slotMove(offsetX, offsetY);
	} else {
		_buttons->release(event->x(), event->y());
		update();
	}
}

