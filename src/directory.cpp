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

#include "include/directory.h"

TDirectory::TDirectory()
{
	_dir = NULL;
}

TDirectory::~TDirectory()
{
	if(_dir != NULL) {
		delete _dir;
	}
}

void TDirectory::setDir(const QString &path)
{
	if(_dir != NULL) {
		delete _dir;
	}

	_dir = new QDir(path);
}

const QStringList &TDirectory::loadFiles(const QString &filter, bool files, bool reverse)
{
	if(_dir != NULL) {
		QStringList filters;
	
		if(reverse) {
			_dir->setSorting(QDir::Name | QDir::Reversed);
		} else {
			_dir->setSorting(QDir::Name);
		}

		if(files) {
			_dir->setFilter(QDir::Files);
		} else {
			_dir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		}

		filters << filter;
		_list = _dir->entryList(filters);
	}	

	return _list;
}

