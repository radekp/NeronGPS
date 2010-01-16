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

#ifdef Q_WS_QWS
#include <QAction>
#include <QMenu>
#include <QtopiaApplication>
#else
#include <QApplication>
#endif

#include <QMessageBox>
#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <QDesktopWidget>
#include <QWhereaboutsFactory>
#include <QNmeaWhereabouts>
#include <QSoftMenuBar>
#include <QTimeZone>
#include <QtopiaServiceRequest>

#include "include/appli.h"
#include "include/global.h"
#include "include/keyboard.h"
#include "include/converter.h"

TGpsAppli::TGpsAppli(QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	qDebug("In TGpsAppli()");

#ifdef Q_WS_QWS
	setObjectName("NeronGPS");
	QtopiaApplication::setInputMethodHint(this, QtopiaApplication::AlwaysOff);
	setWindowTitle(tr("NeronGPS", "application header"));
#endif

	showMaximized();

	_server.configure(_settings, "tileserver", "caches");
	_mapTraces.configure(_settings, "traces");
	_recorder.configure(_settings, "recorder");
	_gpxLoader.configure(_settings, "traces");
	_poi.configure(_settings, "poi");
	_buttons.configure(_settings, "buttons");
	_mapCentering.configure(_settings, "automode");
	_mapCross.configure(_settings, "map");
	_mapCursor.configure(_settings, "cursor");
	_mapTarget.configure(_settings, "target");
	_mapTrailer.configure(_settings, "trailer");
	_gpsData.configure(_settings, "gps");
	_gpsStats.configure(_settings, "gps");
	_drawState.configure(_settings, "map");
	_keyboard = TKeyboard::getKeyboards(_settings, "keyboard");

	QMenu *contextMenu = QSoftMenuBar::menuFor(this);

	contextMenu->addSeparator ();
	_others.setTitle("More...");
	contextMenu->addMenu(&_others);

	QStringList actionsList;

	actionsList.append("zoom_minus/Zoom -/button/0/0/ell2(0,0,100,100)/line(25,50,75,50)");
	actionsList.append("zoom_plus/Zoom +/button/1/0/ell2(0,0,100,100)/line(25,50,75,50)/line(50,25,50,75)");
	actionsList.append("auto_center/Auto center/button/-1/0/ell2(0,0,100,100)/ell1(40,40,20,20)/ell1(45,45,10,10)");
	actionsList.append("start_batch/Start batch/button/2/0/ell2(0,0,100,100)/line(25,60,75,60)/line(30,50,70,50)/line(35,40,65,40)");
	actionsList.append("stop_batch/Stop batch/button/2/0/ell2(0,0,100,100)/line(25,25,75,75)/line(25,75,75,25)");
	actionsList.append("server/Server/main/0");
	actionsList.append("zoom/Zoom/main/1");
	actionsList.append("magnification/Magnification/main/2");
	actionsList.append("journey/Journey/main/3");
	actionsList.append("poi/POI/main/4");
	actionsList.append("canceldriveto/Cancel drive to/main/5");
	actionsList.append("traces/Traces/more/0");
	actionsList.append("clock/Clock/more/1");
	actionsList.append("cache/Cache/more/2");
	actionsList.append("user_log/User log/more/3");
	actionsList.append("display_always_on/Display Always On/more/4");
	actionsList.append("record/Record/more/5");
	actionsList.append("display_trailer/Display trailer/more/6");

	_actions.configure(_settings, "actions", actionsList, *contextMenu, _others, _buttons);

	_actions.connectTrigger("Zoom -", &_drawState, SLOT(slotZoomMinus()));
	_actions.connectTrigger("Zoom +", &_drawState, SLOT(slotZoomPlus()));
	_actions.connectTrigger("Auto center", &_drawState, SLOT(slotAutoOn()));
	_actions.connectChange("Auto center", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("Start batch", &_drawState, SLOT(slotTriggerBatchLoading()));
	_actions.connectChange("Start batch", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("Stop batch", &_batch, SLOT(slotStopBatchLoading()));
	_actions.connectChange("Stop batch", &_drawState, SLOT(slotRefresh()));
	_actions.connectTrigger("Cancel drive to", &_drawState, SLOT(slotCancelDriveTo()));

	_actions.slotChangeState("Auto center", false, false);
	_actions.slotChangeState("Start batch", false, false);
	_actions.slotChangeState("Stop batch", false, false);
	_actions.slotChangeState("Cancel drive to", false, false);

	_actions.connectTrigger("Clock", this, SLOT(openClock()));
	_actions.connectTrigger("Cache", this, SLOT(openCache()));
	_actions.connectTrigger("Traces", this, SLOT(openGpx()));
	_actions.connectTrigger("Journey", this, SLOT(openJourney()));
	_actions.connectTrigger("User log", this, SLOT(openUserLog()));
	_actions.connectTrigger("Server", this, SLOT(openServer()));
	_actions.connectTrigger("Zoom", this, SLOT(openZoom()));
	_actions.connectTrigger("Magnification", this, SLOT(openMagnification()));
	_actions.connectTrigger("POI", this, SLOT(openPoi()));

	connect(&_drawState, SIGNAL(signalActionState(const QString &, bool, bool)), &_actions, SLOT(slotChangeState(const QString &, bool, bool)));
	connect(&_batch, SIGNAL(signalActionState(const QString &, bool, bool)), &_actions, SLOT(slotChangeState(const QString &, bool, bool)));
	connect(&_gpxLoader, SIGNAL(signalLoaded(QList<TTrace *> *)), &_mapTraces, SLOT(slotNewTraces(QList<TTrace *> *)));

	_batch.setServer(&_server);
	connect(&_drawState, SIGNAL(signalBatchLoading(int, int, int, int, int)), &_batch, SLOT(slotStartBatchLoading(int, int, int, int, int)));

	_actions.connectToggle("Display Always On", _drawState.displayAlwaysOn(), &_drawState, SLOT(slotDisplayAlwaysOn(bool)));
	_actions.connectToggle("Record", _recorder.isRecording(), &_recorder, SLOT(slotRecord(bool)));
	_actions.connectToggle("Display trailer", true, &_mapTrailer, SLOT(slotEnable(bool)));

/*
        QFile *sampleFile = new QFile("/home/root/nmea_sample.txt", this);
        sampleFile->open(QIODevice::ReadOnly);
        QNmeaWhereabouts *whereabouts = new QNmeaWhereabouts(this);
        whereabouts->setUpdateMode(QNmeaWhereabouts::SimulationMode);
        whereabouts->setSourceDevice(sampleFile);
	_location = whereabouts;
*/

	_location = QWhereaboutsFactory::create();
	connect(_location, SIGNAL(stateChanged(QWhereabouts::State)), &_gpsState, SLOT(slotGpsState(QWhereabouts::State)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_gpsState, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_gpsData, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_gpsClock, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_mapTrailer, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_recorder, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), &_gpsStats, SLOT(slotGpsData(const QWhereaboutsUpdate &)));
	connect(&_gpsState, SIGNAL(signalGpsState(bool)), &_drawState, SLOT(slotGpsState(bool)));
	connect(&_gpsState, SIGNAL(signalGpsState(bool)), &_recorder, SLOT(slotGpsState(bool)));
	connect(&_gpsData, SIGNAL(signalGpsData(bool, int, int, qreal)), &_drawState, SLOT(slotGpsData(bool, int, int, qreal)));
	_location->startUpdates(1000); 

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
	_mapWidget = new TMapWidget(&_drawState, &_drawList, &_buttons, &TGlobal::messageBoard(), this);

	connect(&_drawState, SIGNAL(signalUpdate()), _mapWidget, SLOT(update()));
	_mapWidget->resize(this->size());
	_mapWidget->show();
}

TGpsAppli::~TGpsAppli()
{
	delete _mapWidget;

	qDebug("In ~TGpsAppli()");
}

void TGpsAppli::closeEvent(QCloseEvent *event)
{
	QMessageBox dialog(QMessageBox::Question, "Closing", "Exit?", QMessageBox::Yes | QMessageBox::No);
	if(dialog.exec() == QMessageBox::Yes) {
		event->accept();
	} else {
		event->ignore();
	}
}

void TGpsAppli::openClock()
{
	TClockForm *clockForm = new TClockForm();

	connect(&_gpsClock, SIGNAL(signalClock(QDateTime)), clockForm, SLOT(slotClock(QDateTime)));
	connect(&_gpsClock, SIGNAL(signalTimeZone(QTimeZone)), clockForm, SLOT(slotTimeZone(QTimeZone)));
	connect(clockForm, SIGNAL(signalSync()), &_gpsClock, SLOT(slotSync()));

	clockForm->setWindowState(Qt::WindowMaximized);
	clockForm->show();

	_gpsClock.resend();
}

void TGpsAppli::openCache()
{
	TCacheForm *cacheForm = new TCacheForm(_server.getCacheDir(), _server.getCacheList(), _server.getServerList(), _batch.getBatchLoadingCache(), _batch.zoomStart(), _batch.zoomStop());

	connect(cacheForm, SIGNAL(signalNewLoadingCache(QString, int, int, bool, QStringList)), &_batch, SLOT(slotNewLoadingCache(QString, int, int, bool, QStringList)));
	connect(cacheForm, SIGNAL(signalAddCache()), &_server, SLOT(slotAddCache()));
	connect(cacheForm, SIGNAL(signalDeleteCache(QString)), &_server, SLOT(slotDeleteCache(QString)));
	connect(&_server, SIGNAL(signalNewCacheList(QStringList)), cacheForm, SLOT(slotNewCacheList(QStringList)));

	cacheForm->setWindowState(Qt::WindowMaximized);
	cacheForm->show();
}

void TGpsAppli::openGpx()
{
	TGpxForm *gpxForm = new TGpxForm(_recorder.dir(), QStringList());

	connect(gpxForm, SIGNAL(signalGpx(QString, QColor)), &_gpxLoader, SLOT(slotLoad(QString, QColor)));
	connect(gpxForm, SIGNAL(signalClear()), &_mapTraces, SLOT(slotClear()));

	gpxForm->setWindowState(Qt::WindowMaximized);
	gpxForm->show();
}

void TGpsAppli::openJourney()
{
	TJourneyForm *journeyForm = new TJourneyForm(_recorder.samples(), _keyboard);

	connect(&_gpsStats, SIGNAL(signalStatistics(int, int, float, int, int, int, int)), journeyForm, SLOT(slotNewStat(int, int, float, int, int, int, int)));
	connect(journeyForm, SIGNAL(signalReset()), &_gpsStats, SLOT(slotReset()));
	connect(journeyForm, SIGNAL(signalReset()), &_mapTrailer, SLOT(slotReset()));
	connect(journeyForm, SIGNAL(signalTrack(QString)), &_recorder, SLOT(slotNewTrack(QString)));
	connect(journeyForm, SIGNAL(signalWayPoint(QString)), &_recorder, SLOT(slotNewWayPoint(QString)));
	connect(&_recorder, SIGNAL(signalRecordInfo(QString, int)), journeyForm, SLOT(slotRecordInfo(QString, int)));
	connect(_location, SIGNAL(updated(const QWhereaboutsUpdate &)), journeyForm, SLOT(slotGpsData(const QWhereaboutsUpdate &)));

	journeyForm->setWindowState(Qt::WindowMaximized);
	journeyForm->show();

	_gpsStats.resend();
}

void TGpsAppli::openUserLog()
{
	TLogForm *logForm = new TLogForm(TGlobal::userLog());

	logForm->setWindowState(Qt::WindowMaximized);
	logForm->show();
}

void TGpsAppli::openServer()
{
	TServerForm *serverForm = new TServerForm(_server.getServerList(), _drawState.httpName());

	connect(serverForm, SIGNAL(signalNewServer(const QString &)), &_drawState, SLOT(slotSwitchHttpServer(const QString &)));

	serverForm->setWindowState(Qt::WindowMaximized);
	serverForm->show();
}

void TGpsAppli::openZoom()
{
	TZoomForm *zoomForm = new TZoomForm(_drawState.zoom());

	connect(zoomForm, SIGNAL(signalZoom(int)), &_drawState, SLOT(slotSetZoom(int)));

	zoomForm->setWindowState(Qt::WindowMaximized);
	zoomForm->show();
}

void TGpsAppli::openMagnification()
{
	TMagnificateForm *magnificationForm = new TMagnificateForm(_drawState.magnificationParam());

	connect(magnificationForm, SIGNAL(signalMagnification(int)), &_drawState, SLOT(slotSetMagnification(int)));

	magnificationForm->setWindowState(Qt::WindowMaximized);
	magnificationForm->show();
}

void TGpsAppli::openPoi()
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

