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

#ifndef MAGNIFICATEFORM_H
#define MAGNIFICATEFORM_H

#include "ui_magnificateform.h"

class TMagnificateForm : public QWidget
{
	Q_OBJECT

	public:
		TMagnificateForm(int magnification, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TMagnificateForm();
		
	public slots:
		void slotX1Clicked(bool checked);
		void slotX2Clicked(bool checked);
		void slotX4Clicked(bool checked);
		void slotKill();

	signals:
		void signalMagnification(int magnification);

	private:
		Ui::MagnificationForm ui;
};

#endif

