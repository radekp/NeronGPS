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

#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QPainter>
#include <QAction>
#include <QColor>

#include "include/painter.h"

class TButton : public QObject
{
	Q_OBJECT
	public:
		TButton(QAction *action, const QString &drawString, int xHint, int yHint);
		~TButton();

		void setPosition(int x, int y, int width, int height);

		int xHint() {return _xHint; }
		int yHint() {return _yHint; }

		bool press(int x, int y);
		bool move(int x, int y);
		bool release(int x, int y);

		void draw(QPainter &painter, QPen &pen, QPen &disabled, QBrush &brush, QBrush &pressed);

	private:
		TPainter _painter;
		QAction *_action;
		int _xHint;
		int _yHint;
		int _x;
		int _y;
		int _width;
		int _height;
		bool _pressed;
		bool _out;

		bool hit(int x, int y);
};

#endif

