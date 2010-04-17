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

#ifndef GPSSOURCELIBLOC_H
#define GPSSOURCELIBLOC_H

extern "C" {
#include <location/location-gps-device.h>
#include <location/location-gpsd-control.h>
}

#include "include/gpssource.h"

class TGpsSourceLibloc : public TGpsSourcePlugin
{
	Q_OBJECT
	public:
		TGpsSourceLibloc();
		~TGpsSourceLibloc();

		QString name() { return QString("Liblocation"); }

		void start();

		void startRawRecording(const QString &/*filename*/) { };
		void stopRawRecording() { };

	public slots:
		static void locationChanged(LocationGPSDevice *device, gpointer userdata);

	signals:
		void signalUpdate(TGpsSample sample);

	private:
		LocationGPSDevice *_device;
		LocationGPSDControl *_control;

		void update(LocationGPSDevice *device);
};

#endif

