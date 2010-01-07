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

#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

#include <math.h>

#include "tile.h"

#define NUMLEVELS 20

class TConverter : public QObject
{
	Q_OBJECT
	public:
		static int convertLon(double lon, int zoom) {
			return ((int)((1 << NUMLEVELS) * (lon * TILESIZE / 360 + (TILESIZE / 2)))) >> zoom;
		}
		static int convertLat(double lat, int zoom) {
			return ((int)((1 << NUMLEVELS) * (-atanh(sin(lat*M_PI/180)) * TILESIZE / (2*M_PI) + (TILESIZE/2)))) >> zoom;
		}
		static int prepareX(double lon) { 
			return (int)((1 << NUMLEVELS) * (lon * TILESIZE / 360 + (TILESIZE / 2)));
		}
		static int prepareY(double lat) {
			return (int)((1 << NUMLEVELS) * (-atanh(sin(lat*M_PI/180)) * TILESIZE / (2*M_PI) + (TILESIZE/2)));
		}
		static double unprepareX(int x) { 
			return (((double)(x) / (1 << NUMLEVELS)) - (TILESIZE / 2.)) * 360. / TILESIZE;
		}
		static double unprepareY(int y) {
			return asin(-tanh((((double)(y) / (1 << NUMLEVELS)) - (TILESIZE/2.)) * (2.*M_PI) / TILESIZE)) * 180. / M_PI;
		}
		static int convert(int value, int zoom) {
			return value >> (NUMLEVELS - zoom);
		}
		static int convertBack(int value, int zoom) {
			return value << (NUMLEVELS - zoom);
		}
		static int autoZoom(int width, int height, int fwidth, int fheight) {
			int i;
			for (i = 0; (i < NUMLEVELS) && (((width >> i) > fwidth) || ((height >> i) > fheight)); i++);
			return (NUMLEVELS - i);
		}

		static QString longitudeToString(double lon);
		static QString latitudeToString(double lat);
		static double longitudeFromString(const QString &lon);
		static double latitudeFromString(const QString &lat);
		static double earthRadius(double lat);
		static float distance(double lat1, double lon1, double lat2, double lon2, double altitude);

	private:
		TConverter();
		~TConverter();
};

#endif

