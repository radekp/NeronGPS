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

#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QStringList>
#include <QMutex>

class TLog : public QObject, public QStringList
{
	Q_OBJECT
	public:
		TLog();
		~TLog();

		void operator<< (const QString &str);

	public slots:
		void slotClear();

	signals:
		void signalLog(QString log);
		void signalClear();

	private:
		QMutex _mutex;
};

#endif

