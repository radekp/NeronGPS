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

#ifndef TILECOMPRESSED_H
#define TILECOMPRESSED_H

#include <QObject>
#include <QPixmap>

#include "tileref.h"

class TTile;

class TTileCompressed : public QObject, public QByteArray
{
	Q_OBJECT
	public:
		TTileCompressed();
		TTileCompressed(const QByteArray &array);
		~TTileCompressed();

		void setRef(const TTileRef &ref);
		const TTileRef &ref() const { return _ref; }
		bool compare(const TTileRef &ref) { return (_ref == ref); }
		TTile *uncompress();

	private:
		TTileRef _ref;
};

#endif

