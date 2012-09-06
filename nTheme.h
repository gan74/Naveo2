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

#ifndef NTHEME_H
#define NTHEME_H

#include <QtGui>

class nTheme
{
	public:
		static const int iconCount = 8;
		enum IconRole {
			Back = 0,
			Forward = 1,
			Stop = 2,
			Reload = 3,
			DefaultPage = 4,
			Plugin = 5,
			NewTab = 6,
			Download = 7
		};

		nTheme();

		void setPixmap(QPixmap pix, IconRole ic);
		QPixmap getPixmap(IconRole ic) const;
		QIcon getIcon(IconRole ic) const;

		virtual QIcon getStyleIcon(QStyle::StandardPixmap ic) const {
			return QApplication::style()->standardIcon(ic);
		}


	private:
		QPixmap *icons;


};

#endif // NTHEME_H
