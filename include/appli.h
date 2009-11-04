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

#ifndef APPLI_H
#define APPLI_H

#include <QMainWindow>
#include <QMenu>

#include "include/clockform.h"
#include "include/cacheform.h"
#include "include/traceform.h"
#include "include/journeyform.h"
#include "include/logform.h"
#include "include/serverform.h"
#include "include/zoomform.h"
#include "include/magnificateform.h"
#include "include/tileserver.h"
#include "include/tilebatchloader.h"
#include "include/mapwidget.h"
#include "include/gpsstate.h"
#include "include/gpsdata.h"
#include "include/gpsclock.h"
#include "include/traceserver.h"
#include "include/actionsmanager.h"
#include "include/buttonsboard.h"
#include "include/messageboard.h"
#include "include/drawstate.h"
#include "include/settings.h"
#include "include/gpsstatistics.h"

class TGpsAppli : public QWidget
{
	Q_OBJECT

	public:
		TGpsAppli(QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TGpsAppli();

	public slots:
		void openClock();
		void openCache();
		void openTrace();
		void openJourney();
		void openUserLog();
		void openServer();
		void openZoom();
		void openMagnification();

	private:
		QMenu _others;

		QWhereabouts *_location;
		TGpsState _gpsState;
		TGpsData _gpsData;
		TGpsClock _gpsClock;
		TGpsStatistics _gpsStats;

		TSettings _settings;
		TTileServer _server;
		TTileBatchLoader _batch;
		TDrawState _drawState;
		TLocationPointer _pointer;
		TActionsManager _actions;
		TButtonsBoard _buttons;
		TTraceServer _traces;

		TMapWidget *_mapWidget;
};

#endif
