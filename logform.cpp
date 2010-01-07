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

#include "include/logform.h"
#include "include/log.h"

TLogForm::TLogForm(TLog &log, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	QMutexLocker locker(&_mutex);

	ui.log->clear();
	ui.log->addItems(log);
	
	connect(&log, SIGNAL(signalLog(QString)), this, SLOT(slotLog(QString)));
	connect(&log, SIGNAL(signalClear()), this, SLOT(slotCleared()));
	connect(this, SIGNAL(signalClear()), &log, SLOT(slotClear()));

	connect(ui.clear, SIGNAL(clicked (bool)), this, SLOT(slotClear(bool)));
}

TLogForm::~TLogForm()
{
}

void TLogForm::slotClear(bool /*checked*/)
{
	emit signalClear();
}

void TLogForm::slotLog(QString log)
{
	ui.log->addItem(log);
}

void TLogForm::slotCleared()
{
	ui.log->clear();
}


