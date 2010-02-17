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
#include <QMessageBox>

#include <math.h>

#include "include/converter.h"

TConverter::TConverter()
{
}

TConverter::~TConverter()
{
}

QString TConverter::longitudeToString(double lon)
{
	QString endString;

	if(lon < 0) {
		lon = -lon;
		endString = QString(QLatin1Char(176)) + 'W';
	} else {
		endString = QString(QLatin1Char(176)) + 'E';
	}

	return QString("%1").arg(lon, 0, 'f', 10) + endString;
}

QString TConverter::latitudeToString(double lat)
{
	QString endString;

	if(lat < 0) {
		lat = -lat;
		endString = QString(QLatin1Char(176)) + 'S';
	} else {
		endString = QString(QLatin1Char(176)) + 'N';
	}

	return QString("%1").arg(lat, 0, 'f', 10) + endString;
}

double TConverter::longitudeFromString(const QString &lon)
{
	double ret = 0;
	QString tmp = lon;

	tmp.remove(' ');

	QStringList split = tmp.split(QLatin1Char(176));
	if(split.size() == 2) {
		ret = (double)split[0].toDouble();
	
		if(split[1] == "W") {
			ret = -ret;
		} else if(split[1] != "E") {
			qDebug() << "Error, invalid longitude format, end character is not 'E' or 'W': " << lon;
		}
	} else {
		qDebug() << "Error, invalid longitude format, no '°' found: " << lon;
	}

	return ret;
}

double TConverter::latitudeFromString(const QString &lat)
{
	double ret = 0;
	QString tmp = lat;

	tmp.remove(' ');

	QStringList split = tmp.split(QLatin1Char(176));
	if(split.size() == 2) {
		ret = (double)split[0].toDouble();
	
		if(split[1] == "S") {
			ret = -ret;
		} else if(split[1] != "N") {
			qDebug() << "Error, invalid longitude format, end character is not 'N' or 'S': " << lat;
		}
	} else {
		qDebug() << "Error, invalid longitude format, no '°' found: " << lat;
	}

	return ret;
}

double TConverter::earthRadius(double lat)
{
	double a = 6378137.0;
	double b = 6356752.3;

	double c = cos(lat);
	double s = sin(lat);

	double tmp1 = a * a * c;
	double tmp2 = b * b * s;
	double tmp3 = a * c;
	double tmp4 = b * s;

	double tmp5 = tmp1 * tmp1 + tmp2 * tmp2;
	double tmp6 = tmp3 * tmp3 + tmp4 * tmp4;

	return (float)sqrt( tmp5 / tmp6 );
}

float TConverter::distance(double lat1, double lon1, double lat2, double lon2, double altitude)
{
	float val;

	if((lat1 == lat2) && (lon1 == lon2)) {
		val = 0;
	} else {
		lat1 = lat1 * M_PI / 180;
		lon1 = lon1 * M_PI / 180;
		lat2 = lat2 * M_PI / 180;
		lon2 = lon2 * M_PI / 180;

		double interim = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon2 - lon1);

		if(interim >= 1) {
			val = 0;
		} else if(interim <= -1) {
			val = M_PI;
		} else {
			val = (earthRadius(lat1) + altitude) * acos(interim);
		}
	}

	return val;
}

