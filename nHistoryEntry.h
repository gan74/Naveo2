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

#ifndef NHISTORYENTRY_H
#define NHISTORYENTRY_H

#include <QtGui>
#include <QtNetwork>
#include <QWebHistory>

/* Qt provide with QWebHistoryInterface a way to get history,
 * HOWEVER you need to implement your own way to get the page title.
 * If I have to call the history manager directly, i wish i have
 * to call it only once per entry.
 */
#define NAVEO_DONT_USE_WEBKIT_HISTORY

class nHistoryEntry
{

	public:
		nHistoryEntry();
		nHistoryEntry(const QUrl &u, const QDateTime &d, QString t = QString());
		~nHistoryEntry();


		QDateTime getDate() const;
		QString getTitle() const;
		QUrl getUrl() const;

		void setDate(const QDateTime &d);
		void setTitle(const QString &t);
		void setUrl(const QUrl &u);

		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		QTreeWidgetItem *getTreeWidgetItem();
		void setTreeWidgetItem(QTreeWidgetItem *i);
		#endif

		bool match(const QString &s) const;

	private:
		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		QTreeWidgetItem *item;
		#endif

		QString title;
		QUrl url;
		QDateTime date;
};

QDataStream& operator<<(QDataStream& stream, const nHistoryEntry &entry);
QDataStream& operator>>(QDataStream& stream, nHistoryEntry &entry);

#endif // NHISTORYENTRY_H
