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

#ifndef NWINDOW_H
#define NWINDOW_H

#include <QtGui>
#include <QtWebKit>
#include <nTabBar.h>
#include <nWebView.h>
#include <nUrlLineEdit.h>

class nWindow : public QWidget
{
	Q_OBJECT

	public:
		nWindow(QWidget *parent = 0);
		~nWindow();

	public slots:
		void addTab();
		void addTab(nWebView *view);

		void closeTab(int index);

	private slots:
		void tabMoved(int from, int to);
		void currentTabChanged(int index);
		void tabTitleChanged(const QString &title);
		void loadProgress(int pro);

		void urlEntered(const QUrl &url);

		void changeSettings();

	private:
		int tabIndex(QObject *o) {
			return tabIndexes.indexOf(stack->indexOf(qobject_cast<nWebView *>(o)));
		}

		void disconnectTab(nWebView *v);
		void connectTab(nWebView *v);

		nTabBar *tabBar;
		QToolBar *toolBar;
		nUrlLineEdit *urlEdit;
		QStackedWidget *stack;
		nWebView *current;
		QList<int> tabIndexes;

		QAction *backAction;
		QAction *forwardAction;
		QAction *stopAction;
		QAction *reloadAction;
};

#endif // NWINDOW_H
