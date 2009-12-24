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

#ifndef MAPDRAWLIST_H
#define MAPDRAWLIST_H

#include <QObject>
#include <QList>
#include <QPainter>

#include "include/drawstate.h"

class TMapDrawListElement : public QObject
{
	Q_OBJECT
	public:
		virtual ~TMapDrawListElement() {};
		virtual void draw(QPainter &painter, TDrawState &drawState) = 0;
};


class TMapDrawList : public QList<TMapDrawListElement *>
{
	public:
		TMapDrawList();
		~TMapDrawList();
		
		void setDrawState(TDrawState *drawState);
		void draw(QPainter &painter, int width, int height); 

	private:
		TDrawState *_drawState;
};

#endif

