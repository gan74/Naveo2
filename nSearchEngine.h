/*******************************
Copyright (C) 2009-2010 gr�goire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/

#ifndef NSEARCHENGINE_H
#define NSEARCHENGINE_H

#include <QtCore>

class nSearchEngine
{
	public:
		nSearchEngine() {}

		virtual QString search(QString s) = 0;
};

class nGoogleSearchEngine : public nSearchEngine
{
	public:
		nGoogleSearchEngine();

		QString search(QString s);
};

#endif // NSEARCHENGINE_H
