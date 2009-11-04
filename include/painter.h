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

#ifndef PAINTER_H
#define PAINTER_H

#include "include/painterelement.h"

class TPainter
{
	public:
		TPainter();
		~TPainter();

		void configure(const QString &drawString);

		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		QList<TPainterElement *> _elements;
};

#endif

