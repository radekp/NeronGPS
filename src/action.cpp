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
#include <QPainter>

#include "include/action.h"

TAction::TAction() : QAction(NULL)
{
	_noDisplay = true;
}

TAction::~TAction()
{
}

void TAction::configure(TSettings &settings, const QString &section, const QString &name)
{
	QString displayName = name;
	QString paintString = QString("");

	if(name == QString("minus")) {
		displayName = QString("Zoom -");
		paintString = QString("line(25,50,75,50)");
		_noDisplay = false;
	} else if(name == QString("plus")) {
		displayName = QString("Zoom +");
		paintString = QString("line(25,50,75,50)/line(50,25,50,75)");
		_noDisplay = false;
	} else if(name == QString("center")) {
		displayName = QString("Auto center");
		paintString = QString("ell1(30,30,40,40)");
		_noDisplay = false;
	} else if(name == QString("fullscreen")) {
		displayName = QString("Fullscreen");
	} else if(name == QString("server")) {
		displayName = QString("Map server");
	} else if(name == QString("zoom")) {
		displayName = QString("Zoom");
	} else if(name == QString("magnification")) {
		displayName = QString("Magnification");
	} else if(name == QString("journey")) {
		displayName = QString("Journey");
	} else if(name == QString("poi")) {
		displayName = QString("POI");
	} else if(name == QString("canceldrive")) {
		displayName = QString("Cancel Drive To");
	} else if(name == QString("canceldrive")) {
		displayName = QString("Cancel Drive To");
	} else if(name == QString("startbatch")) {
		displayName = QString("Start batch");
	} else if(name == QString("stopbatch")) {
		displayName = QString("Stop batch");
	} else if(name == QString("traces")) {
		displayName = QString("GPX traces");
	} else if(name == QString("clock")) {
		displayName = QString("GPS clock");
	} else if(name == QString("caches")) {
		displayName = QString("Map caches");
	} else if(name == QString("log")) {
		displayName = QString("User log");
	} else if(name == QString("displayon")) {
		displayName = QString("Display always on");
	} else if(name == QString("record")) {
		displayName = QString("Record");
	} else if(name == QString("trailer")) {
		displayName = QString("Display trailer");
	}

	QStringList list;
	list << displayName;
	list << paintString;

	settings.beginGroup(section);

	list = settings.getValue(name, list).toStringList();

	settings.endGroup();

	_name = name;

	if(list.size() > 0) {
		QAction::setText(list[0]);
	} else {
		QAction::setText(displayName);
	}

	if(list.size() > 1) {
		paintString = list[1];
	}

	if(!paintString.isEmpty()) {
		_painter.configure(paintString);
	}
}

void TAction::draw(QPainter &painter, int x, int y, int w, int h)
{
	_painter.draw(painter, x, y, w, h);
}

