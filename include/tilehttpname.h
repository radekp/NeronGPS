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

#ifndef TILEHTTPNAME_H
#define TILEHTTPNAME_H

#include <QStringList>

#include "include/settings.h"

class TTileHttpName
{
	public:
		TTileHttpName();
		~TTileHttpName();

		static QString defaultServer();
		static QString initDefault(TSettings &settings);

		static QString createName(const QString &settingName, const QString &type);
		static QString createComposerName(const QString &settingName);

		static bool isComposer(const QString &name);

		static QString convertToDisplayName(const QString &name);		
		static QString convertFromDisplayName(const QString &name);		

		static QString convertToCacheName(const QString &name);		
		static QString convertFromCacheName(const QString &name);		
};

#endif

