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

#ifndef SERVERFORM_H
#define SERVERFORM_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QList>

#include "ui_serverform.h"

class TServerForm : public QWidget
{
	Q_OBJECT

	public:
		TServerForm(const QStringList &serverList, const QString current, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TServerForm();
		
	public slots:
		void slotPressed();
		void slotClicked(bool checked);
		void slotKill();

	signals:
		void signalNewServer(const QString &server);

	private:
		Ui::ServerForm ui;

		QList<QPushButton *> _buttons;
		QPushButton *_pressed;
};

#endif
