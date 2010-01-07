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

#ifndef POISERVER_H
#define POISERVER_H

#include <QObject>
#include <QStringList>
#include <QMutex>
#include <QFile>

class TSettings;

class TPoiServer : public QObject
{
	Q_OBJECT
	public:
		TPoiServer();
		~TPoiServer();

		void configure(TSettings &settings, const QString &section);

		const QStringList &poiList() { return _poiList; }

	public slots:
		void slotRegisterPoi(QString name, QString coordinates);
		void slotDeletePoi(QString poi);

	signals:
		void signalPoiList(QStringList poiList);

	private:
		QMutex _mutex;

		QString _poiDirName;
		QString _poiFileName;

		QFile *_poiFile;
		QStringList _poiList;

		void writeFile();
};

#endif

