/*
 * Copyright 2009 Thierry Vuillaume
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

#ifndef TRACELOADER_H
#define TRACELOADER_H

#include <QObject>
#include <QThread>
#include <QFile>

#include "tracesegment.h"

class TTraceLoader : public QThread
{
	Q_OBJECT
	public:
		TTraceLoader();
		~TTraceLoader();
		bool setFile(const QString &filename);
		void run();

	signals:
		void sendSegment(TTraceSegment *seg);

	private:
		QFile *_file;
};

#endif

