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

#ifndef MAPTARGET_H
#define MAPTARGET_H

#include <QObject>

#include "include/drawstate.h"
#include "include/painter.h"
#include "include/mapdrawlist.h"

class TMapTarget : public TMapDrawListElement
{
	Q_OBJECT
	public:
		TMapTarget();
		~TMapTarget();
		
		void configure(TSettings &settings, const QString &section);
		void draw(QPainter &painter, TDrawState &drawState);
		
	private:
		TPainter _paint;
		int _size;
		QColor _onTrackColor;
		QColor _outOfTrackColor;

		int _shadowLevel;
		int _shadowX;
		int _shadowY;
};

#endif

