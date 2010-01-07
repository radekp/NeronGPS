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

#include <QHeaderView>
#include <QPainter>
#include <QMessageBox>

#include "include/gpxform.h"

TGpxForm::TGpxForm(const QString &dir, const QStringList &loaded, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	_dirName = dir;
	_gpxDir.setDir(dir);

	connect(ui.clear, SIGNAL(clicked (bool)), this, SLOT(slotClear(bool)));
	connect(ui.del, SIGNAL(clicked (bool)), this, SLOT(slotDelete(bool)));

	_colors += QColor(128, 0, 0);
	_colors += QColor(0, 128, 0);
	_colors += QColor(0, 0, 128);
	_colors += QColor(128, 128, 0);
	_colors += QColor(128, 0, 128);
	_colors += QColor(0, 128, 128);
	_colors += QColor(0, 0, 0);
	_colors += QColor(128, 128, 128);

	ui.color1->setIcon(colorIcon(_colors[0], ui.color1->iconSize().width(), ui.color1->iconSize().height() / 2));
	ui.color2->setIcon(colorIcon(_colors[1], ui.color2->iconSize().width(), ui.color2->iconSize().height() / 2));
	ui.color3->setIcon(colorIcon(_colors[2], ui.color3->iconSize().width(), ui.color3->iconSize().height() / 2));
	ui.color4->setIcon(colorIcon(_colors[3], ui.color4->iconSize().width(), ui.color4->iconSize().height() / 2));
	ui.color5->setIcon(colorIcon(_colors[4], ui.color5->iconSize().width(), ui.color5->iconSize().height() / 2));
	ui.color6->setIcon(colorIcon(_colors[5], ui.color6->iconSize().width(), ui.color6->iconSize().height() / 2));
	ui.color7->setIcon(colorIcon(_colors[6], ui.color7->iconSize().width(), ui.color7->iconSize().height() / 2));
	ui.color8->setIcon(colorIcon(_colors[7], ui.color8->iconSize().width(), ui.color8->iconSize().height() / 2));

	connect(ui.color1, SIGNAL(clicked (bool)), this, SLOT(slotColor1(bool)), Qt::QueuedConnection);
	connect(ui.color2, SIGNAL(clicked (bool)), this, SLOT(slotColor2(bool)), Qt::QueuedConnection);
	connect(ui.color3, SIGNAL(clicked (bool)), this, SLOT(slotColor3(bool)), Qt::QueuedConnection);
	connect(ui.color4, SIGNAL(clicked (bool)), this, SLOT(slotColor4(bool)), Qt::QueuedConnection);
	connect(ui.color5, SIGNAL(clicked (bool)), this, SLOT(slotColor5(bool)), Qt::QueuedConnection);
	connect(ui.color6, SIGNAL(clicked (bool)), this, SLOT(slotColor6(bool)), Qt::QueuedConnection);
	connect(ui.color7, SIGNAL(clicked (bool)), this, SLOT(slotColor7(bool)), Qt::QueuedConnection);
	connect(ui.color8, SIGNAL(clicked (bool)), this, SLOT(slotColor8(bool)), Qt::QueuedConnection);

	refresh();
}

TGpxForm::~TGpxForm()
{
}

void TGpxForm::slotClear(bool /*checked*/)
{
	emit signalClear();
}

void TGpxForm::slotDelete(bool /*checked*/)
{
	QString name = getFile();

	if(!name.isNull()) {
		QFile(name).remove();

		refresh();
	}
}

void TGpxForm::slotColor1(bool /*checked*/) { colorPressed(_colors[0]); }
void TGpxForm::slotColor2(bool /*checked*/) { colorPressed(_colors[1]); }
void TGpxForm::slotColor3(bool /*checked*/) { colorPressed(_colors[2]); }
void TGpxForm::slotColor4(bool /*checked*/) { colorPressed(_colors[3]); }
void TGpxForm::slotColor5(bool /*checked*/) { colorPressed(_colors[4]); }
void TGpxForm::slotColor6(bool /*checked*/) { colorPressed(_colors[5]); }
void TGpxForm::slotColor7(bool /*checked*/) { colorPressed(_colors[6]); }
void TGpxForm::slotColor8(bool /*checked*/) { colorPressed(_colors[7]); }

void TGpxForm::colorPressed(const QColor &color)
{
	QString name = getFile();

	if(!name.isNull()) {
		emit signalGpx(name, color);
		delete this;
	}
}

QString TGpxForm::getFile()
{
	QString name;

	if(ui.tabWidget->tabText(ui.tabWidget->currentIndex()) == QString("Records")) {
		QList<QListWidgetItem *> items = ui.records->selectedItems();

		if(items.count() > 0) {
			name = items[0]->text().section('\n', 0, 0);
		}
	}

	return _dirName + '/' + name + ".gpx";
}

void TGpxForm::refresh()
{
	int i;

	ui.records->clear();

	QStringList fileList = _gpxDir.loadFiles("*.gpx", true, true);
	QStringList entryList;

	for(i = 0; i < fileList.size(); i++) {
		QString str = fileList[i].left(fileList[i].size() - 4) + "\n (";
		
		int size = QFile(_dirName + '/' + fileList[i]).size();
		if(size < 1000L) {
			str += QString::number(size) + " bytes)";
		} else if(size < 1000000L) {
			str += QString::number(size / 1000L) + " KB)";
		} else if(size < 1000000000L) {
			str += QString::number(size / 1000000L) + " MB)";
		} else {
			str += QString::number(size / 1000000000L) + " GB)";
		}

		entryList += str;
	}

	ui.records->addItems(entryList);

	for(i = 0; i < ui.records->count(); i++) {
		ui.records->item(i)->setTextAlignment(Qt::AlignCenter); 
	}
}

QIcon TGpxForm::colorIcon(const QColor &color, int w, int h)
{
	QPixmap pixmap(w, h);

	pixmap.fill(color);

	return QIcon(pixmap);
}


