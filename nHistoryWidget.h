/*******************************
Copyright (C) 2009-2010 grégoire ANGERAND

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

#ifndef NHISTORYWIDGET_H
#define NHISTORYWIDGET_H

#include <QtGui>
#include <nHistoryManager.h>

class nHistoryWidget : public QWidget
{
	Q_OBJECT
	class nTreeWidgetItem : public QTreeWidgetItem
	{
		public:
			nTreeWidgetItem(nHistoryEntry *e, QIcon i = QIcon()) : QTreeWidgetItem() {
				#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
				entry = e;
				entry->setTreeWidgetItem(this);
				#endif
				setText(0, e->getTitle());
				setText(1, e->getUrl().toString());
				setText(2, e->getDate().toString(QObject::tr("MM/dd/yyyy \'at\' hh:mm:ss")));
				setIcon(0, i);
			}

		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
			~nTreeWidgetItem() {

				if(entry) {
					entry->setTreeWidgetItem(0);
				}
			}

		private:
			nHistoryEntry *entry;
		#endif

	};

	public:
		explicit nHistoryWidget(nHistoryManager *man, QWidget *parent = 0);
		
	signals:
		
	public slots:
		void clear();
		void search(QString text);

	private slots:
		void addEntry(nHistoryEntry *entry);

		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		void updateEntry(nHistoryEntry *entry);
		#endif

	private:
		nHistoryManager *manager;

		QTreeWidget *tree;
		QTreeWidgetItem *cathegories[3];
		QLineEdit *searchLineEdit;
		
};

#endif // NHISTORYWIDGET_H
