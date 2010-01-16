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

#ifndef KEYBOARDFORM_H
#define KEYBOARDFORM_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>

#include "ui_keyboardform.h"

#include "keyboard.h"

class TKeyboardForm : public QWidget
{
	Q_OBJECT

	public:
		TKeyboardForm(const QStringList &keyboards, const QString &exitText, QWidget *parent = 0, Qt::WFlags f = 0);
		~TKeyboardForm();
		
	public slots:
		void slotKey(QString key);

	signals:
		void signalText(QString text);

	protected:
		void closeEvent(QCloseEvent *);

	private:
		Ui::KeyboardForm ui;

		TKeyboard _keyboard;
		QString _exitText;
};

#endif
