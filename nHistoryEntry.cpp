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

#include <nHistoryEntry.h>
#include <nNaveoApplication.h>

nHistoryEntry::nHistoryEntry() {
	date = QDateTime(QDate::currentDate(), QTime::currentTime());
	#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
	item = 0;
	#endif
}

nHistoryEntry::nHistoryEntry(const QUrl &u, const QDateTime &d, QString t) {
	title = t;
	url = u;
	date = d;
	#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
	item = 0;
	#endif
}

nHistoryEntry::~nHistoryEntry() {
}

QDateTime nHistoryEntry::getDate() const {
	return date;
}

QString nHistoryEntry::getTitle() const {
	return title.isEmpty() ? url.host() : title;
}

QUrl nHistoryEntry::getUrl() const {
	return url;
}

#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
QTreeWidgetItem *nHistoryEntry::getTreeWidgetItem() {
	return item;
}

void nHistoryEntry::setTreeWidgetItem(QTreeWidgetItem *i) {
	item = i;
}
#endif

void nHistoryEntry::setDate(const QDateTime &d) {
	date = d;
}

void nHistoryEntry::setTitle(const QString &t) {
	title = t;
}

void nHistoryEntry::setUrl(const QUrl &u) {
	url = u;
}

bool nHistoryEntry::match(const QString &s) const {
	return title.contains(s) || (s.size() > 3 && url.toString().contains(s));
}

QDataStream &operator<<(QDataStream &stream, const nHistoryEntry &entry) {
	QByteArray t = entry.getTitle().toAscii();
	QByteArray u = entry.getUrl().toString().toAscii();
	stream<<t.size()<<t.data()<<u.size()<<u.data()<<entry.getDate().toTime_t();
	return stream;
}

QDataStream& operator>>(QDataStream& stream, nHistoryEntry &entry) {
	uint size = 0;
	char *c = 0;
	stream>>size;
	stream.readBytes(c, size);
	entry.setTitle(QString(c));
	stream>>size;
	stream.readBytes(c, size);
	entry.setUrl(QUrl(QString(c)));
	stream>>size; // time
	QDateTime date;
	date.setTime_t(size);
	entry.setDate(date);
	return stream;
}
