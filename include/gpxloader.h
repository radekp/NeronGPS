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

#ifndef GPXLOADER_H
#define GPXLOADER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QMutex>
#include <QList>
#include <QXmlStreamReader>

#include "trace.h"
#include "tracesegment.h"

class TGpxLoader : public QThread
{
	Q_OBJECT
	public:
		TGpxLoader();
		~TGpxLoader();

		void configure(TSettings &settings, const QString &section);

		void run();

	public slots:
		void slotLoad(QString gpx, QColor color);

	signals:
		void signalLoaded(QList<TTrace *> *traces);

	private:
		QMutex _mutex;

		QPen _pen;
		int _point;
		int _transparency;

		QStringList _names;
		QList<QColor> _colors;

		TTrace *loadTrace(QXmlStreamReader &xml, const QString &name, const QColor &color);
};

#endif

