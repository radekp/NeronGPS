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

#include "include/serverform.h"
#include "include/tilehttpname.h"

TServerForm::TServerForm(const QStringList &serverList, const QString current, QWidget *parent, Qt::WFlags f) : QWidget(parent, f)
{
	QStringList tmpList;
	QString currentDisplay = TTileHttpName::convertToDisplayName(current);
	int i;

	_pressed = NULL;

	ui.setupUi(this);

	QVBoxLayout *layout = new QVBoxLayout;

	for(i = 0; i < serverList.size(); i++) {
		tmpList << TTileHttpName::convertToDisplayName(serverList[i]);
	}
	tmpList.sort();

	for(i = 0; i < tmpList.size(); i++) {
		QPushButton *button = new QPushButton(tmpList[i], ui.frame);

		if(tmpList[i] == currentDisplay) {
			button->setDisabled(true); 
		}

		layout->addWidget(button);
		_buttons.append(button);

		connect(button, SIGNAL(pressed()), this, SLOT(slotPressed()));
		connect(button, SIGNAL(clicked(bool)), this, SLOT(slotClicked(bool)));
	}

	ui.frame->setLayout(layout);
}

TServerForm::~TServerForm()
{
	delete ui.frame->layout();

	while(_buttons.size() > 0) {
		delete _buttons.takeFirst();
	}
}

void TServerForm::slotPressed()
{
	int i;
	for(i = 0; (i < _buttons.size()) && (!_buttons[i]->isDown()); i++);

	if(i < _buttons.size()) {
		_pressed = _buttons[i];
	} else {
		qDebug("Error, unknown button pressed");
	}
}

void TServerForm::slotClicked(bool /*checked*/)
{
	if(_pressed != NULL) {
		QString str = TTileHttpName::convertFromDisplayName(_pressed->text());

		hide();

		emit signalNewServer(str);

		QTimer::singleShot(1, this, SLOT(slotKill()));
	} else {
		qDebug("Error, button clicked without having been pressed!");
	}
}

void TServerForm::slotKill()
{
	delete this;
}

