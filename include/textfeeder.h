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

#ifndef TEXTFEEDER_H
#define TEXTFEEDER_H

class TTextFeeder;

class TTextFeeder
{
	public:
		TTextFeeder(const TTextFeeder &feeder);
		TTextFeeder();
		~TTextFeeder();

		inline const QString &content() const { return _content; }
	
		inline TTextFeeder &operator<<(QChar t) { _content += t; return *this; }
		inline TTextFeeder &operator<<(bool t) { _content += t ? "true" : "false"; return *this; }
		inline TTextFeeder &operator<<(char t) { _content += t; return *this; }
		inline TTextFeeder &operator<<(signed short t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(unsigned short t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(signed int t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(unsigned int t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(signed long t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(unsigned long t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(qint64 t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(quint64 t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(float t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(double t) { _content += QString::number(t); return *this; }
		inline TTextFeeder &operator<<(const char* t) { _content += QString::fromAscii(t); return *this; }
		inline TTextFeeder &operator<<(const QString & t) { _content += t; return *this; }
		inline TTextFeeder &operator<<(const QLatin1String &t) { _content += t.latin1(); return *this; }
		inline TTextFeeder &operator<<(const QByteArray & t) { _content += t; return *this; }
		inline TTextFeeder &operator<<(const void *t) { _content += QString::number((uint)t, 16); return *this; }

	private:
		QString _content;
};

#endif

