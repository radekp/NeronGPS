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

#include "include/tilel3cachemux.h"
#include "include/settings.h"

TTileL3CacheMux::TTileL3CacheMux()
{
}

TTileL3CacheMux::~TTileL3CacheMux()
{
	QMutexLocker locker(&_mutex);

	if(_processes.size() > 0) {
		QString str = "There is a cache directory deletion in progress. Do you want to wait for completion before to exit?";
		QMessageBox dialog(QMessageBox::Warning, "Warning", str, QMessageBox::Yes | QMessageBox::No);

		if(dialog.exec() == QMessageBox::Yes) {
			while(_processes.size() > 0) {
				QProcess *process = _processes.takeFirst();
				if(process->state() != QProcess::NotRunning) {
					process->waitForFinished(600000);
				}
				delete process;
			}
		} else {
			while(_processes.size() > 0) {
				delete _processes.takeFirst();
			}
		}
	}
}

void TTileL3CacheMux::configure(TSettings &settings, const QString &section)
{
	settings.beginGroup(section);
	QString cacheDir = settings.getValue("cachedir", "/media/card/NeronGPS/maps").toString();
	QString tempCache = settings.getValue("tempcachedir", "/media/card/NeronGPS/temp").toString();
	settings.endGroup();

	_dir = cacheDir;
	_tempName = tempCache;

	QDir::current().mkpath(_dir);
	QDir::current().mkpath(_tempName);

	QString empty;
	_loader.configure(empty, tempCache);
	
	QDir dir(cacheDir);
	QStringList filters;
	dir.setSorting(QDir::Name);
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	filters << "*";

	QStringList names = dir.entryList(filters);

	int i;
	for(i = 0; i < names.size(); i++) {
		addCache(names[i]);
	}
}

bool TTileL3CacheMux::addCache(const QString &name)
{
	bool ret = false;

	QMutexLocker locker(&_mutex);

	QString dir = _dir + '/' + name;

	if(dir != _tempName) {
		qDebug() << "Add cache: " << dir;
		TTileL3Cache *cache = new TTileL3Cache();
		cache->configure(name, dir);
		_caches.append(cache);
		_cacheNames.append(name);
		_cacheNames.sort();
		ret = true;
	}

	return ret;
}

bool TTileL3CacheMux::addCache()
{
	bool ret = false;

	QDir dir(_dir);
	QString name;

	bool end = false;
	int i;
	for(i = 0; (i < 10000) && !end; i++) {
		name = QString("%1").arg(i, 4, 10, QChar('0'));
		end = !dir.exists(name);
	}

	if(i < 10000) {
		dir.mkpath(_dir + '/' + name);
		ret = addCache(name);
	} else {
		qDebug() << "Error, exceeded maximum of 10000 cache directories";
	}

	return ret;
}

bool TTileL3CacheMux::deleteCache(const QString &name)
{
	bool ret = false;

	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; (i < _caches.size()) && (_caches[i]->name() != name); i++);

	if(i < _caches.size()) {
		QString str = "You are about to delete the following directory: " + _dir + '/' + name + ".\nAre you sure?";
		QMessageBox dialog(QMessageBox::Warning, "Warning", str, QMessageBox::Ok | QMessageBox::Cancel);

		if(dialog.exec() == QMessageBox::Ok) {
			QProcess *process = new QProcess;
			_processes.append(process);
			connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));

			QStringList args;
			args << "-R";
			args << QString(_dir + '/' + name);
			process->start("rm", args, QIODevice::NotOpen);

			delete _caches.takeAt(i);
			_cacheNames.removeAll(name);

			ret = true;
		}
	} else {
		qDebug() << "Error, attempt to delete an unknwon cache";
	}

	return ret;
}

void TTileL3CacheMux::slotFinished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _processes.size(); i++) {
		if(_processes[i]->state() == QProcess::NotRunning) {
			delete _processes.takeAt(i);
			i--;
		}
	}
}

bool TTileL3CacheMux::checkTile(const QString &cacheName, const TTileRef &ref)
{
	bool ret = false;

	QMutexLocker locker(&_mutex);

	if(cacheName == "") {
		ret = _loader.checkTile(ref);
	} else {
		int i;
		for(i = 0; (i < _caches.size()) && (_caches[i]->name() != cacheName); i++);

		if(i != _caches.size()) {
			ret = _caches[i]->checkTile(ref);
		} else {
			qDebug() << "Error, attempt to check an unknow cache directory: " << cacheName;
		}
	}

	return ret;
}

void TTileL3CacheMux::addTile(TTileCompressed *tile, QString cacheName)
{
	QMutexLocker locker(&_mutex);

	if(cacheName == "") {
		_loader.addTile(tile);
	} else {
		int i;
		for(i = 0; (i < _caches.size()) && (_caches[i]->name() != cacheName); i++);

		if(i != _caches.size()) {
			_caches[i]->addTile(tile);
		} else {
			qDebug() << "Error, attempt to write an unknow cache directory: " << cacheName;
		}
	}
}

TTileCompressed *TTileL3CacheMux::getTile(const TTileRef &ref)
{
	QMutexLocker locker(&_mutex);

	TTileCompressed *tile = NULL;

	int i;
	for(i = 0; (tile == NULL) && (i < _caches.size()); i++) {
		tile = _caches[i]->getTile(ref);
	}

	if(tile == NULL) {
		tile = _loader.getTile(ref);
	}

	return tile;
}

