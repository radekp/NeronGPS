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
#include <QWhereaboutsFactory>
#include <QNmeaWhereabouts>

#include "qtmoko/gpssourceabout.h"

TGpsSourceAbout::TGpsSourceAbout(const QString &fileName)
{
        _file = new QFile(fileName);
        _file->open(QIODevice::ReadOnly);

        QNmeaWhereabouts *temp = new QNmeaWhereabouts();
        temp->setUpdateMode(QNmeaWhereabouts::SimulationMode);
        temp->setSourceDevice(_file);

	_about = temp;
}

TGpsSourceAbout::TGpsSourceAbout()
{
	_about = QWhereaboutsFactory::create();
	_file = NULL;
}

TGpsSourceAbout::~TGpsSourceAbout()
{
	delete _about;

	if(_file != NULL) {
		delete _file;
	}
}

void TGpsSourceAbout::start()
{
	connect(_about, SIGNAL(updated(const QWhereaboutsUpdate &)), this, SLOT(slotUpdate(const QWhereaboutsUpdate &)));

	_about->startUpdates(1000); 
}

void TGpsSourceAbout::slotUpdate(const QWhereaboutsUpdate &update)
{
	TGpsSample sample(update.updateDateTime(), update.coordinate().latitude(), update.coordinate().longitude());

	if(update.coordinate().type() == QWhereaboutsCoordinate::Coordinate3D) {
		sample.setAltitude(update.coordinate().altitude());
	}

	if(update.dataValidityFlags() & QWhereaboutsUpdate::GroundSpeed) {
		sample.setSpeed(update.groundSpeed());
	}

	emit signalUpdate(sample);
}




