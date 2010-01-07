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
#include <QMessageBox>
#include <QTimer>

#include "include/tilel1cache.h"
#include "include/tilemap.h"
#include "include/tile.h"
#include "include/drawstate.h"

TTileMap::TTileMap()
{
	_pixmap = NULL;
	_oldPixmap = NULL;
	_server = NULL;
	_reloadLimit = 32;

	connect(&_client, SIGNAL(signalTile(TTile *, void *)), this, SLOT(slotNewTile(TTile *, void *)));

	_first = true;
}

TTileMap::~TTileMap()
{
	_loader.flush(&_client);

	if(_server != NULL) {
		_server->discardTransactions(&_client);
	}

	if(_pixmap != NULL) {
		delete _pixmap;
	}

	while(_drawn.size() > 0) {
		delete _drawn.takeFirst();
	}
}

void TTileMap::setServer(TTileServer *server)
{
	QMutexLocker locker(&_mutex);

	_server = server;
} 

void TTileMap::draw(QPainter &painter, TDrawState &drawState)
{
	if(drawState.flag() & DRAWSTATE_FLAG_NEW_SIZE) {
		setSurface(drawState.width() * 3, drawState.height() * 3, drawState.width(), drawState.height(), Qt::black);
	}

	if(drawState.moving()) {
		flush();
	} else {
		if(drawState.flag() & (DRAWSTATE_FLAG_NEW_SIZE | DRAWSTATE_FLAG_NEW_ZOOM | DRAWSTATE_FLAG_NEW_MAGNIFICATION | DRAWSTATE_FLAG_NEW_SERVER)) {
			loadTiles(drawState.httpName(), drawState.centerX(), drawState.centerY(), drawState.zoom(), drawState.magnification());
		} else if(drawState.flag() & DRAWSTATE_FLAG_NEW_POSITION) {
			moveTo(drawState.centerX(), drawState.centerY());
		}
	}

	int x = (drawState.width() / 2) - (_pixmap->width() / 2);
	x += TConverter::convert(_centerX, _zoom) - TConverter::convert(drawState.centerX(), _zoom);

	int y = (drawState.height() / 2) - (_pixmap->height() / 2);
	y += TConverter::convert(_centerY, _zoom) - TConverter::convert(drawState.centerY(), _zoom);

	painter.drawPixmap(x, y, _pixmap->width(), _pixmap->height(), *_pixmap);
}

void TTileMap::setSurface(int width, int height, int hotWidth, int hotHeight, const QColor &background)
{
	_loader.flush(&_client);

	if(_server != NULL) {
		_server->discardTransactions(&_client);
	}

	if(_pixmap != NULL) {
		delete _pixmap;
	}

	_pixmap = new QPixmap(width, height);

	_width = width;
	_height = height;
	_hotWidth = hotWidth;
	_hotHeight = hotHeight;
	_background = background;
}

void TTileMap::loadTiles(const QString &httpName, int centerX, int centerY, int zoom, int magnification)
{
	QMutexLocker locker(&_mutex);

	_pixmap->fill(_background);

	_httpName = httpName;
	_centerX = centerX;
	_centerY = centerY;
	_magnification = magnification;
	_zoom = zoom;

	load();
}

void TTileMap::moveTo(int centerX, int centerY)
{
	QMutexLocker locker(&_mutex);

	int deltaHoriz = TConverter::convert(_centerX, _zoom) - TConverter::convert(centerX, _zoom);
	if(deltaHoriz < 0) deltaHoriz = -deltaHoriz;

	int deltaVert = TConverter::convert(_centerY, _zoom) - TConverter::convert(centerY, _zoom);
	if(deltaVert < 0) deltaVert = -deltaVert;

	if((deltaHoriz > _reloadLimit) || (deltaVert > _reloadLimit)) {
		if(deltaHoriz > (_width - _hotWidth) / 2) {
			_horizontalOut = true;
		}

		if(deltaVert > (_height - _hotHeight) / 2) {
			_verticalOut = true;
		}

		_oldPixmap = _pixmap;
		_pixmap = new QPixmap(_width, _height);
		_pixmap->fill(_background);

		_oldTranslatX = _translatX;
		_oldTranslatY = _translatY;

		while(_drawn.size() > 0) {
			_oldDrawn.append(_drawn.takeFirst());
		}

		_centerX = centerX;
		_centerY = centerY;

		load();
	}
}

void TTileMap::drawTile(TTile *tile)
{
	QPainter painter(_pixmap);

	int x = (tile->ref().x() << (TILESIZE_P2 + _magnification)) + _translatX;
	int y = (tile->ref().y() << (TILESIZE_P2 + _magnification)) + _translatY;
	int size = TILESIZE << _magnification;
	painter.drawPixmap(x, y, size, size, *tile);

	_drawn.append(new TTileRef(tile->ref()));

	if(_server != NULL) {
		_server->releaseTile(tile, &_client);
	}
}

void TTileMap::flush()
{
	_loader.flush(&_client);

	if(_server != NULL) {
		_server->discardTransactions(&_client);
	}
}

