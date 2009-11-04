/*
 * Copyright 2009 Thierry Vuillaume
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

#include "include/traceform.h"

TTraceForm::TTraceForm(const QString &dir, const QStringList &loaded, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	_toBeLoaded = loaded;

	_dirName = dir;
	_traceDir.setDir(dir);

	ui.traceTable->verticalHeader()->hide();
	ui.traceTable->horizontalHeader()->hide();

	_setAll = colorCombo();
	_setAll->setCurrentIndex(-1); 
	ui.traceTable->setRowCount(1);
	ui.traceTable->setItem(0, 0, new QTableWidgetItem(QString("Set all")));
	ui.traceTable->item(0, 0)->setFlags(Qt::ItemIsEnabled);
	ui.traceTable->setCellWidget(0, 1, _setAll);

	connect(ui.traceList, SIGNAL(currentRowChanged(int)), this, SLOT(slotList(int)));
	connect(ui.clear, SIGNAL(clicked (bool)), this, SLOT(slotClear(bool)));
	connect(ui.del, SIGNAL(clicked (bool)), this, SLOT(slotDelete(bool)));
	connect(_setAll, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSetAll(int)));
	connect(ui.load, SIGNAL(clicked (bool)), this, SLOT(slotLoad(bool)), Qt::QueuedConnection);

	QListWidgetItem *item = new QListWidgetItem("All files");
	item->setTextAlignment(Qt::AlignHCenter);
	ui.traceList->addItem(item); 

	setup();
}

void TTraceForm::setup()
{
	QStringList binList = _traceDir.loadFiles("????????_??_??????.bin", true, true);
	QStringList logList = _traceDir.loadFiles("????????_??_??????.log", true, true);

	_traceNames.clear();
	ui.traceTable->setRowCount(1);

	int i;
	for(i = 0; i < binList.size(); i++) {
		QString name = binList[i].section('.', 0, 0);
		if(_toBeLoaded.indexOf(name) == -1) { _traceNames.append(name);	}
	}

	for(i = 0; i < logList.size(); i++) {
		QString name = logList[i].section('.', 0, 0);
		if((_toBeLoaded.indexOf(name) == -1) && (_traceNames.indexOf(name) == -1)) { _traceNames.append(name); }
	}

	_traceNames.sort();

	while(ui.traceList->count() > 1) {
		ui.traceList->takeItem(0);
	}

	QString shortName = "";
	bool empty = true;
	for(i = 0; i < _traceNames.size(); i++) {
		if(_traceNames[i].mid(0, 11) != shortName) {
			shortName = _traceNames[i].mid(0, 11);
			_shortNames.append(shortName);
			QListWidgetItem *item = new QListWidgetItem(expendDate(_traceNames[i].mid(0, 8)) + " - " + expendTime(_traceNames[i].mid(12, 6)));
			item->setTextAlignment(Qt::AlignHCenter);
			ui.traceList->insertItem(0, item); 
			empty = false;
		}
	}

	if(!empty) {
		ui.traceList->setCurrentRow(0);
		slotList(0);
	}
}

TTraceForm::~TTraceForm()
{
}

void TTraceForm::resizeEvent (QResizeEvent */*event*/)
{
	ui.traceTable->setColumnWidth(0, 2 * (ui.traceTable->width() / 3));
	ui.traceTable->setColumnWidth(1, ui.traceTable->width() / 4);
}

void TTraceForm::slotList(int index)
{
	comboToStringList();

	QStringList list;

	if(index == _shortNames.size()) {
		list = _traceNames;
	} else {
		list = _traceNames.filter(_shortNames[_shortNames.size() - index - 1]);
	}
	int i;

	ui.traceTable->setRowCount(list.size() + 1);

	for(i = 0; i < list.size(); i++) {
		ui.traceTable->setItem(i + 1, 0, new QTableWidgetItem(list[i]));
		ui.traceTable->item(i + 1, 0)->setFlags(Qt::ItemIsEnabled);
		ui.traceTable->setCellWidget(i + 1, 1, colorCombo());
	}

	stringListToCombo();
}

void TTraceForm::comboToStringList()
{
	int i;
	for(i = 1; i < ui.traceTable->rowCount(); i++) {
		QString trace = ui.traceTable->item(i, 0)->text();
		QComboBox *combo = (QComboBox *)ui.traceTable->cellWidget(i, 1);
		QString color = combo->itemData(combo->currentIndex()).toString();

		int j;
		for(j = 0; (j < _toBeLoaded.size()) && (_toBeLoaded[j].mid(0, trace.size()) != trace); j++);

		if(j < _toBeLoaded.size()) {
			if(color == "") {
				_toBeLoaded.removeAt(j);
			} else {
				_toBeLoaded.replace(j, trace + ',' + color);
			}
		} else {
			if(color != "") {
				_toBeLoaded.append(trace + ',' + color);
			}
		}
	}
}

