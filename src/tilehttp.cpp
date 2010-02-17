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

#include "include/tilehttp.h"
#include "include/converter.h"

TTileHttp::TTileHttp()
{
	connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(newLoad(int, bool)));
}

TTileHttp::~TTileHttp()
{
}

void TTileHttp::setServer(TSettings &settings, const QString &section, const QString &serverURL)
{
	QString hostname;
	int pos;

	settings.beginGroup(section);
	_template = settings.getValue(serverURL, "http://tile.openstreetmap.org/%3/%1/%2.png").toString();
	_rootLevel = settings.getValue("rootlevel", 0).toInt();
	_equatorCentric = settings.getValue("equatorcentric", false).toBool();
	_snailMode = settings.getValue("snailmode", false).toBool();
	settings.endGroup();

	hostname = _template;

	pos = hostname.indexOf("//");
	if(pos != -1) {
		hostname = hostname.remove(0, pos + 2);
	}

	pos = hostname.indexOf('/');
	if(pos != -1) {
		hostname.truncate(pos);
	}

	setHost(hostname);
}

QString TTileHttp::getURL(const TTileRef &ref)
{
	int x = ref.x();
	int y = ref.y();
	int zoom;

	if(_rootLevel > 10) {
		zoom = _rootLevel - ref.zoom();
	} else {
		zoom = _rootLevel + ref.zoom();
	}

	if(_equatorCentric) {
		int numTiles = (1 << (_rootLevel - zoom));
		y = (numTiles / 2) - y - 1;
	}

	QString url;
	if(_snailMode) {
		QString snail = "";

		int i;
		for(i = zoom - 1; i >= 0; i--) {
			int pos = ((x >> i) & 0x1) + (((y >> i) & 0x1) << 1);
			snail += (char)((int)'0' + pos);
		}

		url = _template.arg(snail);
	} else {
		url = _template.arg(QString::number(x)).arg(QString::number(y)).arg(QString::number(zoom));
	}

	return url;
}

void TTileHttp::load(TTileHttpTrans *trans)
{
	QMutexLocker locker(&_mutex);

	trans->setHttpId(get(getURL(trans->ref()), &trans->buffer()));
	_transactions.prepend(trans);
}

void TTileHttp::newLoad(int id, bool error)
{
	TTileHttpTrans *trans = NULL;

	_mutex.lock();

	int i;
	for(i = 0; (i < _transactions.size()) && (_transactions[i]->httpId() != id); i++);
	if(i != _transactions.size()) {
		trans = _transactions.takeAt(i);
	}

	_mutex.unlock();

	if(trans != NULL) {
		if (error) {
			qDebug() << errorString();
		} else if(checkSignature(trans->buffer().data())) {
			trans->setCompressed(new TTileCompressed(trans->buffer().data()));
		} else {
			qDebug() << "Received data aren't a valid image format: " << trans->ref().x() << trans->ref().y() << trans->ref().zoom();
		}
		emit signalNewTile(trans);
	}
}

bool TTileHttp::checkSignature(const QByteArray &data)
{
	return checkPNG(data) || checkJPG(data);
}

bool TTileHttp::checkPNG(const QByteArray &data)
{
	static QByteArray pngSignature;
	static bool initialized = false;

	if(!initialized) {
		pngSignature += (char)0x89;
		pngSignature += (char)0x50;
		pngSignature += (char)0x4e;
		pngSignature += (char)0x47;
		pngSignature += (char)0x0d;
		pngSignature += (char)0x0a;
		pngSignature += (char)0x1a;
		pngSignature += (char)0x0a;
		initialized = true;
	}

	QByteArray signature;

	int i;
	for(i = 0; i < 8; i++) {
		signature += data[i];
	}

	return signature == pngSignature;
}

bool TTileHttp::checkJPG(const QByteArray &data)
{
	static QByteArray jpgSignature;
	static bool initialized = false;

	if(!initialized) {
		jpgSignature += (char)0xff;
		jpgSignature += (char)0xd8;
		jpgSignature += (char)0xff;
		jpgSignature += (char)0xe0;
		jpgSignature += (char)0x4a;
		jpgSignature += (char)0x46;
		jpgSignature += (char)0x49;
		jpgSignature += (char)0x46;
		jpgSignature += (char)0x00;
		initialized = true;
	}

	QByteArray signature;

	int i;
	for(i = 0; i < 11; i++) {
		if((i != 4) && (i != 5)) {
			signature += data[i];
		}
	}

	return signature == jpgSignature;
}


