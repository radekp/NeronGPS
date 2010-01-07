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

#ifdef Q_WS_QWS
#include <QAction>
#include <QMenu>
#include <QtopiaApplication>
#else
#include <QApplication>
#endif

#include "include/drawstate.h"
#include "include/converter.h"
#include "include/trace.h"
#include "include/tilehttpwheel.h"
#include "include/tilehttpname.h"
#include "include/global.h"
#include "include/gpsdata.h"

TDrawState::TDrawState()
{
	_width = -1;
	_height = -1;
	_magnification = 0;
	_magnificate = true;
	_zoom = 0;
	_centerX = 0;
	_centerY = 0;
	_autoOn = true;
	_driveOn = false;
	_driveOnTrack = true;
	_validPos = false;
	_fix = false;
	_moving = false;
	_displayAlwaysOn = false;
	_currentAngle = 0;
	_flag = 0;
}

TDrawState::~TDrawState()
{
}

void TDrawState::configure(TSettings &settings, const QString &section)
{
	QString defaultServer = TTileHttpName::defaultServer();

	settings.beginGroup(section);
	_magnification = settings.getValue("magnification", 1).toInt();
	_dontMagnificate = settings.getValue("nomagnification", "").toStringList();
	_zoom = settings.getValue("defaultzoom", 0).toInt() + magnification();
	_centerX = TConverter::prepareX(settings.getValue("defaultlongitude", 5.71).toDouble());
	_centerY = TConverter::prepareY(settings.getValue("defaultlatitude", 45.234444).toDouble());
	_displayAlwaysOn = settings.getValue("displayalwayson", false).toBool();
	QString httpName = settings.getValue("defaultserver", defaultServer).toString();
	_centeringTopBorder = settings.getValue("centeringtopborder", 120).toInt();
	_centeringBottomBorder = settings.getValue("centeringbottomborder", 50).toInt();
	_centeringRightBorder = settings.getValue("centeringrightborder", 50).toInt();
	_centeringLeftBorder = settings.getValue("centeringleftborder", 50).toInt();
	settings.endGroup();

	if(_displayAlwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}

	slotSwitchHttpServer(httpName);
}

void TDrawState::setSize(int width, int height)
{
	if((width != _width) || (height != _height)) {
		_width = width;
		_height = height;
		_flag |= DRAWSTATE_FLAG_NEW_SIZE;
	}
}

void TDrawState::getPosition(int &x, int &y)
{
	if(_autoOn && _validPos) {
		x = _currentX;
		y = _currentY;
	} else {
		x = _centerX;
		y = _centerY;
	}
}

void TDrawState::slotSetZoom(int zoom)
{
	if(zoom != _zoom) {
		_zoom = zoom;
		_flag |= DRAWSTATE_FLAG_NEW_ZOOM;
		emit signalUpdate();
		tMessage((void *)this, 1000) << (_zoom - magnification());
	}
}

void TDrawState::slotZoomPlus()
{
	if(_zoom < NUMLEVELS) {
		slotSetZoom(_zoom + 1);
	}
}

void TDrawState::slotZoomMinus()
{
	if(_zoom > 0) {
		slotSetZoom(_zoom - 1);
	}
}

void TDrawState::slotSetMagnification(int magnification)
{
	if(_magnification == magnification) {
		return;
	}

	_magnification = magnification;

	if(_magnificate) {
		_flag |= DRAWSTATE_FLAG_NEW_MAGNIFICATION;
		emit signalUpdate();
	}
}

void TDrawState::slotMove(int offsetX, int offsetY)
{
	_centerX += offsetX;
	_centerY += offsetY;

	_flag |= DRAWSTATE_FLAG_NEW_POSITION;
	emit signalUpdate();
}

void TDrawState::slotGoTo(int x, int y)
{
	_centerX = x;
	_centerY = y;

	_flag |= DRAWSTATE_FLAG_NEW_POSITION;
	slotAutoOff();
}

