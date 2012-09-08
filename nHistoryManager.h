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

#ifndef NHISTORYMANAGER_H
#define NHISTORYMANAGER_H

#include <QtNetwork>
#include <QWebHistoryInterface>
#include <nHistoryEntry.h>

class nHistoryManager : public QWebHistoryInterface
{
    Q_OBJECT

	class nHistoryEntryRef
	{
		public:
			nHistoryEntryRef(nHistoryEntry *e = 0) {
				entry = e;
			}

			nHistoryEntry *entry;
	};

	public:
		explicit nHistoryManager(QObject *parent = 0);
		~nHistoryManager();

		void addHistoryEntry(const QString &url);
		bool historyContains(const QString &url) const;

		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		void updateEntries(const QUrl &url, const QString &title);
		#else
		void addEntry(const QUrl &url, const QString &title);
		#endif

	signals:
		void entryAdded(nHistoryEntry *entry);
		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		void entryUpdated(nHistoryEntry *entry);
		#endif

	public slots:
		bool save();
		bool load();

		void clear();

	protected:
		void clearData();

	private:
		static const int maxDataSize = 4; // 16 ?
		bool loaded;
		QList<nHistoryEntry *> history;
		QCache<QString, nHistoryEntryRef> cache; // OOoooh, This is bad ! create custom cache class


};

#endif // NHISTORYMANAGER_H
