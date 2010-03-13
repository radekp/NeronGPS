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

#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QPainter>
#include <QAction>

#include "include/settings.h"
#include "include/painter.h"

class TAction : public QAction
{
	Q_OBJECT
	public:
		TAction();
		~TAction();

		void configure(TSettings &settings, const QString &section, const QString &name);
		void draw(QPainter &painter, int x, int y, int w, int h);

		const QString &name() { return _name; }
		bool noDisplay() { return _noDisplay; }

	private:
		QString _name;
		TPainter _painter;
		bool _noDisplay;
		bool _configured;
};

#endif

