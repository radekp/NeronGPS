/*
 * Copyright 2009 Thierry Vuillaume
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

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QWhereabouts>
#include <QWhereaboutsUpdate>

#include "include/traceserver.h"
#include "include/buttonsboard.h"
#include "include/messageboard.h"
#include "include/drawstate.h"
#include "include/locationpointer.h"

class TMapWidget : public QWidget
{
	Q_OBJECT

	public:
		TMapWidget(TDrawState *drawState, TLocationPointer *pointer, TTraceServer *traces, TButtonsBoard *buttons, TMessageBoard *messages, QWidget *parent = 0);
		
	protected:
		void resizeEvent (QResizeEvent *event);
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event); 
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

	private:
		TDrawState *_drawState;
		TLocationPointer *_pointer;
		TTraceServer *_traces;
		TButtonsBoard *_buttons;
		TMessageBoard *_messages;

		int _mouseX;
		int _mouseY;
};

#endif
