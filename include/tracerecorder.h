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

#ifndef TRACERECORDER_H
#define TRACERECORDER_H

#include <QObject>
#include <QFile>
#include <QWhereaboutsUpdate>
#include <QMutex>

#define TRACERECORDER_CONCATENATION	32

class TTraceRecorder : public QObject
{
	Q_OBJECT
	public:
		TTraceRecorder();
		~TTraceRecorder();

		void setDir(const QString &dir);

		bool isRecording() { return _state != stopped; }
		const QString &filename() { return _filename; }

		int samples() { return _samples; }

	public slots:
		void newGpsData(const QWhereaboutsUpdate &update);
		void start();
		void stop();
		void reset();

	signals:
		void signalSample(int samples);

	private:
		QMutex _mutex;

		enum {stopped, starting, reseting, started} _state;
		QString _dirName;
		QString _shortName;
		QString _filename;
		QFile *_file;
		QFile *_bin;
		int _samples;
		int _section;

		QString _concatLog;
		QByteArray _concatBin;
		int _concatCount;

		void selectNextName(const QString &day);
		void createFile(const QString &time);
		void addSample(const QWhereaboutsUpdate &update);
		void flush();	
};

#endif

