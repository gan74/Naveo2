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

#ifndef NTABBAR_H
#define NTABBAR_H

#include <QTabBar>
#include <QMenu>

class nTabBar : public QTabBar
{
    Q_OBJECT

	public:
        explicit nTabBar(QWidget *menu, QWidget *parent);

		int count() const;

		int insertTab(int index, const QIcon &icon, const QString &text);
		int insertTab(int index, const QString &text);
		int addTab(const QString &text);
		int addTab(const QIcon &icon, const QString &text);

	signals:
		void newTabRequested();

	private slots:
		void closeContextMenu();
		void tabMoved(int from, int to);

	protected:
		void tabRemoved(int index);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void contextMenuEvent(QContextMenuEvent *event);


		QMenu *createContextMenuForTab(int index);

		QMenu *contextMenu;
		bool moveTabButton;



};

#endif // NTABBAR_H
