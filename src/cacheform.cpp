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

#include "include/cacheform.h"
#include "include/tilehttpname.h"

TCacheForm::TCacheForm(const QString &cacheDir, const QStringList &cacheList, const QStringList &serverList, const QString &cache, int beginZoom, int endZoom, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	if(cache != "") {
		QString txt = cache + " - " + beginZoom + "/" + endZoom;
		ui.currentCache->setText(txt);
	}

	ui.cacheList->clear();
	ui.cacheList->addItems(cacheList);

	ui.serverList->clear();
	ui.serverList->addItems(serverList);

	ui.beginZoom->setMaximum(NUMLEVELS);
	ui.beginZoom->setValue(0);
	ui.bZoomText->setText("0");
	ui.beginZoom->setMaximum(NUMLEVELS);
	ui.endZoom->setValue(NUMLEVELS);
	ui.eZoomText->setText(QString::number(NUMLEVELS));

	_dir.cd(cacheDir);

	connect(ui.beginZoom, SIGNAL(valueChanged(int)), this, SLOT(beginZoomChanged(int)));
	connect(ui.endZoom, SIGNAL(valueChanged(int)), this, SLOT(endZoomChanged(int)));
	connect(ui.newCache, SIGNAL(clicked (bool)), this, SLOT(newCache(bool)));
	connect(ui.deleteCache, SIGNAL(clicked (bool)), this, SLOT(deleteCache(bool)));
	connect(ui.setCache, SIGNAL(clicked (bool)), this, SLOT(setCache(bool)));
	connect(ui.downCache, SIGNAL(clicked (bool)), this, SLOT(downCache(bool)));
	connect(ui.cacheList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(slotCacheChanged(const QString &)));
}

TCacheForm::~TCacheForm()
{
}

void TCacheForm::beginZoomChanged(int value)
{
	if(value > ui.endZoom->value()) {
		ui.beginZoom->setValue(ui.endZoom->value());
	}

	ui.bZoomText->setText(QString::number(ui.beginZoom->value()));
}

void TCacheForm::endZoomChanged(int value)
{
	if(value < ui.beginZoom->value()) {
		ui.endZoom->setValue(ui.beginZoom->value());
	}

	ui.eZoomText->setText(QString::number(ui.endZoom->value()));
}

void TCacheForm::slotNewCacheList(QStringList cacheList)
{
	ui.cacheList->clear();
	ui.cacheList->addItems(cacheList);
}

void TCacheForm::newCache(bool /*checked*/)
{
	emit signalAddCache();
}

void TCacheForm::deleteCache(bool /*checked*/)
{
	QList<QListWidgetItem *> selectedCache = ui.cacheList->selectedItems();

	if(selectedCache.size() != 0) {
		emit signalDeleteCache(selectedCache[0]->text());
	}
}

void TCacheForm::setCache(bool /*checked*/)
{
	QList<QListWidgetItem *> selectedCache = ui.cacheList->selectedItems();
	QList<QListWidgetItem *> selectedServers = ui.serverList->selectedItems();

	if(!selectedCache.isEmpty() && !selectedServers.isEmpty()) {
		QStringList servers;
		int i;

		for(i = 0; i < selectedServers.size(); i++) {
			servers.append(selectedServers[i]->text());
		}

		bool forceLoad = ui.forceLoad->checkState() == Qt::Checked;
		emit signalNewLoadingCache(selectedCache[0]->text(), ui.beginZoom->value(), ui.endZoom->value(), forceLoad, servers);

		QString txt;
		if(forceLoad) {
			txt = "^ ";
		}
		txt += selectedCache[0]->text();
		txt += " - ";
		txt += QString::number(ui.beginZoom->value());
		txt += "/";
		txt += QString::number(ui.endZoom->value());
		if(forceLoad) {
			txt += " ^";
		}
		ui.currentCache->setText(txt);
	}
}

void TCacheForm::downCache(bool /*checked*/)
{
	emit signalNewLoadingCache("", 0, 20, false, QStringList());

	ui.currentCache->setText("No cache");
}

void TCacheForm::slotCacheChanged(const QString &name)
{
	QDir dir = _dir;

	dir.cd(name);

	QStringList filters;
	filters << "*";
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringList servers = dir.entryList(filters);

	int i, j;
	for(i = ui.serverList->count() - 1; i >= 0 ; i--) {
		bool selected = false;
		for(j = 0; (j < servers.size()) && !selected; j++) {
			QString str = TTileHttpName::convertFromCacheName(servers[j]);

			selected = ui.serverList->item(i)->text() == str;
		}

		if(selected) {
			ui.serverList->setCurrentRow(i, QItemSelectionModel::Select);
		} else {
			ui.serverList->setCurrentRow(i, QItemSelectionModel::Deselect);
		}
	}

	int min = NUMLEVELS;
	int max = 0;
	for(i = 0; i < servers.size(); i++) {
		QDir tmpDir = dir;

		tmpDir.cd(servers[i]);

		tmpDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		QStringList levels = tmpDir.entryList(filters);

		for(j = 0; j < levels.size(); j++) {
			bool ok;
			int level = levels[j].toInt(&ok);
			if(ok) {
				if(level > max) {
					max = level;
				} else if(level < min) {
					min = level;
				}
			}
		}
	}

	if(min > max) {
		min = 0;
		max = NUMLEVELS;
	}

	ui.beginZoom->setValue(min);
	ui.bZoomText->setText(QString::number(min));
	ui.endZoom->setValue(max);
	ui.eZoomText->setText(QString::number(max));
	
}


