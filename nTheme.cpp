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
	icons = new QIcon[iconCount];
	setIcon(QIcon(":/theme/back.png"), Back);
	setIcon(QIcon(":/theme/forward.png"), Forward);
	setIcon(QIcon(":/theme/stop.png"), Stop);
	setIcon(QIcon(":/theme/reload.png"), Reload);
}

void nTheme::setIcon(QIcon i, Icon ic) {
	icons[(int)ic] = i;
}

QIcon nTheme::getIcon(Icon ic) {
	return icons[(int)ic];
}
