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
#include <QMessageBox>

#include "include/tilehttpname.h"

TTileHttpName::TTileHttpName()
{
}

TTileHttpName::~TTileHttpName()
{
}

QString TTileHttpName::defaultServer()
{
	return "osm_road";
}

QString TTileHttpName::initDefault(TSettings &settings)
{
	QString defaultServer("osm");

	settings.beginGroup(defaultServer);
	settings.setValue("servers", "road");
	settings.setValue("road1", "http://tile.openstreetmap.org/%3/%1/%2.png");
	settings.setValue("rootlevel", 0);
	settings.setValue("equatorcentric", false);
	settings.setValue("snailmode", false);
	settings.endGroup();

	return defaultServer;
}

QString TTileHttpName::createName(const QString &settingName, const QString &type)
{
	return settingName + "_" + type;
}

QString TTileHttpName::createComposerName(const QString &settingName)
{
	return settingName + "_comp";
}

bool TTileHttpName::isComposer(const QString &name)
{
	return name.section('_', -1, -1) == "comp";
}

QString TTileHttpName::convertToDisplayName(const QString &name)
{
	return name;
}

QString TTileHttpName::convertFromDisplayName(const QString &name)
{
	return name;
}

QString TTileHttpName::convertToCacheName(const QString &name)
{
	return name;
}

QString TTileHttpName::convertFromCacheName(const QString &name)
{
	return name;
}

