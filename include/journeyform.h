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

#ifndef JOURNEYFORM_H
#define JOURNEYFORM_H

#include <QWhereabouts>
#include <QWhereaboutsUpdate>

#include "ui_journeyform.h"

class TJourneyForm : public QWidget
{
	Q_OBJECT

	public:
		TJourneyForm(int samples, QWidget *parent = 0, Qt::WFlags f = 0);
		virtual ~TJourneyForm();
		
	public slots:
		void slotRecordInfo(QString name, int samples);
		void slotReset(bool checked);
		void slotGpsData(const QWhereaboutsUpdate &update);
		void slotNewStat(int time, int distance, float speed, int altitude, int altMin, int altMax, int fix);

	signals:
		void signalReset();

	private:
		Ui::JourneyForm ui;
};

#endif

