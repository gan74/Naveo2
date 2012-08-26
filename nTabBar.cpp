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

#include <nTabBar.h>
#include <nNaveoApplication.h>

nTabBar::nTabBar(QWidget *parent) : QTabBar(parent) {
	setMovable(true);
	setTabsClosable(true);
	setUsesScrollButtons(false);
	setExpanding(false);
	QTabBar::addTab("+");
	tabButton(0, QTabBar::RightSide)->deleteLater();
	setTabButton(0, QTabBar::RightSide, 0);

	setStyleSheet("QTabBar::tab:last { width: 31px; }");
}

int nTabBar::count() const {
	return QTabBar::count() - 1;
}

void nTabBar::mousePressEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton && tabAt(event->pos()) == count()) {
		emit newTabRequested();
		return;
	}
	QTabBar::mousePressEvent(event);
}

int nTabBar::insertTab(int index, const QIcon &icon, const QString &text) {
	return QTabBar::insertTab(index < 0 ? count() : index, icon, text);
}

int nTabBar::insertTab(int index, const QString &text) {
	return insertTab(index, QIcon(), text);
}

int nTabBar::addTab(const QIcon &icon, const QString &text) {
	return insertTab(-1, icon, text);
}

int nTabBar::addTab(const QString &text) {
	return addTab(QIcon(), text);
}

void nTabBar::tabRemoved(int index) {
	QTabBar::tabRemoved(index);
	if(currentIndex() == count()) {
		setCurrentIndex(index - 1);
	}
}

