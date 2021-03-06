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

#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include "include/mapdrawlist.h"
#include "include/action.h"

class TActionsManager : public TMapDrawListElement
{
	Q_OBJECT 
	public:
		TActionsManager();
		~TActionsManager();

		void configure(TSettings &settings, const QString &section);
		void populateMainMenu(QMenu &menu);
		void populateAlternateMenu(QMenu &menu);

		void draw(QPainter &painter, TDrawState &drawState);
		void press(int x, int y, int w, int h);

		void connectTrigger(const QString &name, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection);
		void connectChange(const QString &name, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection);
		void connectToggle(const QString &name, bool state, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection);

	public slots:
		void slotChangeState(const QString &name, bool visible, bool enabled);

	private:
		QList<TAction *> _actions;

		QColor _buttonBackground;
		QColor _buttonForeground;
		int _buttonSize;

		TAction *_topLeft;
		TAction *_topRight;
		TAction *_bottomLeft;
		TAction *_bottomRight;

		QList<TAction *> _mainMenu;
		QList<TAction *> _alternateMenu;

		QAction *getAction(const QString &name);
};

#endif

