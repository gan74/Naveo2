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

		QString search(QString s, bool privateMode = false) {
			return privateMode ? processPrivate(s) : process(s);
		}

	protected:
		virtual QString process(QString s) = 0;
		virtual QString processPrivate(QString s) const = 0 ; // for ease of use rather than for safety (because easy to bypass
};

class nGoogleSearchEngine : public nSearchEngine
{
	public:
		nGoogleSearchEngine();

	protected:
		QString process(QString s);
		QString processPrivate(QString s) const;
};

#endif // NSEARCHENGINE_H
