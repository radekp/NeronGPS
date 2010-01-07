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

#include "include/messagefeeder.h"

TMessageFeeder::TMessageFeeder(const TMessageFeeder &feeder) : TTextFeeder(feeder)
{
	_board = feeder._board;
	_id = feeder._id;
	_timeout = feeder._timeout;
}

TMessageFeeder::TMessageFeeder(TMessageBoard &board, void *id, int timeout)
{
	_board = &board;
	_id = id;
	_timeout = timeout;
}

TMessageFeeder::~TMessageFeeder()
{
	_board->message(content(), _id, _timeout);
}

