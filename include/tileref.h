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

#ifndef TILEREF_H
#define TILEREF_H

class TTileRef
{
	public:
		enum State { Empty, Invalid, Error, Valid };

		TTileRef(const QString &server, int x, int y, int zoom);
		TTileRef(const TTileRef &ref);
		TTileRef();
		~TTileRef();

		int x() const { return _x; }
		int y() const { return _y; }
		int zoom() const { return _zoom; }
		const QString &server() const { return _server; }
		const State &state() const { return _state; }

		TTileRef &operator= (const TTileRef &ref);
		bool operator== (const TTileRef &ref) const;
		bool operator!= (const TTileRef &ref) const;

	private:
		QString _server;
		int _x;
		int _y;
		int _zoom;
		State _state;

		void check();
};

#endif

