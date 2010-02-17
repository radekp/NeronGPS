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
#include <QMessageBox>
#include <QMenu>

#include "include/actionsmanager.h"

TActionsManager::TActionsManager()
{
}

TActionsManager::~TActionsManager()
{
}

void TActionsManager::configure(TSettings &settings, const QString &section, const QStringList &actionsList, QMenu &mainMenu, QMenu &othersMenu, TButtonsBoard &board)
{
	QStringList main;
	QStringList more;
	QStringList button;

	settings.beginGroup(section);

	int i;
	for(i = 0; i < actionsList.size(); i++) {
		QString setting = actionsList[i].section('/', 0, 0);
		QString name = actionsList[i].section('/', 1, 1);
		QString config = actionsList[i].section('/', 2, -1);

		config = settings.getValue(setting, config).toString();
		sort(name, config, main, more, button);
	}

	settings.endGroup();

	setupMenu(main, mainMenu);
	setupMenu(more, othersMenu);
	setupButtons(button, board);
}

void TActionsManager::sort(const QString &name, const QString &config, QStringList &main, QStringList &more, QStringList &button)
{
	QString str1 = config.section('/', 0, 0);
	QString str2 = config.section('/', 1, -1) + '/' + name;

	if(str1 == "main") {
		main.append(str2);
	} else if(str1 == "more") {
		more.append(str2);
	} else if(str1 == "button") {
		button.append(str2);
	} else if(str1 != "none") {
		qDebug() << "Error, unknown action type: " << str1;
	}
}

void TActionsManager::setupMenu(QStringList &actions, QMenu &menu)
{
	actions.sort();

	int i;
	for(i = 0; i < actions.size(); i++) {
		QAction *action = new QAction(actions[i].section('/', -1, -1), this);
		menu.addAction(action);
		_actions.append(action);
	}
}

void TActionsManager::setupButtons(QStringList &actions, TButtonsBoard &board)
{
	int i;
	for(i = 0; i < actions.size(); i++) {
		QAction *action = new QAction(actions[i].section('/', -1, -1), this);
		board.addButton(action, actions[i].section('/', 2, -2), (actions[i].section('/', 0, 0)).toInt(), (actions[i].section('/', 1, 1)).toInt());
		_actions.append(action);
	}
}

void TActionsManager::connectTrigger(const QString &name, const QObject *receiver, const char *method, Qt::ConnectionType type)
{
	QAction *action = getAction(name);

	if(action != NULL) {
		QObject::connect(action, SIGNAL(triggered()), receiver, method, type);	
	} else {
		qDebug() << "Error: attempt to connect to an unknown action, " << name;
	}
}

void TActionsManager::connectChange(const QString &name, const QObject *receiver, const char *method, Qt::ConnectionType type)
{
	QAction *action = getAction(name);

	if(action != NULL) {
		QObject::connect(action, SIGNAL(changed()), receiver, method, type);	
	} else {
		qDebug() << "Error: attempt to connect to an unknown action, " << name;
	}
}

void TActionsManager::connectToggle(const QString &name, bool state, const QObject *receiver, const char *method, Qt::ConnectionType type)
{
	QAction *action = getAction(name);

	if(action != NULL) {
		action->setCheckable(true);
		action->setChecked(state);
		QObject::connect(action, SIGNAL(toggled(bool)), receiver, method, type);	
	} else {
		qDebug() << "Error: attempt to connect to an unknown action, " << name;
	}
}

void TActionsManager::slotChangeState(const QString &name, bool visible, bool enabled)
{
	QAction *action = getAction(name);

	if(action != NULL) {
		action->setEnabled(enabled);
		action->setVisible(visible);
	} else {
		qDebug() << "Error: attempt to change state of an unknown action, " << name;
	}
}

QAction *TActionsManager::getAction(const QString &name)
{
	QAction *ret;

	int i;
	for(i = 0; (i < _actions.size()) && (_actions[i]->text() != name); i++);

	if(i == _actions.size()) {
		ret = NULL;
	} else {
		ret = _actions[i];
	}

	return ret;
}

