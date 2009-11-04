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

#ifndef TRACESERVER_H
#define TRACESERVER_H

#include <QObject>
#include <QList>
#include <QPainterPath>
#include <QMutex>
#include <QWhereaboutsUpdate>

#include "include/trace.h"
#include "include/tracerecorder.h"
#include "include/drawstate.h"

class TTraceServer : public QObject
{
	Q_OBJECT
	public:
		TTraceServer();
		~TTraceServer();

		void configure(TSettings &settings, const QString &section);
		void draw(QPainter &painter, TDrawState &drawState);

		QString getDir() { return _traceDir; }
		bool isRecording() { return _record.isRecording(); }
		int recordSamples() { return _record.samples(); }
		QStringList &traceNames() { return _traceNames; }
	
	public slots:
		void slotLoadTraces(QStringList traceNames);
		void slotTraceLoading(TTrace *trace, int samples);
		void slotTraceLoaded(TTrace *trace);
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotRecord(bool record);
		void slotDisplayTrailer(bool display);
		void slotRecordSample(int samples);
		void slotReset();
		void slotCenterTraces();

	signals:
		void signalTraceLoaded(int xmin, int xmax, int ymin, int ymax);
		void signalRecordInfo(QString name, int samples);

	private:
		QMutex _mutex;

		TTraceRecorder _record;

		QList<TTrace *> _traces;
		QStringList _traceNames;

		TTrace *_onGoing;

		QString _traceDir;
		QPen _tracePen;
		int _tracePoint;
		int _traceTransparency;

		bool _displayTrailer;

		void loadTrace(QString traceName, QColor color);
};

#endif

