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
#include <QTimer>

#include "include/gpssourcenmea.h"

TGpsSourceNmea::TGpsSourceNmea()
{
	_file = NULL;
	_timer.setSingleShot(true);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

TGpsSourceNmea::~TGpsSourceNmea()
{
	_timer.stop();

	if(_file != NULL) {
		delete _file;
	}
}

void TGpsSourceNmea::setFile(const QString &fileName)
{
	QMutexLocker locker(&_mutex);

	if(_file != NULL) {
		delete _file;
	}

        _file = new QFile(fileName);
        if(!_file->open(QIODevice::ReadOnly)) {
		_file = NULL;
		qDebug() << "Error opening file to read: " << fileName;
	}
}

void TGpsSourceNmea::start()
{
	if(_file != NULL) {
		parse();
		if(_parser.sampleReady()) {
			_sample = _parser.retrieveSample();
			_lastTime = _sample.time();
			slotTimer();
		}
	}
}

void TGpsSourceNmea::stop()
{
	QMutexLocker locker(&_mutex);

	_timer.stop();

	if(_file != NULL) {
		delete _file;
		_file = NULL;
	}
}

void TGpsSourceNmea::slotTimer()
{
	QMutexLocker locker(&_mutex);

	if(_file == NULL) {
		return;
	}

	emit signalUpdate(_sample);

	parse();
	if(_parser.sampleReady()) {
		_sample = _parser.retrieveSample();
		_timer.start(interval(_lastTime, _sample.time()));
		_lastTime = _sample.time();
	}
}

void TGpsSourceNmea::parse()
{
	while(!_file->atEnd() && !_parser.parse(QString(_file->readLine())));
}

int TGpsSourceNmea::interval(QDateTime first, QDateTime next)
{
	int interval;

	if(first.date() == next.date()) {
		interval = first.time().msecsTo(next.time());
	} else {
		interval = 1000 * first.secsTo(next);
	}

	return interval;
}


