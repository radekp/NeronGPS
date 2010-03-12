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
	_topLeft = NULL;
	_topRight = NULL;
	_bottomLeft = NULL;
	_bottomRight = NULL;
}

TActionsManager::~TActionsManager()
{
}

void TActionsManager::configure(TSettings &settings, const QString &section)
{
	QStringList main;
	QStringList alternate;
	QString topLeft;
	QString topRight;
	QString bottomLeft;
	QString bottomRight;

	main << "server" << "zoom" << "magnification" << "journey" << "poi" << "canceldrive" << "startbatch" << "stopbatch";
	alternate << "traces" << "clock" << "caches" << "log" << "displayon" << "record" << "trailer";

	settings.beginGroup(section);

	_buttonBackground = settings.getColor("buttonbackground", "000000ff");
	_buttonForeground = settings.getColor("buttonforeground", "ffffffff");
	_buttonSize = settings.getValue("buttonsize", 80).toInt();

	topLeft = settings.getValue("topleft", "minus").toString();
	topRight = settings.getValue("topright", "plus").toString();
	bottomLeft = settings.getValue("bottomleft", "").toString();
	bottomRight = settings.getValue("bottomright", "center").toString();

	main = settings.getValue("mainmenu", main).toStringList(); 
	alternate = settings.getValue("alternatemenu", alternate).toStringList(); 

	settings.endGroup();

	QStringList list = main;

	int i;
	for(i = 0; i < alternate.size(); i++) {
		if(!list.contains(alternate[i])) { list << alternate[i]; }
	}

	if(!topLeft.isEmpty() && !list.contains(topLeft)) { list << topLeft; }
	if(!topRight.isEmpty() && !list.contains(topRight)) { list << topRight; }
	if(!bottomLeft.isEmpty() && !list.contains(bottomLeft)) { list << bottomLeft; }
	if(!bottomRight.isEmpty() && !list.contains(bottomRight)) { list << bottomRight; }

	for(i = 0; i < list.size(); i++) {
		TAction *action = new TAction;
		action->configure(settings, section, list[i]);
		_actions << action;
	}
	
	for(i = 0; i < main.size(); i++) {
		_mainMenu << _actions[list.indexOf(main[i])];
	}

	for(i = 0; i < alternate.size(); i++) {
		_alternateMenu << _actions[list.indexOf(alternate[i])];
	}

	if(!topLeft.isEmpty()) { _topLeft = _actions[list.indexOf(topLeft)]; }
	if(!topRight.isEmpty()) { _topRight = _actions[list.indexOf(topRight)]; }
	if(!bottomLeft.isEmpty()) { _bottomLeft = _actions[list.indexOf(bottomLeft)]; }
	if(!bottomRight.isEmpty()) { _bottomRight = _actions[list.indexOf(bottomRight)]; }
}

void TActionsManager::populateMainMenu(QMenu &menu)
{
	int i;
	for(i = 0; i < _mainMenu.size(); i++) {
		menu.addAction(_mainMenu[i]);
	}
}

void TActionsManager::populateAlternateMenu(QMenu &menu)
{
	int i;
	for(i = 0; i < _alternateMenu.size(); i++) {
		menu.addAction(_alternateMenu[i]);
	}
}

void TActionsManager::draw(QPainter &painter, TDrawState &drawState)
{
	int x = drawState.width();
	int y = drawState.height();

	QPen oldPen = painter.pen();
	QBrush oldBrush = painter.brush();

	if((_topLeft != NULL) && (_topLeft->isVisible())) {
		painter.setBrush(QBrush(_buttonBackground));
		painter.setPen(QPen(_buttonBackground));
		QPolygon polygon;
		polygon << QPoint(0, 0) << QPoint(_buttonSize, 0)  << QPoint(0, _buttonSize);
		painter.drawPolygon(polygon);		
		painter.setBrush(QBrush(_buttonForeground));
		painter.setPen(QPen(_buttonForeground));
		_topLeft->draw(painter, 0, 0, _buttonSize / 2, _buttonSize / 2);
		painter.drawLine(_buttonSize, 0, 0, _buttonSize);
	}

	if((_topRight != NULL) && (_topRight->isVisible())) {
		painter.setBrush(QBrush(_buttonBackground));
		painter.setPen(QPen(_buttonBackground));
		QPolygon polygon;
		polygon << QPoint(x, 0) << QPoint(x - _buttonSize, 0)  << QPoint(x, _buttonSize);
		painter.drawPolygon(polygon);		
		painter.setBrush(QBrush(_buttonForeground));
		painter.setPen(QPen(_buttonForeground));
		_topRight->draw(painter, x - (_buttonSize / 2), 0, _buttonSize / 2, _buttonSize / 2);
		painter.drawLine(x - _buttonSize, 0, x, _buttonSize);
	}

	if((_bottomLeft != NULL) && (_bottomLeft->isVisible())) {
		painter.setBrush(QBrush(_buttonBackground));
		painter.setPen(QPen(_buttonBackground));
		QPolygon polygon;
		polygon << QPoint(0, y) << QPoint(_buttonSize, y)  << QPoint(0, y - _buttonSize);
		painter.drawPolygon(polygon);		
		painter.setBrush(QBrush(_buttonForeground));
		painter.setPen(QPen(_buttonForeground));
		_bottomLeft->draw(painter, 0, y - (_buttonSize / 2), _buttonSize / 2, _buttonSize / 2);
		painter.drawLine(_buttonSize, y, 0, y - _buttonSize);
	}

	if((_bottomRight != NULL) && (_bottomRight->isVisible())) {
		painter.setBrush(QBrush(_buttonBackground));
		painter.setPen(QPen(_buttonBackground));
		QPolygon polygon;
		polygon << QPoint(x, y) << QPoint(x - _buttonSize, y)  << QPoint(x, y - _buttonSize);
		painter.drawPolygon(polygon);		
		painter.setBrush(QBrush(_buttonForeground));
		painter.setPen(QPen(_buttonForeground));
		_bottomRight->draw(painter, x - (_buttonSize / 2), y - (_buttonSize / 2), _buttonSize / 2, _buttonSize / 2);
		painter.drawLine(x - _buttonSize, y, x, y - _buttonSize);
	}

	painter.setBrush(oldBrush);
	painter.setPen(oldPen);
}

void TActionsManager::press(int x, int y, int w, int h)
{
	int midX = w / 2;
	int midY = h / 2;

	if((x < midX) && (y < midY) && (_topLeft != NULL) && (_topLeft->isVisible()) && (_topLeft->isEnabled())) {
		_topLeft->trigger();
	}

	if((x >= midX) && (y < midY) && (_topRight != NULL) && (_topRight->isVisible()) && (_topRight->isEnabled())) {
		_topRight->trigger();
	}

	if((x < midX) && (y >= midY) && (_bottomLeft != NULL) && (_bottomLeft->isVisible()) && (_bottomLeft->isEnabled())) {
		_bottomLeft->trigger();
	}

	if((x >= midX) && (y >= midY) && (_bottomRight != NULL) && (_bottomRight->isVisible()) && (_bottomRight->isEnabled())) {
		_bottomRight->trigger();
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
	for(i = 0; (i < _actions.size()) && (_actions[i]->name() != name); i++);

	if(i == _actions.size()) {
		ret = NULL;
	} else {
		ret = _actions[i];
	}

	return ret;
}

