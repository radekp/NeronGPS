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

#include "include/gpxloader.h"
#include "include/global.h"

TGpxLoader::TGpxLoader()
{
}

TGpxLoader::~TGpxLoader()
{
}

void TGpxLoader::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);

	_pen.setWidth(settings.getValue("tracethickness", 10).toInt());
	_pen.setCapStyle(Qt::RoundCap);

	_transparency = settings.getValue("tracetransparency", 192).toInt();
	_point = settings.getValue("tracepoint", 400).toInt();

	settings.endGroup();
}

void TGpxLoader::slotLoad(QString gpx, QColor color)
{
	QMutexLocker locker(&_mutex);

	_names += gpx;
	_colors += color;

	start();
}

void TGpxLoader::run()
{
	_mutex.lock();

	while(_names.size() > 0) {
		QString name = _names.takeFirst();
		QColor color = _colors.takeFirst();
		_mutex.unlock();

		QFile file(name);
		if (file.open(QIODevice::ReadOnly)) {
			QXmlStreamReader xml;
			xml.setDevice(&file);

			QList<TTrace *> *traces = new QList<TTrace *>;
			while (!xml.atEnd()) {
				if((xml.readNext() == QXmlStreamReader::StartElement) &&
					(xml.name().toString() == QString("trk"))) {
					*traces += loadTrace(xml, name, color);
				}
			}
			
			if (xml.hasError()) {
				qDebug() << "Gpx loading error (" << name << "):" << xml.errorString();
			}

			if(traces->size() > 0) {
				emit signalLoaded(traces);
			} else {
				qDebug() << "Warning, no track found in: " << name;
			}
		} else {
			qDebug() << "Error opening gpx file: " << name;
		}

		_mutex.lock();
	}

	_mutex.unlock();
}

TTrace *TGpxLoader::loadTrace(QXmlStreamReader &xml, const QString &name, const QColor &color)
{
	TTrace *trace = new TTrace;

	QColor temp = color;
	temp.setAlpha(_transparency);
	_pen.setColor(color);
	trace->setPen(_pen, _point);

	int samples = 0;
	int count = 0;
	tMessage((void *)this) << "Loading...";

	bool end = false;
	while (!end) {
		if(xml.atEnd()) {
			qDebug() << "Gpx parsing error (" << name << "): end of file found before end of track";
			end = true;
		} else {
			QXmlStreamReader::TokenType type = xml.readNext();
			
			if((type == QXmlStreamReader::EndElement) && (xml.name().toString() == QString("trkseg"))) {
				end = true;
			}

			if((type == QXmlStreamReader::StartElement) && (xml.name().toString() == QString("trkpt"))) {
				QXmlStreamAttributes attributes = xml.attributes();

				double lat = attributes.value(QString("lat")).toString().toDouble();
				double lon = attributes.value(QString("lon")).toString().toDouble();

				int x = TConverter::prepareX(lon);
				int y = TConverter::prepareY(lat);

				trace->addSample(x, y);

				samples ++;
				count ++;
				if(count == 500) {
					tMessage((void *)this) << "Loading: " << samples;
					count = 0;
				}
			}
		}
	}

	tMessage((void *)this) << "";

	return trace;
}


