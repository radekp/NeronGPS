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

#include "include/fullscreen.h"

TFullScreen::TFullScreen(QWidget *parent) : QMainWindow(parent, Qt::WindowStaysOnTopHint)
{
	setWindowTitle(QLatin1String( "_allow_on_top_" ));
}

bool TFullScreen::event(QEvent *event)
{
	if(event->type() == QEvent::WindowDeactivate)
	{
		lower();
	} else if(event->type() == QEvent::WindowActivate) {
		raise();
	}

	return QWidget::event(event);
}

