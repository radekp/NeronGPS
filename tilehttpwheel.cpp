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

#include "include/tilehttpwheel.h"
#include "include/tilehttpname.h"

TTileHttpWheel::TTileHttpWheel()
{
	_next = 0;
}

TTileHttpWheel::~TTileHttpWheel()
{
	int i;

	for(i = 0; i < _servers.size(); i++) {
		delete _servers[i];
	}
}

void TTileHttpWheel::configure(TSettings &settings, const QString &section, const QString type)
{
	_name = TTileHttpName::createName(section, type);

	qDebug() << "Add http server: " << _name;

	bool contains = true;
	char i;
	for(i = '1'; contains; i++) {
		settings.beginGroup(section);
		contains = settings.contains(type + i);
		settings.endGroup();	

		if(contains) {
			TTileHttp *tileHttp;
			tileHttp = new TTileHttp();
			tileHttp->setServer(settings, section, type + i);
			connect(tileHttp, SIGNAL(signalNewTile(TTileHttpTrans *)), this, SLOT(slotNewTile(TTileHttpTrans *)));
			_servers.append(tileHttp);
		}
	}
}

void TTileHttpWheel::load(TTileHttpTrans *trans)
{
	if(_servers.size() != 0) {
		_servers[_next]->load(trans);
		_next ++;
		if(_next >= _servers.size()) {
			_next = 0;
		}
	}
}

void TTileHttpWheel::slotNewTile(TTileHttpTrans *trans)
{
	emit signalNewTile(trans);
}


