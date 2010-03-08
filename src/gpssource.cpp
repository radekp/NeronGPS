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
#include <QDir>

#include "include/global.h"
#include "include/gpssource.h"

TGpsSource::TGpsSource()
{
	_recording = false;

	if(_gpsdSource.connect()) {
		qDebug() << "Connected to gpsd";
		_source = &_gpsdSource;
		connect(_source, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));
	} else {
		qDebug() << "Unable to connect to gpsd";
		_source = NULL;
	}
}

TGpsSource::~TGpsSource()
{
}

void TGpsSource::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_rawDir = settings.getValue("rawdir", settings.rootDir() + QString("/nmea")).toString();
	QDir::current().mkpath(_rawDir);

	settings.endGroup();	
}

void TGpsSource::addSource(TGpsSourcePlugin *_plugin)
{
	if(_source == NULL) {
		_source = _plugin;
		connect(_source, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));
	}
}

void TGpsSource::start()
{
	if(_source != NULL) {
		qDebug() << "Use gps source: " << _source->name();
		tUserLog() << "Gps source: " << _source->name();
		_source->start();
	} else {
		qDebug() << "Cannot start gps, no source available";
		tUserLog() << "No gps source available";
	}
}

void TGpsSource::slotRawRecording(bool start)
{
	if((_recording != start) && (_source != NULL)) {
		if(_recording) {
			QString filename = QDate::currentDate().toString("yyyyMMdd") + '_' + QTime::currentTime().toString("hhmmss");
			_source->startRawRecording(_rawDir + QString('/') + filename);
		} else {
			_source->stopRawRecording();
		}
	}

	_recording = start;
}

void TGpsSource::slotNmeaPlay(const QString &filename)
{
	if(_source != NULL) {
		disconnect(_source, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));
	}

	connect(&_nmeaSource, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));

	_nmeaSource.setFile(filename);
	_nmeaSource.start();
}

void TGpsSource::slotNmeaStop()
{
	disconnect(&_nmeaSource, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));

	if(_source != NULL) {
		connect(_source, SIGNAL(signalUpdate(TGpsSample)), this, SLOT(slotUpdate(TGpsSample)));
	}

	_nmeaSource.stop();
}

void TGpsSource::slotUpdate(TGpsSample sample)
{
	emit signalUpdate(sample);
}

