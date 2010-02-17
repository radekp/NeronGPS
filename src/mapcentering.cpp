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

#include "include/mapcentering.h"
#include "include/settings.h"
#include "include/drawstate.h"

TMapCentering::TMapCentering()
{
	_currentElasticity = 0;
}

TMapCentering::~TMapCentering()
{
}

void TMapCentering::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);
	_zoomMax = settings.getValue("maxautozoom", 18).toInt();
	_elasticityStartZoom = settings.getValue("elasticitystartzoom", 12).toInt();
	_elasticitySpeed = settings.getValue("elasticityspeed", 10).toInt();
	_elasticityTopBorder = settings.getValue("elasticitytopborder", 130).toInt();
	_elasticityBottomBorder = settings.getValue("elasticitybottomborder", 80).toInt();
	_elasticityRightBorder = settings.getValue("elasticityrightborder", 80).toInt();
	_elasticityLeftBorder = settings.getValue("elasticityleftborder", 80).toInt();
	settings.endGroup();
}

void TMapCentering::draw(QPainter &/*painter*/, TDrawState &drawState)
{
	if(drawState.flag() & DRAWSTATE_FLAG_NEW_GPS_DATA) {
		if(drawState.flag() & DRAWSTATE_FLAG_NO_SIGNIFICANT_MOVE) {
			_currentElasticity -= _elasticitySpeed;
			if(_currentElasticity < 0) {
				_currentElasticity = 0;
			}
		} else {
			_currentElasticity += _elasticitySpeed;
		}
	}

	if((drawState.flag() & DRAWSTATE_FLAG_NEW_ZOOM) && drawState.autoOn() && drawState.driveOn() && drawState.validPos()) {
		drawState.setDriveOnTrack(drawState.zoom() == getDriveZoom(drawState));
	}

	if(drawState.autoOn() && drawState.driveOn() && drawState.validPos()) {
		if(drawState.driveOnTrack()) {
			drawState.setZoom(getDriveZoom(drawState));
			centerDrive(drawState);
		} else if(drawState.zoom() <= getDriveZoom(drawState)) {
			centerDrive(drawState);
		} else {
			center(drawState);
		}
	} else if(drawState.autoOn() && drawState.validPos()) {
		center(drawState);
	}
}

void TMapCentering::center(TDrawState &draw)
{
	int x, y;

	if(draw.zoom() >= _elasticityStartZoom) {
		qreal radian = (draw.currentAngle() + 90) * M_PI / 180;

		int offsetX = (int)(cos(radian) * _currentElasticity);
		if(offsetX > (draw.width() / 2) - _elasticityRightBorder) {
			_currentElasticity = ((draw.width() / 2) - _elasticityRightBorder) / cos(radian);
		} else if (offsetX < - (draw.width() / 2) + _elasticityLeftBorder) {
			_currentElasticity = (- (draw.width() / 2) + _elasticityLeftBorder) / cos(radian);
		}
			
		int offsetY = (int)(sin(radian) * _currentElasticity);
		if(offsetY > (draw.height() / 2) - _elasticityBottomBorder) {
			_currentElasticity = ((draw.height() / 2) - _elasticityBottomBorder) / sin(radian);
		} else if (offsetY < - (draw.height() / 2) + _elasticityTopBorder) {
			_currentElasticity = (- (draw.height() / 2) + _elasticityTopBorder) / sin(radian);
		}
			
		offsetX = (int)(cos(radian) * _currentElasticity);
		offsetY = (int)(sin(radian) * _currentElasticity);

		x = draw.currentX() - TConverter::convertBack(offsetX, draw.zoom()); 
		y = draw.currentY() - TConverter::convertBack(offsetY, draw.zoom()); 
	} else {
		x = draw.currentX();
		y = draw.currentY();
	}

	draw.setCenter(x, y);
}

void TMapCentering::centerDrive(TDrawState &draw)
{
	int xmin, xmax, ymin, ymax;

	if(draw.driveX() < draw.currentX()) {
		xmin = draw.driveX();
		xmax = draw.currentX();
	} else {
		xmin = draw.currentX();
		xmax = draw.driveX();
	}

	if(draw.driveY() < draw.currentY()) {
		ymin = draw.driveY();
		ymax = draw.currentY();
	} else {
		ymin = draw.currentY();
		ymax = draw.driveY();
	}

	draw.centerTo(xmin, xmax, ymin, ymax);
}

int TMapCentering::getDriveZoom(TDrawState &draw)
{
	int xmin, xmax, ymin, ymax;

	if(draw.driveX() < draw.currentX()) {
		xmin = draw.driveX();
		xmax = draw.currentX();
	} else {
		xmin = draw.currentX();
		xmax = draw.driveX();
	}

	if(draw.driveY() < draw.currentY()) {
		ymin = draw.driveY();
		ymax = draw.currentY();
	} else {
		ymin = draw.currentY();
		ymax = draw.driveY();
	}

	int zoom = draw.getCenteringZoom(xmin, xmax, ymin, ymax);

	return (zoom > _zoomMax) ? _zoomMax : zoom;
}


