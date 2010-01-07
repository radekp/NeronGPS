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

#ifndef TILEMAP_H
#define TILEMAP_H

#include <QObject>
#include <QList>
#include <QPainter>
#include <QMutex>
#include <QTimer>

#include "include/tileserver.h"
#include "include/tileasyncloader.h"
#include "include/tile.h"
#include "include/tileclient.h"
#include "include/tilel3cachemux.h"
#include "include/mapdrawlist.h"

class TDrawState;

class TTileMap : public TMapDrawListElement
{
	Q_OBJECT

	public:
		TTileMap();
		~TTileMap();

		void setServer(TTileServer *server);
		void setSurface(int width, int height, int hotWidth, int hotHeight, const QColor &background);

		void loadTiles(const QString &httpName, int centerX, int centerY, int zoom, int magnification);
		void moveTo(int centerX, int centerY);

		void draw(QPainter &painter, TDrawState &drawState);
		void flush();

	signals:
		void sendUpdate();

	public slots:
		void slotNewTile(TTile *tile, void *privateData);

	private:
		bool _first;

		QMutex _mutex;

		TTileAsyncLoader _loader;
		TTileServer *_server;
		TTileClient _client;
		QString _httpName;

		QPixmap *_pixmap;
		QQueue<TTileRef *> _toLoad;
		QList<TTileRef *> _drawn;
		QColor _background;
		int _reloadLimit;

		QPixmap *_oldPixmap;
		QList<TTileRef *> _oldDrawn;
		int _oldTranslatX;
		int _oldTranslatY;
		bool _verticalOut;
		bool _horizontalOut;

		int _width, _height, _hotWidth, _hotHeight;
		int _leftHot, _rightHot, _topHot, _bottomHot;
		int _magnification;
		int _translatX;
		int _translatY;
		int _centerX;
		int _centerY;
		int _zoom;

		void drawTile(TTile *tile);
		void load();
		void loadOne(TTileRef &ref, bool hot);
		int distanceToCenter(TTileRef &ref);
};

#endif

