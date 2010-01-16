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

#include "include/keyboardform.h"

TKeyboardForm::TKeyboardForm(const QStringList &keyboards, const QString &exitText, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	ui.setupUi(this);

	_exitText = exitText;

	connect(&_keyboard, SIGNAL(signalKey(QString)), this, SLOT(slotKey(QString)));

	_keyboard.attach(ui.frame, keyboards);
}

TKeyboardForm::~TKeyboardForm()
{
	_keyboard.detach();
}

void TKeyboardForm::closeEvent(QCloseEvent *event)
{
	QMessageBox dialog(QMessageBox::Question, "Closing", _exitText, QMessageBox::Yes | QMessageBox::No);
	if(dialog.exec() == QMessageBox::Yes) {
		emit signalText(ui.text->text());
	}
}

void TKeyboardForm::slotKey(QString key)
{
	if(key == QString("<-")) {
		ui.text->setText(ui.text->text().left(ui.text->text().size() - 1));
	} else {
		ui.text->setText(ui.text->text() + key);
	}
}


