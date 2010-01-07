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

#ifndef ZOOMFORM_H
#define ZOOMFORM_H

#include "ui_zoomform.h"

class TZoomForm : public QWidget
{
	Q_OBJECT

	public:
		TZoomForm(int zoom, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TZoomForm();
		
	public slots:
		void slotZoom1Clicked(bool checked);
		void slotZoom2Clicked(bool checked);
		void slotZoom3Clicked(bool checked);
		void slotZoom4Clicked(bool checked);
		void slotZoom5Clicked(bool checked);
		void slotZoom6Clicked(bool checked);
		void slotZoom7Clicked(bool checked);
		void slotZoom8Clicked(bool checked);
		void slotZoom9Clicked(bool checked);
		void slotZoom10Clicked(bool checked);
		void slotZoom11Clicked(bool checked);
		void slotZoom12Clicked(bool checked);
		void slotZoom13Clicked(bool checked);
		void slotZoom14Clicked(bool checked);
		void slotZoom15Clicked(bool checked);
		void slotZoom16Clicked(bool checked);
		void slotZoom17Clicked(bool checked);
		void slotZoom18Clicked(bool checked);
		void slotZoom19Clicked(bool checked);
		void slotZoom20Clicked(bool checked);
		void slotKill();

	signals:
		void signalZoom(int zoom);

	private:
		Ui::ZoomForm ui;
};

#endif