void TDrawState::slotDriveTo(int x, int y)
{
	_driveX = x;
	_driveY = y;

	_driveOn = true;
	_driveOnTrack = true;

	_flag |= DRAWSTATE_FLAG_DRIVETO_TURNED_ON;

	emit signalActionState("Cancel drive to", true, true);
	slotAutoOn();
}

void TDrawState::slotCancelDriveTo()
{
	_driveOn = false;

	emit signalActionState("Cancel drive to", false, false);
	emit signalUpdate();
}

void TDrawState::slotAutoOn()
{
	_autoOn = true;
	_flag |= DRAWSTATE_FLAG_AUTO_TURNED_ON;
	emit signalActionState("Auto center", false, false);
	emit signalUpdate();
}

void TDrawState::slotAutoOff()
{
	_autoOn = false;
	_flag |= DRAWSTATE_FLAG_AUTO_TURNED_OFF;
	emit signalActionState("Auto center", true, true);
	emit signalUpdate();
}

void TDrawState::slotGpsState(bool fix)
{
	if(!fix) {
		_fix = false;
		emit signalUpdate();
	}
}

void TDrawState::slotGpsData(bool noise, int x, int y, qreal angle)
{
	_validPos = true;
	_currentAngle = angle;
	_fix = true;
	_currentX = x;
	_currentY = y;

	_flag |= DRAWSTATE_FLAG_NEW_GPS_DATA;
	if(noise) {
		_flag |= DRAWSTATE_FLAG_NO_SIGNIFICANT_MOVE;
	}

	emit signalUpdate();
}

void TDrawState::slotCenterTo(int xmin, int xmax, int ymin, int ymax)
{
	setZoom(getCenteringZoom(xmin, xmax, ymin, ymax));
	centerTo(xmin, xmax, ymin, ymax);

	slotAutoOff();
}

void TDrawState::slotTriggerBatchLoading()
{
	int x = (TConverter::convert(_centerX, _zoom) - _width / 2) >> magnification();
	int y = (TConverter::convert(_centerY, _zoom) - _height / 2) >> magnification(); 
	int w = _width >> magnification();  
	int h = _height >> magnification(); 
	int z = _zoom - magnification();

	emit signalBatchLoading(x, y, w, h, z);
}

void TDrawState::slotRefresh()
{
	emit signalUpdate();
}

void TDrawState::slotMoving(bool moving)
{
	_moving = moving;
}

void TDrawState::slotDisplayAlwaysOn(bool alwaysOn)
{
	_displayAlwaysOn = alwaysOn;

	if(_displayAlwaysOn) {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
	} else {
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	}
}

void TDrawState::slotSwitchHttpServer(const QString &name)
{
	_httpName = name;

	_magnificate = _dontMagnificate.indexOf(name) == -1;

	_flag |= DRAWSTATE_FLAG_NEW_SERVER;
	emit signalUpdate();
}

void TDrawState::setCenter(int x, int y)
{
	if((x != _centerX) || (y != _centerY)) {
		_centerX = x;
		_centerY = y;
		_flag |= DRAWSTATE_FLAG_NEW_POSITION;
	}
}

void TDrawState::setZoom(int zoom)
{
	if(zoom != _zoom) {
		_zoom = zoom;
		_flag |= DRAWSTATE_FLAG_NEW_ZOOM | DRAWSTATE_FLAG_NEW_POSITION;
	}
}

int TDrawState::getCenteringZoom(int xmin, int xmax, int ymin, int ymax)
{
	return TConverter::autoZoom(xmax - xmin, ymax - ymin,
		_width - _centeringRightBorder - _centeringLeftBorder,
		_height - _centeringTopBorder - _centeringBottomBorder);
}

void TDrawState::centerTo(int xmin, int xmax, int ymin, int ymax)
{
	int x = (xmin + xmax) / 2 - TConverter::convertBack((_centeringLeftBorder - _centeringRightBorder) / 2, _zoom);
	int y = (ymin + ymax) / 2 - TConverter::convertBack((_centeringTopBorder - _centeringBottomBorder) / 2, _zoom);

	setCenter(x, y);
}

