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

#ifndef GPSSOURCEABOUT_H
#define GPSSOURCEABOUT_H

#include <QWhereabouts>
#include <QWhereaboutsUpdate>
#include <QFile>

#include "include/gpssource.h"

class TGpsSourceAbout : public TGpsSourcePlugin
{
	Q_OBJECT
	public:
		TGpsSourceAbout(const QString &fileName);
		TGpsSourceAbout();
		~TGpsSourceAbout();

		QString name() { return QString("QWhereabouts"); }

		void start();

		void startRawRecording(const QString &/*filename*/) { };
		void stopRawRecording() { };

	public slots:
		void slotUpdate(const QWhereaboutsUpdate &update);

	signals:
		void signalUpdate(TGpsSample sample);

	private:
		QFile *_file;
		QWhereabouts *_about;
};

#endif

