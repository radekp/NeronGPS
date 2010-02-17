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
#include <QDir>

#include "include/tilel3cache.h"
#include "include/tile.h"
#include "include/tilehttpname.h"

const QString FILE_TEMPLATE = "/%4/%3/%1/%2.png";

TTileL3Cache::TTileL3Cache()
{
}

TTileL3Cache::~TTileL3Cache()
{
}

void TTileL3Cache::configure(const QString &name, const QString &dir)
{
	_name = name;
	_dir = dir;
}

bool TTileL3Cache::checkTile(const TTileRef &ref)
{
	QString filename = buildName(ref);

	QMutexLocker locker(&_mutex);

	return QFile::exists(filename);
}

void TTileL3Cache::addTile(TTileCompressed *tile)
{
	createDir(tile->ref());
	QString filename = buildName(tile->ref());

	QMutexLocker locker(&_mutex);

	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Error opening file for write: " << filename;
	} else {
		file.write(*tile);
	}
}

TTileCompressed *TTileL3Cache::getTile(const TTileRef &ref)
{
	TTileCompressed *tile = NULL;
	QString filename = buildName(ref);

	QMutexLocker locker(&_mutex);

	if(QFile::exists(filename)) {
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly)) {
			qDebug() << "Error opening file for read: " << filename;
		} else {
			tile = new TTileCompressed(file.readAll());
			tile->setRef(ref);
		}
	}

	return tile;
}

void TTileL3Cache::createDir(const TTileRef &ref)
{
	QString x = QString::number(ref.x());
	QString zoom = QString::number(ref.zoom());

	QDir dir(_dir);

	QString serverDir = TTileHttpName::convertToCacheName(ref.server());

	if(!dir.exists(serverDir)) {
		dir.mkdir(serverDir);
	}

	dir.cd(serverDir);
	if(!dir.exists(zoom)) {
		dir.mkdir(zoom);
	}

	dir.cd(zoom);
	if(!dir.exists(x)) {
		dir.mkdir(x);
	}
}

QString TTileL3Cache::buildName(const TTileRef &ref)
{
	QString name = _dir;
	QString serverDir = TTileHttpName::convertToCacheName(ref.server());
	name += FILE_TEMPLATE.arg(QString::number(ref.x())).arg(QString::number(ref.y())).arg(QString::number(ref.zoom())).arg(serverDir);

	return name;
}

