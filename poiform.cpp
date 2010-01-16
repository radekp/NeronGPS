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
#include <QMessageBox>

#include "include/poiform.h"
#include "include/keyboardform.h"
#include "include/converter.h"

TPoiForm::TPoiForm(const QStringList &poiList, int x, int y, const QStringList &keyboard, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	_keyboard = keyboard;

	QStringList tmpList;

	int i;
	for(i = 0; i < poiList.size(); i++) {
		QString tmpStr = poiList[i];

		tmpStr.replace(": ", "\n");
		tmpList += tmpStr;
	}

	ui.poiList->addItems(tmpList);
	for(i = 0; i < ui.poiList->count(); i++) {
		ui.poiList->item(i)->setTextAlignment(Qt::AlignCenter); 
	}

	ui.poiList->sortItems();

	ui.coordinates->setText(TConverter::latitudeToString(TConverter::unprepareY(y)) + ", " + TConverter::longitudeToString(TConverter::unprepareX(x)));

	connect(ui.add, SIGNAL(clicked(bool)), this, SLOT(slotAdd(bool)));
	connect(ui.del, SIGNAL(clicked(bool)), this, SLOT(slotDelete(bool)));
	connect(ui.go, SIGNAL(clicked(bool)), this, SLOT(slotGoTo(bool)));
	connect(ui.drive, SIGNAL(clicked(bool)), this, SLOT(slotDriveTo(bool)));
	connect(ui.poiList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
}

TPoiForm::~TPoiForm()
{
}

void TPoiForm::slotItemClicked(QListWidgetItem *item)
{
	ui.coordinates->setText(item->text().section('\n', 1, 1));
}

void TPoiForm::slotAdd(bool /*checked*/)
{
	TKeyboardForm *keyboardForm = new TKeyboardForm(_keyboard, "Save POI?");

	connect(keyboardForm, SIGNAL(signalText(QString)), this, SLOT(slotPoiName(QString)));

	keyboardForm->setWindowState(Qt::WindowMaximized);
	keyboardForm->show();
}

void TPoiForm::slotPoiName(QString name)
{
	ui.poiList->addItem(name + '\n' + ui.coordinates->text());

	ui.poiList->item(ui.poiList->count() - 1)->setTextAlignment(Qt::AlignCenter); 

	ui.poiList->sortItems();

	emit signalPoi(name, ui.coordinates->text());
}

void TPoiForm::slotDelete(bool /*checked*/)
{
	QListWidgetItem *item = ui.poiList->currentItem();

	if(item != NULL) {
		QString poi = item->text();
		poi.replace("\n", ": ");

		QString str = "You are about to delete the following POI:\n\n";
		str += poi;
		str += "\n\nAre you sure?";

		QMessageBox dialog(QMessageBox::Warning, "Warning", str, QMessageBox::Ok | QMessageBox::Cancel);
		if(dialog.exec() == QMessageBox::Ok) {
			signalDeletePoi(poi);
			delete ui.poiList->takeItem(ui.poiList->row(item));
		}
	}
}

void TPoiForm::slotGoTo(bool /*checked*/)
{
	QStringList split = ui.coordinates->text().split(", ");

	if(split.size() == 2) {
		int x = TConverter::prepareX(TConverter::longitudeFromString(split[1]));
		int y = TConverter::prepareY(TConverter::latitudeFromString(split[0]));

		hide();

		emit signalGoTo(x, y);

		QTimer::singleShot(1, this, SLOT(slotKill()));
	}
}

void TPoiForm::slotDriveTo(bool /*checked*/)
{
	QStringList split = ui.coordinates->text().split(", ");

	if(split.size() == 2) {
		int x = TConverter::prepareX(TConverter::longitudeFromString(split[1]));
		int y = TConverter::prepareY(TConverter::latitudeFromString(split[0]));

		hide();

		emit signalDriveTo(x, y);

		QTimer::singleShot(1, this, SLOT(slotKill()));
	}
}

void TPoiForm::slotKill()
{
	delete this;
}

