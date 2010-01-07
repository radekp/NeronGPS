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

#ifndef PAINTERELEMENT_H
#define PAINTERELEMENT_H

#include <QPainter>

class TPainterElement
{
	public:
		virtual ~TPainterElement() {};
		virtual void draw(QPainter &painter, int x, int y, int w, int h) = 0;
};

class TPainterElementLine : public TPainterElement
{
	public:
		TPainterElementLine(const QString &params);
		~TPainterElementLine();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		int _x1, _y1, _x2, _y2;
};

class TPainterElementTransparentRect : public TPainterElement
{
	public:
		TPainterElementTransparentRect(const QString &params);
		~TPainterElementTransparentRect();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		int _x1, _y1, _w, _h;
};

class TPainterElementOpaqueRectangle : public TPainterElement
{
	public:
		TPainterElementOpaqueRectangle(const QString &params);
		~TPainterElementOpaqueRectangle();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		int _x1, _y1, _w, _h;
};

class TPainterElementTransparentEllipse : public TPainterElement
{
	public:
		TPainterElementTransparentEllipse(const QString &params);
		~TPainterElementTransparentEllipse();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		int _x1, _y1, _w, _h;
};

class TPainterElementOpaqueEllipse : public TPainterElement
{
	public:
		TPainterElementOpaqueEllipse(const QString &params);
		~TPainterElementOpaqueEllipse();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		int _x1, _y1, _w, _h;
};

class TPainterElementSegment : public TPainterElement
{
	public:
		TPainterElementSegment(const QString &params);
		~TPainterElementSegment();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		QList<int> _points;
};

class TPainterElementPolygon : public TPainterElement
{
	public:
		TPainterElementPolygon(const QString &params);
		~TPainterElementPolygon();
		void draw(QPainter &painter, int x, int y, int w, int h);

	private:
		QList<int> _points;
};

#endif

