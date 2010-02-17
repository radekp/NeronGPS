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

#include "include/settings.h"

TSettings::TSettings() : QSettings("NeronGPS", "NeronGPS")
{
}

TSettings::~TSettings()
{
}

QVariant TSettings::getValue(const QString &key, const QVariant &defaultValue)
{
	QVariant ret = value(key, "????");
	
	if(ret.toString() == "????") {
		setValue(key, defaultValue);
		ret = defaultValue;
	}

	return ret;
}

QColor TSettings::getColor(const QString &key, const QString &defaultValue)
{
	QVariant val = value(key, "????");
	
	if(val.toString() == "????") {
		setValue(key, defaultValue);
		val = defaultValue;
	}

	QColor color;
	ulong rgba = val.toString().toUInt(0, 16);

	color.setRgb((rgba >> 24) & 0xff, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff);

	return color;
}

