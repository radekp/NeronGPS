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

#include "include/gpssourcegpsd.h"

TGpsSourceGpsd::TGpsSourceGpsd(const QString &logFile)
{
	_log = new QFile(logFile);

	if(!_log->open(QIODevice::WriteOnly)) {
		qDebug() << "Error opening gpsd log file to write: " << logFile;
		delete _log;
		_log = NULL;
	}
}

TGpsSourceGpsd::TGpsSourceGpsd()
{
	_log = NULL;
}

TGpsSourceGpsd::~TGpsSourceGpsd()
{
	if(_log != NULL) {
		delete _log;
	}
}

void TGpsSourceGpsd::start()
{
	connect(&_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

	_socket.connectToHost("127.0.0.1", 2947);

	if(_socket.waitForConnected(10000)) {
		qDebug() << "Connected to gpsd";
		_socket.write(QByteArray("r"));
	} else {
		qDebug() << "Connection to gpsd failed";
	}
}

void TGpsSourceGpsd::slotReadyRead()
{
	while(_socket.canReadLine()) {
		QString line = QString(_socket.readLine());

		if(_log != NULL) {
			_log->write(line.toAscii());
		}

		_parser.parse(line);
		if(_parser.sampleReady()) {
			emit signalUpdate(_parser.retrieveSample());
		}		
	}
}

