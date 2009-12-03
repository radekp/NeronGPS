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
	_validPos = false;
	_fix = false;
	_moving = false;
	_displayAlwaysOn = false;
	_server = NULL;
	_currentElasticity = 0;
	_currentAngle = 0;

	connect(&_map, SIGNAL(sendUpdate()), this, SLOT(slotRefresh()));
}

TDrawState::~TDrawState()
{
	_map.setServer(NULL);
}

void TDrawState::loadDefault(TSettings &settings, const QString &section)
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
	_elasticityStartZoom = settings.getValue("elasticitystartzoom", 12).toInt();
	_elasticitySpeed = settings.getValue("elasticityspeed", 10).toInt();
	_elasticityTopBorder = settings.getValue("elasticitytopborder", 130).toInt();
	_elasticityBottomBorder = settings.getValue("elasticitybottomborder", 80).toInt();
	_elasticityRightBorder = settings.getValue("elasticityrightborder", 80).toInt();
	_elasticityLeftBorder = settings.getValue("elasticityleftborder", 80).toInt();
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

void TDrawState::setTileServer(TTileServer *server)
{
	_server = server;

	_map.setServer(server);

	reloadTiles();
	emit signalUpdate();
}

void TDrawState::draw(QPainter &painter)
{
	_map.draw(painter, *this);
}

void TDrawState::setSize(int width, int height)
{
	_width = width;
	_height = height;

	_map.setSurface(_width * 3, _height * 3, _width, _height, Qt::black);

	reloadTiles();
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
		if(_autoOn && _validPos) {
			center();
		}
		reloadTiles();
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
		reloadTiles();
		emit signalUpdate();
	}
}

void TDrawState::slotMove(int offsetX, int offsetY)
{
	_centerX += offsetX;
	_centerY += offsetY;

	moveTiles();
	emit signalUpdate();
}

void TDrawState::slotGoTo(int x, int y)
{
	_centerX = x;
	_centerY = y;

	slotAutoOff();

	moveTiles();
	emit signalUpdate();
}

void TDrawState::slotDriveTo(int x, int y)
{
	_driveX = x;
	_driveY = y;

	_driveOn = true;

	slotAutoOn();
}

void TDrawState::slotAutoOn()
{
	_autoOn = true;

	if(_validPos) {
		if(_driveOn) {
			if(centerDrive()) {
				reloadTiles();
			} else {
				moveTiles();
			}
		} else {
			_centerX = _currentX;
			_centerY = _currentY;
			moveTiles();
		}
	}

	emit signalUpdate();
	emit signalActionState("Auto center", false, false);
}

void TDrawState::slotAutoOff()
{
	if(_autoOn) {
		_autoOn = false;
		emit signalActionState("Auto center", true, true);
	}
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

	if(_autoOn && _driveOn) {
		if(centerDrive()) {
			reloadTiles();
		} else {
			moveTiles();
		}
	} else if(_autoOn) {
		if(noise) {
			_currentElasticity -= _elasticitySpeed;
			if(_currentElasticity < 0) {
				_currentElasticity = 0;
			}
		} else {
			_currentElasticity += _elasticitySpeed;
		}
		center();
		moveTiles();
	}

	emit signalUpdate();
}

void TDrawState::slotCenterTo(int xmin, int xmax, int ymin, int ymax)
{
	centerTo(xmin, xmax, ymin, ymax);

	reloadTiles();
	emit signalUpdate();
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

	if(_moving) {
		_map.flush();
	}
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

	reloadTiles();
	emit signalUpdate();
}

void TDrawState::reloadTiles()
{
	if((!_moving) && (_server != NULL) && (_width != -1)) {
		_map.loadTiles(_httpName, _centerX, _centerY, _zoom, magnification());
	}
}

void TDrawState::moveTiles()
{
	if((!_moving) && (_server != NULL) && (_width != -1)) {
		_map.moveTo(_centerX, _centerY);
	}
}

void TDrawState::center()
{
	if(_zoom >= _elasticityStartZoom) {
		qreal radian = (_currentAngle + 90) * M_PI / 180;

		int offsetX = (int)(cos(radian) * _currentElasticity);
		if(offsetX > (_width / 2) - _elasticityRightBorder) {
			_currentElasticity = ((_width / 2) - _elasticityRightBorder) / cos(radian);
		} else if (offsetX < - (_width / 2) + _elasticityLeftBorder) {
			_currentElasticity = (- (_width / 2) + _elasticityLeftBorder) / cos(radian);
		}
			
		int offsetY = (int)(sin(radian) * _currentElasticity);
		if(offsetY > (_height / 2) - _elasticityBottomBorder) {
			_currentElasticity = ((_height / 2) - _elasticityBottomBorder) / sin(radian);
		} else if (offsetY < - (_height / 2) + _elasticityTopBorder) {
			_currentElasticity = (- (_height / 2) + _elasticityTopBorder) / sin(radian);
		}
			
		offsetX = (int)(cos(radian) * _currentElasticity);
		offsetY = (int)(sin(radian) * _currentElasticity);

		_centerX = _currentX - TConverter::convertBack(offsetX, _zoom); 
		_centerY = _currentY - TConverter::convertBack(offsetY, _zoom); 
	} else {
		_centerX = _currentX;
		_centerY = _currentY;
	}
}

bool TDrawState::centerDrive()
{
	int xmin, xmax, ymin, ymax;

	if(_driveX < _currentX) {
		xmin = _driveX;
		xmax = _currentX;
	} else {
		xmin = _currentX;
		xmax = _driveX;
	}

	if(_driveY < _currentY) {
		ymin = _driveY;
		ymax = _currentY;
	} else {
		ymin = _currentY;
		ymax = _driveY;
	}

	return centerTo(xmin, xmax, ymin, ymax);
}

bool TDrawState::centerTo(int xmin, int xmax, int ymin, int ymax)
{
	int oldZoom = _zoom;

	_centerX = (xmin + xmax) / 2 - TConverter::convertBack((_centeringLeftBorder - _centeringRightBorder) / 2, _zoom);
	_centerY = (ymin + ymax) / 2 - TConverter::convertBack((_centeringTopBorder - _centeringBottomBorder) / 2, _zoom);
	_zoom = TConverter::autoZoom(xmax - xmin, ymax - ymin,
		_width - _centeringRightBorder - _centeringLeftBorder,
		_height - _centeringTopBorder - _centeringBottomBorder);

	return (oldZoom != _zoom);
}

