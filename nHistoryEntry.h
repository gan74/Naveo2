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

class nHistoryEntry
{
	public:
		nHistoryEntry();
		nHistoryEntry(QString &t, QUrl &u, QDateTime &d, QWebHistory *p = 0);
		nHistoryEntry(QWebHistoryItem item, QWebHistory *p);


		QDateTime getDate() const;
		QString getTitle() const;
		QUrl getUrl() const;

		void setDate(QDateTime d);
		void setTitle(QString t);
		void setUrl(QUrl u);

	private:
		QString title;
		QUrl url;
		QDateTime date;
		QWebHistory *parent;
};

QDataStream& operator<<(QDataStream& stream, const nHistoryEntry &entry);
QDataStream& operator>>(QDataStream& stream, nHistoryEntry &entry);

#endif // NHISTORYENTRY_H
