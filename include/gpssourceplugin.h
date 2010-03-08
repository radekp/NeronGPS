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

#ifndef GPSSOURCEPLUGIN_H
#define GPSSOURCEPLUGIN_H

#include <QObject>

#include "include/gpssample.h"

class TGpsSourcePlugin : public QObject
{
	Q_OBJECT
	public:
		virtual QString name() = 0;

		virtual void start() = 0;

		virtual void startRawRecording(const QString &filename) = 0;
		virtual void stopRawRecording() = 0;

	signals:
		virtual void signalUpdate(TGpsSample sample) = 0;
};

#endif