void TTileMap::slotNewTile(TTile *tile, void */*privateData*/)
{
	drawTile(tile);
	int x = tile->ref().x();
	int y = tile->ref().y();
	if( (x >= _leftHot) && (x <= _rightHot) && (y >= _topHot) && (y <= _bottomHot) ) {
		emit sendUpdate();
	}
}

void TTileMap::load()
{
	if(_server == NULL) {
		return;
	}

	_loader.flush(&_client);
	_server->discardTransactions(&_client);

	_first = false;

	int xFull = (TConverter::convert(_centerX, _zoom) - _width / 2) >> _magnification;
	int yFull = (TConverter::convert(_centerY, _zoom) - _height / 2) >> _magnification; 
	int widthFull = _width >> _magnification;
	int heightFull = _height >> _magnification; 

	int xHot = (TConverter::convert(_centerX, _zoom) - _hotWidth / 2) >> _magnification;
	int yHot = (TConverter::convert(_centerY, _zoom) - _hotHeight / 2) >> _magnification; 
	int widthHot = _hotWidth >> _magnification;
	int heightHot = _hotHeight >> _magnification; 

	_leftHot = xHot >> TILESIZE_P2;
	_topHot = yHot >> TILESIZE_P2;
	_rightHot = _leftHot + (widthHot + (xHot & (TILESIZE - 1))) / TILESIZE; 
	_bottomHot = _topHot + (heightHot + (yHot & (TILESIZE - 1))) / TILESIZE;

	int left = xFull >> TILESIZE_P2;
	int top = yFull >> TILESIZE_P2;
	int right = left + (widthFull + (xFull & (TILESIZE - 1))) / TILESIZE;
	int bottom = top + (heightFull + (yFull & (TILESIZE - 1))) / TILESIZE;

	_translatX = _width / 2 - TConverter::convert(_centerX, _zoom);
	_translatY = _height / 2 - TConverter::convert(_centerY, _zoom);

	int zoom = _zoom - _magnification;

	_loader.stop();

	int i, j;
	for(j = top; j <= bottom; j++) {
		for(i = left; i <= right; i++) {
			TTileRef ref(_httpName, i, j, zoom);
			if(ref.state() == TTileRef::Valid) {
				if((j >= _topHot) && (j <= _bottomHot) && (i >= _leftHot) && (i <= _rightHot)) {
					loadOne(ref, true);
				} else {
					loadOne(ref, false);
				}
			}
		}
	}

	_loader.start(20);

	if(_oldPixmap != NULL) {
		delete _oldPixmap;
		_oldPixmap = NULL;
		while(_oldDrawn.size() > 0) {
			delete _oldDrawn.takeFirst();
		}
	}
}

void TTileMap::loadOne(TTileRef &ref, bool hot)
{
	bool drawn = false;

	if(_oldPixmap != NULL) {
		int i;
		for(i = 0; (i < _oldDrawn.size()) && !( (_oldDrawn[i]->x() == ref.x()) && (_oldDrawn[i]->y() == ref.y())); i++);

		if(i != _oldDrawn.size()) {
			bool copy = true;
			bool queue = false;

			int xOld = (ref.x() << (TILESIZE_P2 + _magnification)) + _oldTranslatX;
			int yOld = (ref.y() << (TILESIZE_P2 + _magnification)) + _oldTranslatY;
			int x = (ref.x() << (TILESIZE_P2 + _magnification)) + _translatX;
			int y = (ref.y() << (TILESIZE_P2 + _magnification)) + _translatY;
			int size = TILESIZE << _magnification;

			if((xOld < 0) || (xOld + size >= _width)) {
				if(hot) {
					if(_horizontalOut) {
						copy = false;
					} else {
						queue = true;
					}
				} else {
					queue = true;
				}	
			}	

			if((yOld < 0) || (yOld + size >= _height)) {
				if(hot) {
					if(_verticalOut) {
						copy = false;
					} else {
						queue = true;
					}
				} else {
					queue = true;
				}
			}	

			if(queue) {
				_loader.queue(_server, ref, &_client, distanceToCenter(ref));
			}

			if(copy) {
				QPainter painter(_pixmap);
				painter.drawPixmap(x, y, *_oldPixmap, xOld, yOld, size, size);
				drawn = true;
			}
		}
	}

	if(!drawn) {
		if(hot) {
			TTile *tile = _server->load(ref, &_client);

			if(tile != NULL) {
				drawTile(tile);
			}
		} else {
			_loader.queue(_server, ref, &_client, distanceToCenter(ref));
		}
	}
}

int TTileMap::distanceToCenter(TTileRef &ref)
{
	int x = (ref.x() << (TILESIZE_P2 + _magnification)) + _translatX;
	int y = (ref.y() << (TILESIZE_P2 + _magnification)) + _translatY;
	int size = TILESIZE << _magnification;

	x += size / 2;
	y += size / 2;

	x -= _width / 2;
	y -= _height / 2;

	return x * x + y * y;
}

