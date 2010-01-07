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
#include <QMessageBox>
#include <QFile>
#include <QUrl>

#include "include/tilehttpmux.h"
#include "include/tilehttpname.h"

TTileHttpMux::TTileHttpMux()
{
}

TTileHttpMux::~TTileHttpMux()
{
	int i;

	for(i = 0; i < _wheels.size(); i++) {
		delete _wheels[i];
	}
}

void TTileHttpMux::configure(TSettings &settings, const QStringList &sections)
{
	int i, j;
	for(i = 0; i < sections.size(); i++) {
		settings.beginGroup(sections[i]);
		QStringList types = settings.getValue("servers", "road").toStringList();
		settings.endGroup();	

		for(j = 0; j < types.size(); j++) {
			TTileHttpWheel *wheel = new TTileHttpWheel;
			wheel->configure(settings, sections[i], types[j]);
			connect(wheel, SIGNAL(signalNewTile(TTileHttpTrans *)), this, SLOT(slotNewTile(TTileHttpTrans *)));
			_wheels.append(wheel);

			QString name = TTileHttpName::createName(sections[i], types[j]);
			_serverNames << name;
		}
	}
}

void TTileHttpMux::load(TTileHttpTrans *trans)
{
	int i;
	for(i = 0; (i < _wheels.size()) && (_wheels[i]->name() != trans->ref().server()); i++);

	if(i == _wheels.size()) {
		qDebug() << "Error: access to unknown http server, " << trans->ref().server();
	} else {
		_wheels[i]->load(trans);
	}
}

void TTileHttpMux::slotNewTile(TTileHttpTrans *trans)
{
	emit signalNewTile(trans);
}

