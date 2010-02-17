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

#include "include/gpxbuilder.h"
#include "include/global.h"

TGpxBuilder::TGpxBuilder(QString outputFile, QStringList &inputFiles)
{
	_outputFile = outputFile;
	_inputFiles = inputFiles;
}

TGpxBuilder::~TGpxBuilder()
{
}

void TGpxBuilder::run()
{
	QFile file(_outputFile);

	if(file.open(QIODevice::WriteOnly)) {
		writeFile(file);

		tUserLog() << "Created trace file: " << _outputFile;
	} else {
		qDebug() << "Error opening trace file: " << _outputFile;
	}
}

void TGpxBuilder::writeFile(QFile &file)
{
	writeHeader(file);

	int i;
	for(i = 0; i < _inputFiles.size(); i++) {
		if(transferFile(file, _inputFiles[i])) {
			if(!QFile::remove(_inputFiles[i])) {
				qDebug() << "Error suppressing file file: " << _inputFiles[i];
			}
		}
	}

	writeFooter(file);
}

bool TGpxBuilder::transferFile(QFile &file, QString &inputName)
{
	bool ret;

	QFile input(inputName);

	if(ret = input.open(QIODevice::ReadOnly)) {
		transferFile(file, input);		
	} else {
		qDebug() << "Error opening input file: " << inputName;
	}

	return ret;
}

void TGpxBuilder::transferFile(QFile &output, QFile &input)
{
	while(!input.atEnd()) {
		output.write(input.read(GPXBUILDER_TRANSFERSIZE));
	}
}

void TGpxBuilder::writeHeader(QFile &file)
{
	QString str;

	str = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	str += QString("<gpx\n");
	str += QString("  version=\"1.1\"\n");
	str += QString("  creator=\"NeronGPS\"\n");
	str += QString("  xmlns=\"http://www.topografix.com/GPX/1/1\"\n");
	str += QString("  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
	str += QString("  xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");

	file.write(str.toAscii());
}

void TGpxBuilder::writeFooter(QFile &file)
{
	file.write(QString("\n</gpx>\n").toAscii());
}

