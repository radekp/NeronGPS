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

#include "include/poiserver.h"
#include "include/settings.h"
#include "include/converter.h"

TPoiServer::TPoiServer()
{
	_poiFile = NULL;
}

TPoiServer::~TPoiServer()
{
	if(_poiFile != NULL) {
		delete _poiFile;
	}
}

void TPoiServer::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_poiDirName = settings.getValue("poidir", "/media/card/NeronGPS/poi").toString();
	QDir::current().mkpath(_poiDirName);

	_poiFileName = settings.getValue("poifile", "poilist").toString();

	settings.endGroup();

	QMutexLocker locker(&_mutex);

	_poiFile = new QFile(_poiDirName + '/' + _poiFileName);

	if(_poiFile->open(QIODevice::ReadOnly)) {
		while(!_poiFile->atEnd()) {
			_poiList += QString(_poiFile->readLine()).remove('\n');
		}
		_poiFile->close();
	}
}

void TPoiServer::slotRegisterPoi(QString name, QString coordinates)
{
	QMutexLocker locker(&_mutex);

	if(_poiFile == NULL) {
		return;
	}

	_poiList += name + ": " + coordinates;

	writeFile();
}

void TPoiServer::slotDeletePoi(QString poi)
{
	int index = _poiList.indexOf(poi);

	if(index != -1) {
		_poiList.removeAt(index);
	} else {
		qDebug() << "Error, attempt to suppress on unknwon poi: " + poi;
	}

	writeFile();
}

void TPoiServer::writeFile()
{
	if(_poiFile->open(QIODevice::WriteOnly)) {
		int i;
		for(i = 0; i < _poiList.size(); i++) {
			_poiFile->write((_poiList[i] + '\n').toAscii());
		}
		_poiFile->close();
	} else {
		qDebug() << "Error, impossible to open poi file to write: " + _poiDirName + '/' + _poiFileName;
	}
}