void TTraceForm::stringListToCombo()
{
	int i;
	for(i = 0; i < _toBeLoaded.size(); i++) {
		QString trace = _toBeLoaded[i].section(',', 0, -4);
		QString color = _toBeLoaded[i].section(',', -3, -1);

		int j;
		for(j = 1; (j < ui.traceTable->rowCount()) && (ui.traceTable->item(j, 0)->text() != trace); j++);

		if(j < ui.traceTable->rowCount()) {
			QComboBox *combo = (QComboBox *)ui.traceTable->cellWidget(j, 1);
			int index = combo->findData(QVariant(color)); 
			if(index != -1) {
				combo->setCurrentIndex(index);
			} else {
				qDebug() << "Error, unknow color: " << color;
			}
		}
	}
}

void TTraceForm::slotSetAll(int index)
{
	_setAll->setCurrentIndex(-1); 

	int i;
	for(i = 1; i < ui.traceTable->rowCount(); i++) {
		QComboBox *combo = (QComboBox *)ui.traceTable->cellWidget(i, 1);
		combo->setCurrentIndex(index); 
	}
}

void TTraceForm::slotLoad(bool /*checked*/)
{
	comboToStringList();

	int i;
	for(i = 0; i < _toBeLoaded.size(); i++) {
		if(_toBeLoaded[i].section(',', -1) == "Delete") {
			_toBeLoaded.removeAt(i);
			i--;
		}
	}

	emit signalLoad(_toBeLoaded);
	delete this;
}

void TTraceForm::slotClear(bool /*checked*/)
{
	_toBeLoaded.clear();

	int i;
	for(i = 1; i < ui.traceTable->rowCount(); i++) {
		QComboBox *combo = (QComboBox *)ui.traceTable->cellWidget(i, 1);
		combo->setCurrentIndex(combo->findData(QVariant(QString("")))); 
	}
}

void TTraceForm::slotDelete(bool /*checked*/)
{
	comboToStringList();

	QStringList toBeDeleted;

	int i;
	for(i = 0; i < _toBeLoaded.size(); i++) {
		if(_toBeLoaded[i].section(',', -1) == "Delete") {
			toBeDeleted.append(_toBeLoaded.takeAt(i).section(',', 0, 0));
			i--;
		}
	}

	if(toBeDeleted.size() > 0) {
		QString str = "You are about to delete the following trace files:\n\n";
		for(i = 0; i < toBeDeleted.size(); i++) {
			str += toBeDeleted[i] + "\n";
		}
		str += "\nAre you sure?";

		QMessageBox dialog(QMessageBox::Warning, "Warning", str, QMessageBox::Ok | QMessageBox::Cancel);
		if(dialog.exec() == QMessageBox::Ok) {
			for(i = 0; i < toBeDeleted.size(); i++) {
				QString name = _dirName + '/' + toBeDeleted[i];
				if(QFile::exists(name + ".log")) {
					QFile(name + ".log").remove();	
				}
				if(QFile::exists(name + ".bin")) {
					QFile(name + ".bin").remove();
				}
			}

			setup();
		}
	}
}

const QStringList &TTraceForm::monthsList()
{
	static QStringList list;
	static bool first = true;

	if(first) {
		first = false;
		list << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
	}

	return list;
}

QString TTraceForm::expendDate(const QString &date)
{
	return date.mid(6, 2) + '-' + monthsList()[date.mid(4, 2).toInt() - 1] + '-' + date.mid(2, 2);
}

QString TTraceForm::expendTime(const QString &time)
{
	return time.mid(0, 2) + ':' + time.mid(2, 2);
}

QIcon TTraceForm::colorIcon(const QColor &color, int w, int h)
{
	QPixmap pixmap(w, h);

	pixmap.fill(color);

	QPainter *painter = new QPainter(&pixmap);
	QPen pen;
	pen.setWidth(4);
	painter->setPen(pen);
	painter->drawRect(0, 0, w, h);
	delete painter;

	return QIcon(pixmap);
}

QComboBox *TTraceForm::colorCombo()
{
	QComboBox *combo = new QComboBox;

	int w = combo->iconSize().width();
	int h = combo->iconSize().height() / 2;

	combo->addItem(QString());
	combo->addItem(QString("Delete"), QVariant("Delete"));
	combo->addItem(colorIcon(QColor(128, 0, 0), w, h), QString(), QVariant("128,0,0"));
	combo->addItem(colorIcon(QColor(0, 128, 0), w, h), QString(), QVariant("0,128,0"));
	combo->addItem(colorIcon(QColor(0, 0, 128), w, h), QString(), QVariant("0,0,128"));
	combo->addItem(colorIcon(QColor(128, 128, 0), w, h), QString(), QVariant("128,128,0"));
	combo->addItem(colorIcon(QColor(128, 0, 128), w, h), QString(), QVariant("128,0,128"));
	combo->addItem(colorIcon(QColor(0, 128, 128), w, h), QString(), QVariant("0,128,128"));

	return combo;
}


