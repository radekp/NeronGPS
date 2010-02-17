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

#include <QTimer>

#include "include/messageboard.h"

TMessageBoard::TMessageBoard()
{
	_msgFont.setPixelSize(24);
}

TMessageBoard::~TMessageBoard()
{
}

void TMessageBoard::draw(QPainter &painter, TDrawState &drawState)
{
	if((_messages.size() != 0) && (!drawState.moving())) {
		QMutexLocker locker(&_mutex);

		QFontMetrics metric(_msgFont);
		QBrush brush(QColor(0, 0, 0, 128));

		QPen pen = painter.pen();

		painter.setPen(Qt::white);
		painter.setFont(_msgFont);

		int bannerSize = metric.height() + 2;
		int startPos = drawState.height() - 2 * bannerSize;

		int i;
		for(i = 0; i < _messages.size(); i++) {
			int bannerPos = startPos - i * (bannerSize + 1);

			painter.fillRect(0, bannerPos, drawState.width(), bannerSize, brush);
		
			int x = drawState.width() / 2 - metric.width(_messages[i]) / 2;
			int y = bannerPos + bannerSize - metric.descent() - 1;
			painter.drawText(x, y, _messages[i]);
		}
	
		painter.setPen(pen);
	}
}

void TMessageBoard::message(const QString &message, void *sender, int timeout)
{
	QMutexLocker locker(&_mutex);

	int i = _senders.indexOf(sender);

	if(i != -1) {
		if(message == "") {
			_senders.takeAt(i);
			_messages.takeAt(i);
			_times.takeAt(i);
		} else {
			_messages.replace(i, message);
			if(timeout == -1) {
				_times.replace(i, QTime());
			} else {
				_times.replace(i, QTime::currentTime().addMSecs(timeout));
				 QTimer::singleShot(timeout, this, SLOT(slotTimer()));
			}
		}
	} else {
		if(message != "") {
			_senders.append(sender);
			_messages.append(message);
			if(timeout == -1) {
				_times.append(QTime());
			} else {
				_times.append(QTime::currentTime().addMSecs(timeout));
				 QTimer::singleShot(timeout, this, SLOT(slotTimer()));
			}
		}
	}

	emit signalRefresh();
}

void TMessageBoard::slotTimer()
{
	QMutexLocker locker(&_mutex);

	int i;
	for(i = 0; i < _times.size(); i++) {
		if( !_times[i].isNull() && (_times[i] < QTime::currentTime())) {
			_senders.takeAt(i);
			_messages.takeAt(i);
			_times.takeAt(i);
			i--;		
		}
	}

	emit signalRefresh();
}

