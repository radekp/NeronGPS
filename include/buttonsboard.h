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

#ifndef BUTTONSBOARD_H
#define BUTTONSBOARD_H

#include <QObject>
#include <QPainter>
#include <QList>
#include <QColor>

#include "include/button.h"
#include "include/drawstate.h"

class TButtonsBoard
{
	public:
		TButtonsBoard();
		~TButtonsBoard();

		void configure(TSettings &settings, const QString &section);

		void addButton(QAction *action, const QString &drawString, int xHint, int yHint);

		bool press(int x, int y);
		bool move(int x, int y);
		bool release(int x, int y);
		void draw(QPainter &painter, TDrawState &drawState);

	private:
		QList<TButton *> _buttons;
		int _width;
		int _height;
		int _thickness;

		QBrush _brush;
		QBrush _pressedBrush;
		QPen _pen;
		QPen _disabledPen;
};

#endif

