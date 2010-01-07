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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "logfeeder.h"
#include "messagefeeder.h"

class TLog;
class TMessageBoard;

class TGlobal : public QObject
{
	Q_OBJECT
	public:
		TGlobal();
		~TGlobal();

		static TLog &userLog();
		static TLog &debugLog();
		static TMessageBoard &messageBoard();
};

inline TLogFeeder tUserLog() { return TLogFeeder(TGlobal::userLog()); }
inline TLogFeeder tDebugLog() { return TLogFeeder(TGlobal::debugLog()); }
inline TMessageFeeder tMessage(void *id, int timeout = -1) { return TMessageFeeder(TGlobal::messageBoard(), id, timeout); }

#endif

