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

#ifndef POIFORM_H
#define POIFORM_H

#include "ui_poiform.h"

class TPoiForm : public QWidget
{
	Q_OBJECT

	public:
		TPoiForm(const QStringList &poiList, int x, int y, const QStringList &keyboard, QWidget *parent = 0, Qt::WFlags f = 0);
		~TPoiForm();
		
	public slots:
		void slotAdd(bool checked);
		void slotDelete(bool checked);
		void slotGoTo(bool checked);
		void slotDriveTo(bool checked);
		void slotPoiName(QString name);
		void slotItemClicked(QListWidgetItem *item);
		void slotKill();

	signals:
		void signalPoi(QString name, QString coordinates);
		void signalDeletePoi(QString poi);
		void signalGoTo(int x, int y);
		void signalDriveTo(int x, int y);

	private:
		Ui::PoiForm ui;

		QStringList _keyboard;
};

#endif

