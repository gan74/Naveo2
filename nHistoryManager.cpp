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

#include <nHistoryManager.h>
#include <nNaveoApplication.h>

nHistoryManager::nHistoryManager(QObject *parent) : QWebHistoryInterface(parent) {
	cache.setMaxCost(256);
	loaded = false;
}

nHistoryManager::~nHistoryManager() {
	save();
}

void nHistoryManager::addHistoryEntry(const QString &url) {
	if(nApp()->getWebSettings()->testAttribute(QWebSettings::PrivateBrowsingEnabled)) {
		return;
	}
	#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
	nHistoryEntry *entry = new nHistoryEntry(QUrl(url), QDateTime(QDate::currentDate(), QTime::currentTime()));
	history.append(entry);
	cache.insert(url, new nHistoryEntryRef(entry));
	emit entryAdded(entry);
	if(history.size() % maxDataSize == 0) {
		save();
	}
	#else
	cache.insert(url, new nHistoryEntryRef());
	Q_UNUSED(url);
	#endif
}

#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
void nHistoryManager::updateEntries(const QUrl &url, const QString &title) {
	nHistoryEntryRef *e = cache.object(url.toString());
	if(e) {
		e->entry->setTitle(title);
		emit entryUpdated(e->entry);
	}
}
#else
void nHistoryManager::addEntry(const QUrl &url, const QString &title) {
	if(nApp()->getWebSettings()->testAttribute(QWebSettings::PrivateBrowsingEnabled)) {
		return;
	}
	nHistoryEntry *entry = new nHistoryEntry(url, QDateTime(QDate::currentDate(), QTime::currentTime()), title);
	history.append(entry);
	emit entryAdded(entry);
	if(history.size() % maxDataSize == 0) {
		save();
	}
}

#endif

bool nHistoryManager::historyContains(const QString &url) const {
	return cache.contains(url);
}


bool nHistoryManager::save() {
	QFile file(nSettings(nSettingsManager::HistoryFilePath).toString());
	if(!file.open(loaded ? QIODevice::WriteOnly : QIODevice::Append)) { //overwrite if loaded, else append
		nApp()->error("unable to open " + file.fileName());
		return false;
	}

	QDataStream stream(&file); // encrypt !
	foreach(nHistoryEntry *entry, history) {
		stream<<*entry;
	}

	file.close();
	clearData();
	return true;
}

bool nHistoryManager::load() {
	QFile file(nSettings(nSettingsManager::HistoryFilePath).toString());
	if(!file.open(QIODevice::ReadOnly)) {
		nApp()->error("unable to open " + file.fileName());
		return false; // this->loaded stay the same
	}

	QDataStream stream(&file);
	QList<nHistoryEntry *> lst;
	while(!stream.atEnd()) {
		nHistoryEntry *entry = new nHistoryEntry();
		stream>>*entry;
		lst.append(entry); // check for expiration
		emit entryAdded(entry);
		cache.insert(entry->getUrl().toString(), new nHistoryEntryRef(entry));
	}
	lst.append(history);
	history = lst;

	file.close();
	return loaded = true;
}

void nHistoryManager::clear() {
	if(!QFile::remove(nSettings(nSettingsManager::HistoryFilePath).toString())) {
		nApp()->debug("Nothing to clear");
	}
	clearData();
}

void nHistoryManager::clearData() {
	loaded = false;
	foreach(nHistoryEntry *entry, history) {
		delete entry;
	}
	history.clear();
	cache.clear();
}
