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

#include "include/mapdrawlist.h"

TMapDrawList::TMapDrawList()
{
	_drawState = NULL;
}

TMapDrawList::~TMapDrawList()
{
}

void TMapDrawList::setDrawState(TDrawState *drawState)
{
	_drawState = drawState;
}

void TMapDrawList::draw(QPainter &painter, int width, int height)
{
	if(_drawState != NULL) {
		_drawState->setSize(width, height);

		int i;
		for(i = 0; i < size(); i++) {
			at(i)->draw(painter, *_drawState);
		}

		_drawState->clearFlag();
	}
}


