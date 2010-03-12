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

#include <QMessageBox>
#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <QDesktopWidget>

#include "include/nerongps.h"
#include "include/cacheform.h"
#include "include/gpxform.h"
#include "include/journeyform.h"
#include "include/logform.h"
#include "include/serverform.h"
#include "include/zoomform.h"
#include "include/magnificateform.h"
#include "include/poiform.h"
#include "include/global.h"
#include "include/keyboard.h"
#include "include/converter.h"
#include "include/gpssample.h"
#include "include/clockform.h"

TNeronGPS::TNeronGPS(QWidget *parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	qDebug("In TNeronGPS()");

	showMaximized();
	setObjectName("NeronGPS");
	setWindowTitle(tr("NeronGPS", "application header"));

/*
	QString title = windowTitle();
	setWindowTitle(QLatin1String("_allow_on_top_"));
	setWindowState(windowState() ^ Qt::WindowFullScreen);
	setWindowTitle(title);
*/

	_platform.configure(this);
	_displayAlwaysOn = false;
	_platform.displayAlwaysOn(false);

        _settings.configure(_platform.rootDir());

	_server.configure(_settings, "tileserver", "caches");
	_mapTraces.configure(_settings, "traces");
	_recorder.configure(_settings, "recorder");
	_gpxLoader.configure(_settings, "traces");
	_poi.configure(_settings, "poi");
	_mapCentering.configure(_settings, "automode");
	_mapCross.configure(_settings, "map");
	_mapCursor.configure(_settings, "cursor");
	_mapTarget.configure(_settings, "target");
	_mapTrailer.configure(_settings, "trailer");
	_gpsSource.configure(_settings, "gps");
	_gpsData.configure(_settings, "gps");
	_gpsStats.configure(_settings, "gps");
	_drawState.configure(_settings, "map");
	_actions.configure(_settings, "actions");
	_keyboard = TKeyboard::getKeyboards(_settings, "keyboard");

	if(_platform.gpsSource() != NULL) {
		_gpsSource.addSource(_platform.gpsSource());
	}

	QMenu *contextMenu = _platform.menu();

	_others.setTitle("More...");
	_actions.populateAlternateMenu(_others);
	contextMenu->addMenu(&_others);
	contextMenu->addSeparator();
	_actions.populateMainMenu(*contextMenu);

	_actions.connectTrigger("minus", &_drawState, SLOT(slotZoomMinus()));
	_actions.connectTrigger("plus", &_drawState, SLOT(slotZoomPlus()));
	_actions.connectTrigger("center", &_drawState, SLOT(slotAutoOn()));
	_actions.connectChange("center", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("startbatch", &_drawState, SLOT(slotTriggerBatchLoading()));
	_actions.connectChange("startbatch", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("stopbatch", &_batch, SLOT(slotStopBatchLoading()));
	_actions.connectChange("stopbatch", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("canceldrive", &_drawState, SLOT(slotCancelDriveTo()));

	_actions.slotChangeState("center", false, false);
	_actions.slotChangeState("startbatch", false, false);
	_actions.slotChangeState("stopbatch", false, false);
	_actions.slotChangeState("canceldrive", false, false);

	_actions.connectTrigger("clock", this, SLOT(openClock()));
	_actions.connectTrigger("caches", this, SLOT(openCache()));
	_actions.connectTrigger("traces", this, SLOT(openGpx()));
	_actions.connectTrigger("journey", this, SLOT(openJourney()));
	_actions.connectTrigger("log", this, SLOT(openUserLog()));
	_actions.connectTrigger("server", this, SLOT(openServer()));
	_actions.connectTrigger("zoom", this, SLOT(openZoom()));
	_actions.connectTrigger("magnification", this, SLOT(openMagnification()));
	_actions.connectTrigger("poi", this, SLOT(openPoi()));

	connect(&_drawState, SIGNAL(signalActionState(const QString &, bool, bool)), &_actions, SLOT(slotChangeState(const QString &, bool, bool)));
	connect(&_batch, SIGNAL(signalActionState(const QString &, bool, bool)), &_actions, SLOT(slotChangeState(const QString &, bool, bool)));
	connect(&_gpxLoader, SIGNAL(signalLoaded(QList<TTrace *> *)), &_mapTraces, SLOT(slotNewTraces(QList<TTrace *> *)));

	_batch.setServer(&_server);
	connect(&_drawState, SIGNAL(signalBatchLoading(int, int, int, int, int)), &_batch, SLOT(slotStartBatchLoading(int, int, int, int, int)));

	_actions.connectToggle("displayon", false, this, SLOT(slotDisplayAlwaysOn(bool)));
	_actions.connectToggle("record", _recorder.isRecording(), &_recorder, SLOT(slotRecord(bool)));
	_actions.connectToggle("trailer", true, &_mapTrailer, SLOT(slotEnable(bool)));

	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_gpsState, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_gpsData, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_gpsClock, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_mapTrailer, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_recorder, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), &_gpsStats, SLOT(slotGpsSample(TGpsSample)));
	connect(&_gpsState, SIGNAL(signalGpsState(bool)), &_drawState, SLOT(slotGpsState(bool)));
	connect(&_gpsState, SIGNAL(signalGpsState(bool)), &_recorder, SLOT(slotGpsState(bool)));
	connect(&_gpsData, SIGNAL(signalGpsData(bool, int, int, qreal)), &_drawState, SLOT(slotGpsData(bool, int, int, qreal)));
	_gpsSource.start();

	connect(&_mapTraces, SIGNAL(signalTraceLoaded(int, int, int, int)), &_drawState, SLOT(slotCenterTo(int, int, int, int)));
	connect(&TGlobal::messageBoard(), SIGNAL(signalRefresh()), &_drawState, SLOT(slotRefresh()));

	_map.setServer(&_server);
	connect(&_map, SIGNAL(sendUpdate()), &_drawState, SLOT(slotRefresh()));

	_drawList.setDrawState(&_drawState);
	_drawList += &_mapCentering;
	_drawList += &_map;
	_drawList += &_mapTraces;
	_drawList += &_mapTrailer;
	_drawList += &_mapCross;
	_drawList += &_mapTarget;
	_drawList += &_mapCursor;
	_mapWidget = new TMapWidget(&_drawState, &_drawList, &_actions, &TGlobal::messageBoard(), this);

	setCentralWidget(_mapWidget);

	connect(&_drawState, SIGNAL(signalUpdate()), _mapWidget, SLOT(update()));
	_mapWidget->resize(this->size());
	_mapWidget->show();
}

TNeronGPS::~TNeronGPS()
{
	delete _mapWidget;

	qDebug("In ~TNeronGPS()");
}

void TNeronGPS::closeEvent(QCloseEvent *event)
{
	QMessageBox dialog(QMessageBox::Question, "Closing", "Exit?", QMessageBox::Yes | QMessageBox::No);
	if(dialog.exec() == QMessageBox::Yes) {
		event->accept();
	} else {
		event->ignore();
	}
}

void TNeronGPS::slotDisplayAlwaysOn(bool alwaysOn)
{
	_displayAlwaysOn = alwaysOn;

	_platform.displayAlwaysOn(alwaysOn);
}

void TNeronGPS::openClock()
{
	TClockForm *clockForm = new TClockForm();

	connect(&_gpsClock, SIGNAL(signalClock(QDateTime)), clockForm, SLOT(slotClock(QDateTime)));

	clockForm->setWindowState(Qt::WindowMaximized);
	clockForm->show();
}

void TNeronGPS::openCache()
{
	TCacheForm *cacheForm = new TCacheForm(_server.getCacheDir(), _server.getCacheList(), _server.getServerList(), _batch.getBatchLoadingCache(), _batch.zoomStart(), _batch.zoomStop());

	connect(cacheForm, SIGNAL(signalNewLoadingCache(QString, int, int, bool, QStringList)), &_batch, SLOT(slotNewLoadingCache(QString, int, int, bool, QStringList)));
	connect(cacheForm, SIGNAL(signalAddCache()), &_server, SLOT(slotAddCache()));
	connect(cacheForm, SIGNAL(signalDeleteCache(QString)), &_server, SLOT(slotDeleteCache(QString)));
	connect(&_server, SIGNAL(signalNewCacheList(QStringList)), cacheForm, SLOT(slotNewCacheList(QStringList)));

	cacheForm->setWindowState(Qt::WindowMaximized);
	cacheForm->show();
}

void TNeronGPS::openGpx()
{
	TGpxForm *gpxForm = new TGpxForm(_recorder.dir());

	connect(gpxForm, SIGNAL(signalGpx(QString, QColor)), &_gpxLoader, SLOT(slotLoad(QString, QColor)));
	connect(gpxForm, SIGNAL(signalClear()), &_mapTraces, SLOT(slotClear()));

	gpxForm->setWindowState(Qt::WindowMaximized);
	gpxForm->show();
}

void TNeronGPS::openJourney()
{
	TJourneyForm *journeyForm = new TJourneyForm(_recorder.samples(), _keyboard);

	connect(&_gpsStats, SIGNAL(signalStatistics(int, int, float, int, int, int, int)), journeyForm, SLOT(slotNewStat(int, int, float, int, int, int, int)));
	connect(journeyForm, SIGNAL(signalReset()), &_gpsStats, SLOT(slotReset()));
	connect(journeyForm, SIGNAL(signalReset()), &_mapTrailer, SLOT(slotReset()));
	connect(journeyForm, SIGNAL(signalTrack(QString)), &_recorder, SLOT(slotNewTrack(QString)));
	connect(journeyForm, SIGNAL(signalWayPoint(QString)), &_recorder, SLOT(slotNewWayPoint(QString)));
	connect(&_recorder, SIGNAL(signalRecordInfo(QString, int)), journeyForm, SLOT(slotRecordInfo(QString, int)));
	connect(&_gpsSource, SIGNAL(signalUpdate(TGpsSample)), journeyForm, SLOT(slotGpsSample(TGpsSample)));

	journeyForm->setWindowState(Qt::WindowMaximized);
	journeyForm->show();

	_gpsStats.resend();
}

void TNeronGPS::openUserLog()
{
	TLogForm *logForm = new TLogForm(TGlobal::userLog());

	logForm->setWindowState(Qt::WindowMaximized);
	logForm->show();
}

void TNeronGPS::openServer()
{
	TServerForm *serverForm = new TServerForm(_server.getServerList(), _drawState.httpName());

	connect(serverForm, SIGNAL(signalNewServer(const QString &)), &_drawState, SLOT(slotSwitchHttpServer(const QString &)));

	serverForm->setWindowState(Qt::WindowMaximized);
	serverForm->show();
}

void TNeronGPS::openZoom()
{
	TZoomForm *zoomForm = new TZoomForm(_drawState.zoom());

	connect(zoomForm, SIGNAL(signalZoom(int)), &_drawState, SLOT(slotSetZoom(int)));

	zoomForm->setWindowState(Qt::WindowMaximized);
	zoomForm->show();
}

void TNeronGPS::openMagnification()
{
	TMagnificateForm *magnificationForm = new TMagnificateForm(_drawState.magnificationParam());

	connect(magnificationForm, SIGNAL(signalMagnification(int)), &_drawState, SLOT(slotSetMagnification(int)));

	magnificationForm->setWindowState(Qt::WindowMaximized);
	magnificationForm->show();
}

void TNeronGPS::openPoi()
{
	int x, y;
	_drawState.getPosition(x, y);

	TPoiForm *poiForm = new TPoiForm(_poi.poiList(), x, y, _keyboard);

	connect(poiForm, SIGNAL(signalPoi(QString, QString)), &_poi, SLOT(slotRegisterPoi(QString, QString)));
	connect(poiForm, SIGNAL(signalDeletePoi(QString)), &_poi, SLOT(slotDeletePoi(QString)));
	connect(poiForm, SIGNAL(signalGoTo(int, int)), &_drawState, SLOT(slotGoTo(int, int)));
	connect(poiForm, SIGNAL(signalDriveTo(int, int)), &_drawState, SLOT(slotDriveTo(int, int)));

	poiForm->setWindowState(Qt::WindowMaximized);
	poiForm->show();
}

