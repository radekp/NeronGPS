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

#ifndef DRAWSTATE_H
#define DRAWSTATE_H

#include <QObject>
#include <QSettings>

#include "include/converter.h"
#include "include/settings.h"
#include "include/tileserver.h"

#define DRAWSTATE_FLAG_NEW_SIZE			0x00000001
#define DRAWSTATE_FLAG_NEW_ZOOM			0x00000002
#define DRAWSTATE_FLAG_NEW_MAGNIFICATION	0x00000004
#define DRAWSTATE_FLAG_NEW_POSITION		0x00000008
#define DRAWSTATE_FLAG_NEW_SERVER		0x00000010
#define DRAWSTATE_FLAG_AUTO_TURNED_ON		0x00000020
#define DRAWSTATE_FLAG_AUTO_TURNED_OFF		0x00000040
#define DRAWSTATE_FLAG_DRIVETO_TURNED_ON	0x00000080
#define DRAWSTATE_FLAG_NEW_GPS_DATA		0x00000100
#define DRAWSTATE_FLAG_NO_SIGNIFICANT_MOVE	0x00000200

class TTrace;

class TDrawState : public QObject
{
	Q_OBJECT
	public:
		TDrawState();
		~TDrawState();

		void configure(TSettings &settings, const QString &section);
		void setSize(int width, int height);

		uint flag() { return _flag; }
		int width() { return _width; }
		int height() { return _height; }
		int zoom() { return _zoom; }
		int magnification() { return (_magnificate) ? _magnification : 0; }
		int magnificationParam() { return _magnification; }
		int centerX() { return _centerX; }
		int centerY() { return _centerY; }
		int currentX() { return _currentX; }
		int currentY() { return _currentY; }
		int driveX() { return _driveX; }
		int driveY() { return _driveY; }
		qreal currentAngle() { return _currentAngle; }
		bool validPos() { return _validPos; }
		bool fix() { return _fix; }
		bool moving() { return _moving; }
		bool autoOn() { return _autoOn; }
		bool driveOn() { return _driveOn; }
		bool driveOnTrack() { return _driveOnTrack; }
		bool displayAlwaysOn() { return _displayAlwaysOn; }
		const QString &httpName() { return _httpName; }
		void getPosition(int &x, int &y);

		int locationX() { return TConverter::convert(_currentX, _zoom) + _width / 2 - TConverter::convert(_centerX, _zoom); }
		int locationY() { return TConverter::convert(_currentY, _zoom) + _height / 2 - TConverter::convert(_centerY, _zoom); }
		int driveLocationX() { return TConverter::convert(_driveX, _zoom) + _width / 2 - TConverter::convert(_centerX, _zoom); }
		int driveLocationY() { return TConverter::convert(_driveY, _zoom) + _height / 2 - TConverter::convert(_centerY, _zoom); }

		int translatX() { return _width / 2 - TConverter::convert(_centerX, _zoom); }
		int translatY() { return _height / 2 - TConverter::convert(_centerY, _zoom); }

		int magnify(int val) { return val << magnification(); }

		void setCenter(int x, int y);
		void setZoom(int zoom);
		void setDriveOnTrack(bool onTrack) { _driveOnTrack = onTrack; }
		int getCenteringZoom(int xmin, int xmax, int ymin, int ymax);
		void centerTo(int xmin, int xmax, int ymin, int ymax);

		void clearFlag() { _flag = 0; }

	public slots:
		void slotGpsState(bool fix);
		void slotGpsData(bool noise, int x, int y, qreal angle);
		void slotSetZoom(int zoom);
		void slotZoomPlus();
		void slotZoomMinus();
		void slotSetMagnification(int magnification);
		void slotCenterTo(int xmin, int xmax, int ymin, int ymax);
		void slotMove(int offsetX, int offsetY);
		void slotGoTo(int x, int y);
		void slotDriveTo(int x, int y);
		void slotCancelDriveTo();
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
		uint _flag;
		QString _httpName;

		int _centeringTopBorder;
		int _centeringBottomBorder;
		int _centeringRightBorder;
		int _centeringLeftBorder;

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
		int _driveX;
		int _driveY;
		qreal _currentAngle;
		bool _validPos;
		bool _fix;
		bool _autoOn;
		bool _driveOn;
		bool _driveOnTrack;
		bool _moving;
		bool _displayAlwaysOn;
};

#endif

