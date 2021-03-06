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

#include <nTheme.h>

nTheme::nTheme() {
	dir = ":/theme/";
	icons = new QPixmap[iconCount];
	setPixmap(QPixmap(dir + "back.png"), Back);
	setPixmap(QPixmap(dir + "forward.png"), Forward);
	setPixmap(QPixmap(dir + "stop.png"), Stop);
	setPixmap(QPixmap(dir + "reload.png"), Reload);
	setPixmap(QPixmap(dir + "newTab.png"), NewTab);
	setPixmap(QPixmap(dir + "download.png"), Download);
	setPixmap(QPixmap(dir + "menu.png"), MainMenu);

	setPixmap(QPixmap(":/earth.png"), DefaultPage);
	setPixmap(QPixmap(":/plugin.png"), Plugin);
}

QString nTheme::getThemeDir() const {
	return dir;
}

void nTheme::setPixmap(QPixmap pix, IconRole ic) {
	icons[(int)ic] = pix;
}

QPixmap nTheme::getPixmap(IconRole ic) const {
	return icons[(int)ic];
}

QIcon nTheme::getIcon(IconRole ic) const {
	return QIcon(getPixmap(ic));
}

