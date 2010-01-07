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

#ifndef GPXFORM_H
#define GPXFORM_H

#include <QResizeEvent>
#include <QComboBox>

#include "ui_gpxform.h"
#include "include/directory.h"

class TGpxForm : public QWidget
{
	Q_OBJECT

	public:
		TGpxForm(const QString &dir, const QStringList &loaded, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TGpxForm();
		
	public slots:
		void slotColor1(bool checked);
		void slotColor2(bool checked);
		void slotColor3(bool checked);
		void slotColor4(bool checked);
		void slotColor5(bool checked);
		void slotColor6(bool checked);
		void slotColor7(bool checked);
		void slotColor8(bool checked);
		void slotClear(bool checked);
		void slotDelete(bool checked);

	signals:
		void signalGpx(QString gpx, QColor color);
		void signalClear();

	private:
		Ui::GpxForm ui;

		QList<QColor> _colors;

		TDirectory _gpxDir;
		QString _dirName;

		void colorPressed(const QColor &color);
		QString getFile();
		void refresh();
		QIcon colorIcon(const QColor &color, int w, int h);
};

#endif

