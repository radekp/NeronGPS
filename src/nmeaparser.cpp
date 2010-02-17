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

#include "include/nmeaparser.h"

TNmeaParser::TNmeaParser()
{
}

TNmeaParser::~TNmeaParser()
{
}

bool TNmeaParser::parse(const QString &sentence)
{
	bool valid = false;

	QStringList elements = sentence.split(',');
	QString id = elements.takeFirst();

	if(id == QString("$GPRMC")) {
		valid = parseRMC(elements);
	}

	return valid;
}

bool TNmeaParser::parseRMC(const QStringList &elements)
{

	bool valid = (elements.size() > 7) && (elements[1] == QString("A"));

	if(valid) {
		QDateTime time = parseDateTime(elements[8], elements[0]);
		double latitude = parseLatitude(elements[2], elements[3]);
		double longitude = parseLongitude(elements[4], elements[5]);

		_sample = TGpsSample(time, latitude, longitude);
		_sampleReady = true;
	}

	return valid;
}

QDateTime TNmeaParser::parseDateTime(const QString &strDate, const QString &strTime)
{
	int year = 2000 + strDate.mid(4, 2).toInt();
	int month = strDate.mid(2, 2).toInt();
	int day = strDate.mid(0, 2).toInt();
	QDate date(year, month, day);

	int hours = strTime.mid(0, 2).toInt();
	int minutes = strTime.mid(2, 2).toInt();
	int seconds = strTime.mid(4, 2).toInt();
	int ms = (strTime.size() > 6) ? (strTime.mid(6, 1).toInt() * 100) : 0; 
	QTime time(hours, minutes, seconds, ms);
	
	return QDateTime(date, time, Qt::UTC);
}

double TNmeaParser::parseLatitude(const QString &angle, const QString &direction)
{
	double latitude = angle.mid(0, 2).toInt();

	latitude += angle.mid(2).toDouble() / 60;

	if(direction == QString("S")) {
		latitude = -latitude;
	}

	return latitude;
}

double TNmeaParser::parseLongitude(const QString &angle, const QString &direction)
{
	double longitude = angle.mid(0, 3).toInt();

	longitude += angle.mid(3).toDouble() / 60;

	if(direction == QString("W")) {
		longitude = -longitude;
	}

	return longitude;
}

