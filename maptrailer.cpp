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
#include <QDir>

#include "include/maptrailer.h"
#include "include/converter.h"
#include "include/global.h"

TMapTrailer::TMapTrailer()
{
	_enable = true;
}

TMapTrailer::~TMapTrailer()
{
}

void TMapTrailer::configure(TSettings &settings, const QString &section)
{
	QPen pen;

	settings.beginGroup(section);

	pen.setWidth(settings.getValue("thickness", 10).toInt());
	pen.setCapStyle(Qt::RoundCap);
	pen.setColor(settings.getColor("color", "008000c0"));
	_trailer.setPen(pen, settings.getValue("accuracy", 200).toInt());

	settings.endGroup();
}

void TMapTrailer::slotGpsData(const QWhereaboutsUpdate &update)
{
	int x = TConverter::prepareX(update.coordinate().longitude());
	int y = TConverter::prepareY(update.coordinate().latitude());
	
	_trailer.addSample(x, y);
}

void TMapTrailer::draw(QPainter &painter, TDrawState &drawState)
{
	if(!drawState.moving() && _enable) {
		_trailer.draw(painter, drawState);
	}
}

void TMapTrailer::slotEnable(bool enable)
{
	_enable = enable;
}

void TMapTrailer::slotReset()
{
	_trailer.reset();
}



