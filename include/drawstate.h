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

#ifndef DRAWSTATE_H
#define DRAWSTATE_H

#include <QObject>
#include <QSettings>

#include "include/converter.h"
#include "include/settings.h"
#include "include/tilemap.h"
#include "include/tileserver.h"

class TTrace;

class TDrawState : public QObject
{
	Q_OBJECT
	public:
		TDrawState();
		~TDrawState();

		void loadDefault(TSettings &settings, const QString &section);
		void setSize(int width, int height);
		void setTileServer(TTileServer *server);

		void draw(QPainter &painter);

		int width() { return _width; }
		int height() { return _height; }
		int zoom() { return _zoom; }
		int magnification() { return (_magnificate) ? _magnification : 0; }
		int magnificationParam() { return _magnification; }
		int centerX() { return _centerX; }
		int centerY() { return _centerY; }
		int currentX() { return _currentX; }
		int currentY() { return _currentY; }
		qreal currentAngle() { return _currentAngle; }
		bool validAngle() { return _validAngle; }
		bool validPos() { return _validPos; }
		bool fix() { return _fix; }
		bool moving() { return _moving; }
		bool displayAlwaysOn() { return _displayAlwaysOn; }
		const QString &httpServer() { return _httpName; }

		int locationX() { return TConverter::convert(_currentX, _zoom) + _width / 2 - TConverter::convert(_centerX, _zoom); }
		int locationY() { return TConverter::convert(_currentY, _zoom) + _height / 2 - TConverter::convert(_centerY, _zoom); }

		int translatX() { return _width / 2 - TConverter::convert(_centerX, _zoom); }
		int translatY() { return _height / 2 - TConverter::convert(_centerY, _zoom); }

		int magnify(int val) { return val << magnification(); }

	public slots:
		void slotGpsState(bool fix);
		void slotGpsData(int x, int y, qreal angle, bool angleValid);
		void slotSetZoom(int zoom);
		void slotZoomPlus();
		void slotZoomMinus();
		void slotSetMagnification(int magnification);
		void slotCenterTo(int xmin, int xmax, int ymin, int ymax);
		void slotMove(int offsetX, int offsetY);
		void slotAutoOn();
		void slotAutoOff();
		void slotTriggerBatchLoading();
		void slotRefresh();
		void slotMoving(bool moving);
		void slotDisplayAlwaysOn(bool alwaysOn);
		void slotSwitchHttpServer(const QString &name);

	signals:
		void signalUpdate();
		void signalActionState(const QString &name, bool visible, bool enabled);
		void signalBatchLoading(int x, int y, int w, int h, int zStart);

	private:
		TTileMap _map;
		TTileServer *_server;
		QString _httpName;

		QStringList _dontMagnificate;
		int _width;
		int _height;
		int _zoom;
		int _magnification;
		bool _magnificate;
		int _centerX;
		int _centerY;
		int _currentX;
		int _currentY;
		qreal _currentAngle;
		bool _validAngle;
		bool _validPos;
		bool _fix;
		bool _auto;
		bool _moving;
		bool _displayAlwaysOn;

		void reloadTiles();
		void moveTiles();
};

#endif

