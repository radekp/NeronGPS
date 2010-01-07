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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>

class QFrame;
class TSettings;

class TKeyboard : public QObject
{
	Q_OBJECT

	public:
		TKeyboard();
		~TKeyboard();

		static QStringList getKeyboards(TSettings &settings, const QString &section);

		void attach(QFrame *frame, const QStringList &keyboards);
		void detach();
		
	public slots:
		void slotPressed();
		void slotClicked(bool checked);

	signals:
		void signalKey(QString key);

	private:
		QFrame *_frame;

		QStringList _keyboards;
		int _currentKeyboard;
		bool _upcase;
		QString _currentLayoutID;
		QList<QPushButton *> _buttons;
		QPushButton *_pressed;

		void setKeyboard(const QString &keyboard);
		void setLayout(const QString &layoutID, const QStringList &keys);
		void setKeys(const QStringList &keys);
		void clearLayout();
		void setUpcase(bool upcase);

		static QString prepareLayoutString(const QString &layout);
};

#endif
