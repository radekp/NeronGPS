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
#include <QFrame>

#include "include/keyboard.h"
#include "include/settings.h"

TKeyboard::TKeyboard()
{
	_pressed = NULL;
	_frame = NULL;
	_upcase = false;
}

TKeyboard::~TKeyboard()
{
	if(_frame != NULL) {
		qDebug() << "Error, destroy an attached keyboard";
		detach();
	}
}

void TKeyboard::attach(QFrame *frame, const QStringList &keyboards)
{
	if(_frame != NULL) {
		qDebug() << "Error, attach a keyboard without detach";
		detach();
	}

	_frame = frame;
	_keyboards = keyboards;

	_currentKeyboard = 0;
	setKeyboard(_keyboards[0]);
}

void TKeyboard::detach()
{
	if(_frame != NULL) {
		clearLayout();
		_frame = NULL;
	} else {
		qDebug() << "Error, detach an unattached keyboard";
	}
}

void TKeyboard::slotPressed()
{
	int i;
	for(i = 0; (i < _buttons.size()) && (!_buttons[i]->isDown()); i++);

	if(i < _buttons.size()) {
		_pressed = _buttons[i];
	} else {
		qDebug("Error, unknown button pressed");
	}
}

void TKeyboard::slotClicked(bool /*checked*/)
{
	if(_pressed != NULL) {
		if(_pressed->text() == QString("ALT")) {
			_currentKeyboard ++;
			if(_currentKeyboard >= _keyboards.size()) {
				_currentKeyboard = 0;		
			}
			setKeyboard(_keyboards[_currentKeyboard]);
		} else if(_pressed->text() == QString("CAPS")) {
			setUpcase(!_upcase);
		} else if(_pressed->text() == QString("SPACE")) {
			emit signalKey(" ");
		} else {
			emit signalKey(_pressed->text());
		}

		_pressed = NULL;
	} else {
		qDebug("Error, button clicked without having been pressed!");
	}
}

void TKeyboard::setKeyboard(const QString &keyboard)
{
	QStringList keys = keyboard.split(QChar(' '));
	QString layoutID = keys.takeFirst();

	if(layoutID != _currentLayoutID) {
		setLayout(layoutID, keys);
		_currentLayoutID = layoutID;
	} else {
		setKeys(keys);
	}
}

void TKeyboard::setLayout(const QString &layoutID, const QStringList &keys)
{
	clearLayout();

	QStringList layout = layoutID.split(QChar('/'));

	QVBoxLayout *vLayout = new QVBoxLayout;

	int i;
	for(i = 0; i < layout.size(); i++) {
		QHBoxLayout *hLayout = new QHBoxLayout;

		int j;
		for(j = 0; j < layout[i].size(); j++) {
			QPushButton *button = new QPushButton("", _frame);
			hLayout->addWidget(button);
			_buttons.append(button);

			connect(button, SIGNAL(pressed()), this, SLOT(slotPressed()));
			connect(button, SIGNAL(clicked(bool)), this, SLOT(slotClicked(bool)));
		}

		vLayout->insertLayout(-1, hLayout);
	}

	setKeys(keys);

	_frame->setLayout(vLayout);
}

void TKeyboard::setKeys(const QStringList &keys)
{
	int i;
	for(i = 0; i < _buttons.size(); i++) {
		QString key = keys[i];

		if(_upcase && (key.size() == 1)) {
			key = key.toUpper();	
		}

		_buttons[i]->setText(key);
	}
}

void TKeyboard::clearLayout()
{
	delete _frame->layout();

	while(_buttons.size() > 0) {
		delete _buttons.takeFirst();
	}
}

void TKeyboard::setUpcase(bool upcase)
{
	_upcase = upcase;

	setKeyboard(_keyboards[_currentKeyboard]);
}

QStringList TKeyboard::getKeyboards(TSettings &settings, const QString &section)
{
	QStringList keyboards;

	settings.beginGroup(section);

	if(!settings.contains("keyboard1")) {
		keyboards += settings.getValue("keyboard1", "a b c d e  f g h i j  k l m n o  p q r s t  u v w x y  z SPACE <- CAPS ALT").toString();
		keyboards += settings.getValue("keyboard2", "1 2 3 4 5  6 7 8 9 0  ( ) [ ] {  } + - = _  / % * ! @  $ SPACE <- CAPS ALT").toString();
	} else {
		int i = 1;
		QString num = QString::number(i);

		while(settings.contains("keyboard" + num)) {
			keyboards += settings.getValue("keyboard" + num, "").toString();
			i++;
			num = QString::number(i);
		}
	}

	settings.endGroup();

	int i;
	for(i = 0; i < keyboards.size(); i ++) {
		keyboards[i] = prepareLayoutString(keyboards[i]);
	}

	return keyboards;
}

QString TKeyboard::prepareLayoutString(const QString &layout)
{
	QString layoutID;
	QString keys;

	QStringList tmp = layout.split(QChar(' '));

	int i;
	for(i = 0; i < tmp.size(); i++) {
		if(tmp[i].isEmpty()) {
			layoutID += '/';
		} else {
			layoutID += '#';
			keys += ' ';
			keys += tmp[i];
		}
	}

	return layoutID + keys;
}

