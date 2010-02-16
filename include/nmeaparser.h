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

#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <QStringList>

#include "include/gpssample.h"

class TNmeaParser
{
	public:
		TNmeaParser();
		~TNmeaParser();

		bool parse(const QString &sentence);
		bool sampleReady() const { return _sampleReady; }
		TGpsSample &retrieveSample() { _sampleReady = false; return _sample; }

	private:
		TGpsSample _sample;
		bool _sampleReady;

		bool parseRMC(const QStringList &elements);
		QDateTime parseDateTime(const QString &strDate, const QString &strTime);
		double parseLatitude(const QString &angle, const QString &direction);
		double parseLongitude(const QString &angle, const QString &direction);
};

#endif

