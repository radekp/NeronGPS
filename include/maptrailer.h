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

#ifndef MAPTRAILER_H
#define MAPTRAILER_H

#include <QObject>
#include <QList>
#include <QPainterPath>
#include <QMutex>
#include <QWhereaboutsUpdate>

#include "include/trace.h"
#include "include/drawstate.h"
#include "include/mapdrawlist.h"

class TMapTrailer : public TMapDrawListElement
{
	Q_OBJECT
	public:
		TMapTrailer();
		~TMapTrailer();

		void configure(TSettings &settings, const QString &section);
		void draw(QPainter &painter, TDrawState &drawState);

	public slots:
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotEnable(bool enable);
		void slotReset();

	private:
		TTrace _trailer;
		bool _enable;
};

#endif

