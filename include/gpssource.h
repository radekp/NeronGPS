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

#ifndef GPSSOURCE_H
#define GPSSOURCE_H

#include "include/gpssample.h"
#include "include/gpssourceplugin.h"
#include "include/gpssourcenmea.h"
#include "include/gpssourcegpsd.h"
#include "include/settings.h"

class TGpsSource : public QObject
{
	Q_OBJECT
	public:
		TGpsSource();
		~TGpsSource();

		void configure(TSettings &settings, const QString &section);

		void addSource(TGpsSourcePlugin *_plugin);
		void start();

	public slots:
		void slotRawRecording(bool start);
		void slotNmeaPlay(const QString &filename);
		void slotNmeaStop();

	private slots:
		void slotUpdate(TGpsSample sample);

	signals:
		void signalUpdate(TGpsSample sample);

	private:
		TGpsSourceGpsd _gpsdSource;
		TGpsSourceNmea _nmeaSource;
		TGpsSourcePlugin *_source;

		QString _rawDir;
		bool _recording;
};

#endif

