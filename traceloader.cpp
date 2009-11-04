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

#include "include/traceloader.h"

TTraceLoader::TTraceLoader()
{
	_file = NULL;
}

TTraceLoader::~TTraceLoader()
{
	if(_file != NULL) {
		delete _file;
	}
}

bool TTraceLoader::setFile(const QString &filename)
{
	bool success;

	if(_file != NULL) {
		delete _file;
	}

	_file = new QFile(filename);

	success = _file->open(QIODevice::ReadOnly);
	if (!success) {
		qDebug() << "Error opening file: " << filename;
		delete _file;
	}
	
	return success;
}

void TTraceLoader::run()
{
	while(!_file->atEnd())
	{
		TTraceSegment *segment = new TTraceSegment;
		segment->readSamples(_file);
		emit sendSegment(segment);
	}

	delete _file;
	_file = NULL;
	emit sendSegment(NULL);
}

