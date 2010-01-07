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

#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include <QObject>
#include <QPainter>
#include <QFont>
#include <QTime>

#include "include/drawstate.h"

class TMessageBoard : public QObject
{
	Q_OBJECT
	public:
		TMessageBoard();
		~TMessageBoard();

		void draw(QPainter &painter, TDrawState &drawState);
		void message(const QString &message, void *sender, int timeout);
		
	public slots:
		void slotTimer();

	signals:
		void signalRefresh();

	private:
		QMutex _mutex;
		QFont _msgFont;

		QStringList _messages;
		QList<void *> _senders;
		QList<QTime> _times;
};

#endif
