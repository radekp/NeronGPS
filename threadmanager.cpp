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

#include "include/threadmanager.h"

TThreadManager::TThreadManager()
{
}

TThreadManager::~TThreadManager()
{
	while(_threads.count() > 0) {
		TThread *thread = _threads.takeFirst();
		thread->wait();
		delete thread;
	}
}

void TThreadManager::addThread(TThread *thread)
{
	QMutexLocker locker(&_mutex);

	if(!_threads.contains(thread)) {
		connect(thread, SIGNAL(signalFinished(TThread *)), this, SLOT(slotFinished(TThread *)));
		_threads += thread;
		thread->start();
	} else {
		qDebug() << "Error, attempt to queue same thread twice";
	}
}

void TThreadManager::slotFinished(TThread *thread)
{
	QMutexLocker locker(&_mutex);

	int removed = _threads.removeAll(thread);

	if(removed <= 0) {
		qDebug() << "Error, unknown thread finished";
	} else if(removed > 1) {
		qDebug() << "Error, several instance of same thread queued";
	}

	delete thread;
}

