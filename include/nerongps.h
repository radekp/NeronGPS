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

#ifndef NERONGPS_H
#define NERONGPS_H

#include <QWidget>
#include <QObject>
#include <QMenu>

#include "include/platform.h"
#include "include/tileserver.h"
#include "include/tilebatchloader.h"
#include "include/mapwidget.h"
#include "include/gpsstate.h"
#include "include/gpsdata.h"
#include "include/gpsclock.h"
#include "include/poiserver.h"
#include "include/actionsmanager.h"
#include "include/buttonsboard.h"
#include "include/messageboard.h"
#include "include/drawstate.h"
#include "include/mapdrawlist.h"
#include "include/settings.h"
#include "include/gpssource.h"
#include "include/gpsstatistics.h"
#include "include/mapcentering.h"
#include "include/mapcross.h"
#include "include/mapcursor.h"
#include "include/maptarget.h"
#include "include/maptraces.h"
#include "include/maptrailer.h"
#include "include/tilemap.h"
#include "include/gpxloader.h"
#include "include/tracerecorder.h"

class TNeronGPS : public QObject
{
	Q_OBJECT

	public:
		TNeronGPS(TPlatform *platform);
		virtual ~TNeronGPS();

	public slots:
		void openClock();
		void openCache();
		void openGpx();
		void openJourney();
		void openUserLog();
		void openServer();
		void openZoom();
		void openMagnification();
		void openPoi();

	protected:
		void closeEvent(QCloseEvent *);

	private:
		TPlatform *_platform;

		QMenu _others;
		QStringList _keyboard;

		TGpsState _gpsState;
		TGpsData _gpsData;
		TGpsClock _gpsClock;
		TGpsStatistics _gpsStats;

		TSettings _settings;
		TTileServer _server;
		TTileBatchLoader _batch;
		TDrawState _drawState;

		TMapDrawList _drawList;
		TMapCentering _mapCentering;
		TTileMap _map;
		TMapCursor _mapCursor;
		TMapTarget _mapTarget;
		TMapCross _mapCross;
		TMapTrailer _mapTrailer;
		TMapTraces _mapTraces;
		TActionsManager _actions;
		TButtonsBoard _buttons;
		TPoiServer _poi;
		TGpxLoader _gpxLoader;
		TTraceRecorder _recorder;

		TMapWidget *_mapWidget;
};

#endif

