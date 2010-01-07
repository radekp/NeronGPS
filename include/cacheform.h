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

#ifndef CACHEFORM_H
#define CACHEFORM_H

#include <QDir>

#include "ui_cacheform.h"
#include "mapwidget.h"

class TCacheForm : public QWidget
{
	Q_OBJECT

	public:
		TCacheForm(const QString &cacheDir, const QStringList &cacheList, const QStringList &serverList, const QString &cache, int beginZoom, int endZoom, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TCacheForm();
		
	public slots:
		void beginZoomChanged(int value);
		void endZoomChanged(int value);
		void newCache(bool checked);
		void deleteCache(bool checked);
		void setCache(bool checked);
		void downCache(bool checked);
		void slotNewCacheList(QStringList cacheList);
		void slotCacheChanged(const QString &name);

	signals:
		void signalNewLoadingCache(QString cache, int beginZoom, int endZoom, bool forceLoad, QStringList servers);
		void signalAddCache();
		void signalDeleteCache(QString cache);

	private:
		Ui::CacheForm ui;

		QDir _dir;
};

#endif
