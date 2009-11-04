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

#ifndef TRACEFORM_H
#define TRACEFORM_H

#include <QWhereabouts>
#include <QWhereaboutsUpdate>

#include <QResizeEvent>
#include <QComboBox>

#include "ui_traceform.h"
#include "include/directory.h"

class TTraceForm : public QWidget
{
	Q_OBJECT

	public:
		TTraceForm(const QString &dir, const QStringList &loaded, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TTraceForm();
		
	protected:
		void resizeEvent (QResizeEvent *event);

	public slots:
		void slotList(int index);
		void slotSetAll(int index);
		void slotLoad(bool checked);
		void slotClear(bool checked);
		void slotDelete(bool checked);

	signals:
		void signalLoad(QStringList traceNames);

	private:
		Ui::TraceForm ui;
		QComboBox *_setAll;

		QStringList _toBeLoaded;

		QStringList _shortNames;
		QStringList _traceNames;
		TDirectory _traceDir;
		QString _dirName;

		static const QStringList &monthsList();
		QString expendDate(const QString &date);
		QString expendTime(const QString &time);

		void setup();
		void comboToStringList();
		void stringListToCombo();

		QIcon colorIcon(const QColor &color, int w, int h);
		QComboBox *colorCombo();
};

#endif

