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

#include <QtGlobal>
#include <QtDebug>

#include "maemo5/gpssourcelibloc.h"

TGpsSourceLibloc::TGpsSourceLibloc()
{
	_control = location_gpsd_control_get_default();
	_device = (LocationGPSDevice*)g_object_new(LOCATION_TYPE_GPS_DEVICE, NULL);
}

TGpsSourceLibloc::~TGpsSourceLibloc()
{
	if(_device != NULL) {
		g_object_unref(_device);
	}

	if(_control != NULL) {
		location_gpsd_control_stop(_control);
	}
}

void TGpsSourceLibloc::start()
{
	if (_control != NULL) {
		location_gpsd_control_start(_control);
	}

	if (_device != NULL) {
		g_signal_connect(_device, "changed", G_CALLBACK(TGpsSourceLibloc::locationChanged), this);
	}
}

void TGpsSourceLibloc::locationChanged(LocationGPSDevice *device, gpointer userdata)
{
	TGpsSourceLibloc *source = static_cast<TGpsSourceLibloc *>(userdata);
	source->update(device);
}

void TGpsSourceLibloc::update(LocationGPSDevice *device)
{
	if(((device->fix->mode == LOCATION_GPS_DEVICE_MODE_3D) || (device->fix->mode == LOCATION_GPS_DEVICE_MODE_2D)) &&
           (device->fix->fields & LOCATION_GPS_DEVICE_LATLONG_SET) &&
	   (device->fix->fields & LOCATION_GPS_DEVICE_TIME_SET)) {
		QDateTime time;
		time.setTime_t(device->fix->time);
		TGpsSample sample(time, device->fix->latitude, device->fix->longitude);

		if((device->fix->mode == LOCATION_GPS_DEVICE_MODE_3D) && (device->fix->fields & LOCATION_GPS_DEVICE_ALTITUDE_SET)) {
			sample.setAltitude(device->fix->altitude);
    		}

		if(device->fix->fields & LOCATION_GPS_DEVICE_SPEED_SET) {
			sample.setSpeed(device->fix->speed);
		}

		emit signalUpdate(sample);
	}
}

