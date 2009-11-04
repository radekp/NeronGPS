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

#include <QtGlobal>
#include <QtDebug>
#include <QMessageBox>
#include <QDir>

#include "include/traceserver.h"
#include "include/converter.h"
#include "include/global.h"

TTraceServer::TTraceServer()
{
	_onGoing = new TTrace;
	_displayTrailer = true;

	connect(&_record, SIGNAL(signalSample(int)), this, SLOT(slotRecordSample(int)));
}

TTraceServer::~TTraceServer()
{
}

void TTraceServer::configure(TSettings &settings, const QString &section)
{
	QPen pen;

	settings.beginGroup(section);

	pen.setWidth(settings.getValue("trailthickness", 10).toInt());
	pen.setCapStyle(Qt::RoundCap);
	pen.setColor(settings.getColor("trailcolor", "008000c0"));
	_onGoing->setPen(pen, settings.getValue("trailpoint", 200).toInt());

	_tracePen.setWidth(settings.getValue("tracethickness", 10).toInt());
	_tracePen.setCapStyle(Qt::RoundCap);

	_traceTransparency = settings.getValue("tracetransparency", 192).toInt();
	_tracePoint = settings.getValue("tracepoint", 400).toInt();

	_traceDir = settings.getValue("tracedir", "/media/card/NeronGPS/traces").toString();
	QDir::current().mkpath(_traceDir);
	_record.setDir(_traceDir);

	bool record = settings.getValue("autorecord", "true").toBool();

	settings.endGroup();

	if(record) {
		QMessageBox dialog(QMessageBox::Warning, "Auto-recording enabled", "Start recording?", QMessageBox::Yes | QMessageBox::No);
		if(dialog.exec() == QMessageBox::Yes) {
			_record.start();
		}
	}
}

void TTraceServer::slotLoadTraces(QStringList traceNames)
{
	int i;
	for(i = 0; i < traceNames.size(); i++) {
		QString name = traceNames[i].section(',', 0, -4);
		int r = traceNames[i].section(',', -3, -3).toInt();
		int g = traceNames[i].section(',', -2, -2).toInt();
		int b = traceNames[i].section(',', -1, -1).toInt();

		int j;
		for(j = 0; (j < _traceNames.size()) && (_traceNames[j].section(',', 0, -4) != name); j++);

		if(j < _traceNames.size()) {
			int k;
			for(k = 0; (k < _traces.size()) && (_traces[k]->name() != name); k++);
			if(k < _traces.size()) {
				_tracePen.setColor(QColor(r, g, b, _traceTransparency));
				_traces[k]->setPen(_tracePen, _tracePoint);
			} 
			_traceNames.removeAt(j);
		} else {
			loadTrace(name, QColor(r, g, b, _traceTransparency));
		}
	}

	for(i = 0; i < _traceNames.size(); i++) {
		QString name = _traceNames[i].section(',', 0, -4);

		int j;
		for(j = 0; (j < _traces.size()) && (_traces[j]->name() != name); j++) {
		}

		if(j < _traces.size()) {
			delete _traces.takeAt(j);
		}		
	}

	_traceNames = traceNames; 

	slotCenterTraces();
}

void TTraceServer::loadTrace(QString traceName, QColor color)
{
	QString str;

	str = _traceDir + '/' + traceName;

	TTrace *trace = new TTrace;

	_tracePen.setColor(color);
	trace->setPen(_tracePen, _tracePoint);

	if(QFile::exists(str + ".bin")) {
		trace->loadBin(str + ".bin");

		_mutex.lock();
		_traces.append(trace);
		_mutex.unlock();
	} else if(QFile::exists(str + ".log")) {
		connect(trace, SIGNAL(signalTraceLoaded(TTrace *)), this, SLOT(slotTraceLoaded(TTrace *)));
		connect(trace, SIGNAL(signalTraceLoading(TTrace *, int)), this, SLOT(slotTraceLoading(TTrace *, int)));
		trace->loadLog(str + ".log");
	} else {
		qDebug() << "Error, unable to find trace file: " << traceName;
	}
}

void TTraceServer::slotTraceLoading(TTrace *trace, int samples)
{
	tMessage((void *)trace) << trace->name() << ", " << samples;
}

void TTraceServer::slotTraceLoaded(TTrace *trace)
{
	disconnect(trace, SIGNAL(signalTraceLoaded(TTrace *)), this, SLOT(slotTraceLoaded(TTrace *)));
	disconnect(trace, SIGNAL(signalTraceLoading(TTrace *, int)), this, SLOT(slotTraceLoading(TTrace *, int)));

	_mutex.lock();
	_traces.append(trace);
	_mutex.unlock();

	tMessage((void *)trace) << "";

	slotCenterTraces();
}

void TTraceServer::slotCenterTraces()
{
	if(_traces.size() == 0) {
		return;
	}

	_mutex.lock();

	int xmin = _traces[0]->xmin();
	int xmax = _traces[0]->xmax();
	int ymin = _traces[0]->ymin();
	int ymax = _traces[0]->ymax();

	int i;
	for(i = 1; i < _traces.size(); i++) {
		if(_traces[i]->xmin() < xmin) {
			xmin = _traces[i]->xmin();
		}
		if(_traces[i]->xmax() > xmax) {
			xmax = _traces[i]->xmax();
		}
		if(_traces[i]->ymin() < ymin) {
			ymin = _traces[i]->ymin();
		}
		if(_traces[i]->ymax() > ymax) {
			ymax = _traces[i]->ymax();
		}	
	}

	_mutex.unlock();

	emit signalTraceLoaded(xmin, xmax, ymin, ymax);
}

void TTraceServer::slotGpsData(const QWhereaboutsUpdate &update)
{
	int x = TConverter::prepareX(update.coordinate().longitude());
	int y = TConverter::prepareY(update.coordinate().latitude());
	
	_onGoing->addSample(x, y);

	_record.newGpsData(update);
}

void TTraceServer::draw(QPainter &painter, TDrawState &drawState)
{
	if(!drawState.moving()) {
		int i;

		QMutexLocker locker(&_mutex);

		if(_displayTrailer) {
			_onGoing->draw(painter, drawState);
		}

		for(i = 0; i < _traces.size(); i++) {
			_traces[i]->draw(painter, drawState);
		}
	}
}

void TTraceServer::slotRecord(bool record)
{
	if(record) {
		emit signalRecordInfo("Wait for fix", 0);
		_record.start();
	} else {
		emit signalRecordInfo("-", 0);
		_record.stop();
	}
}

void TTraceServer::slotDisplayTrailer(bool display)
{
	_displayTrailer = display;
}

void TTraceServer::slotRecordSample(int samples)
{
	if(samples == -1) {
		emit signalRecordInfo("Recording error", 0);
	} else {
		emit signalRecordInfo(_record.filename(), samples);	
	}
}

void TTraceServer::slotReset()
{
	_onGoing->reset();
	_record.reset();
}



