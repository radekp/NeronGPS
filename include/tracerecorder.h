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

#ifndef TRACERECORDER_H
#define TRACERECORDER_H

#include <QObject>
#include <QFile>
#include <QWhereaboutsUpdate>
#include <QMutex>

class TSettings;

class TTraceRecorder : public QObject
{
	Q_OBJECT
	public:
		TTraceRecorder();
		~TTraceRecorder();

		void configure(TSettings &settings, const QString &section);

		QString dir() { return _dir; }

		bool isRecording() { return _state != stopped; }
		const QString &filename() { return _filename; }

		int samples() { return _samples; }

	public slots:
		void slotGpsState(bool fix);
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotRecord(bool record);
		void slotReset();

	signals:
		void signalRecordInfo(QString name, int samples);

	private:
		QMutex _mutex;

		enum {stopped, starting, reseting, started} _state;
		QString _dir;
		QString _filename;
		QFile *_bin;
		QFile *_gpx;
		int _samples;
		int _section;
		bool _onSeg;

		void createFile(const QWhereaboutsUpdate &update);
		void addSample(const QWhereaboutsUpdate &update);
		void close();	

		void gpxHeader();
		void gpxFooter();
		void gpxNewTrack();
		void gpxStartSegment();
		void gpxEndSegment();
		void writeGpx(const QWhereaboutsUpdate &update);
};

#endif

