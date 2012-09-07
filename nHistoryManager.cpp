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

nHistoryManager::nHistoryManager(QWidget *parent) : QWidget(parent) {
	setWindowTitle("Naveo history");

	QVBoxLayout *layout = new QVBoxLayout(this);
	tree = new QTreeWidget(this);

	QStringList headers;
	headers<<tr("Title")<<tr("Url")<<tr("Date");
	tree->setHeaderLabels(headers);

	tree->setColumnWidth(0, 150);
	tree->setColumnWidth(1, 200);
	tree->setColumnWidth(2, 150);
	tree->setColumnCount(3);

	layout->addWidget(tree);
	resize(550, 400);

	load();
}

void nHistoryManager::load() {
	QFile file(nSettings(nSettingsManager::HistoryFilePath).toString());
	if(!file.open(QIODevice::ReadOnly)) {
		nApp()->error("Unable to open " + file.fileName());
		refresh();
		return;
	}

	QDataStream stream(&file);
	while(!stream.atEnd()) {
		nHistoryEntry entry;
		stream>>entry;
		history.append(entry);
	}

	file.close();
	refresh();
}

void nHistoryManager::save() const {
	QFile file(nSettings(nSettingsManager::HistoryFilePath).toString());
	if(!file.open(QIODevice::WriteOnly)) {
		nApp()->error("Unable to open " + file.fileName());
		return;
	}

	QDataStream stream(&file);
	foreach(nHistoryEntry entry, history) {
		stream<<entry;
	}

	file.close();
}

QTreeWidgetItem *nHistoryManager::createItem(nHistoryEntry &entry){
	QStringList l;
	l<<entry.getTitle()<<entry.getUrl().toString()<<entry.getDate().toString(tr("MM/dd/yyyy \'at\' hh:mm:ss"));
	QTreeWidgetItem *item = new QTreeWidgetItem(l);
	item->setIcon(0, nApp()->getWebSettings()->iconForUrl(entry.getUrl()));
	if(item->icon(0).isNull()) {
		item->setIcon(0, nApp()->getTheme()->getIcon(nTheme::DefaultPage));
	}
	return item;
}

void nHistoryManager::addEntry(QWebHistory *h) {
	nHistoryEntry entry(h->currentItem(), h);
	history.append(entry);
	tree->invisibleRootItem()->child(0)->insertChild(0, createItem(entry));

	save();
}

void nHistoryManager::refresh() {
	bool expand[] = {false, false, false};
	if(tree->invisibleRootItem()->childCount()) {  // may store root items instead ?
		expand[0] = tree->invisibleRootItem()->child(0)->isExpanded();
		expand[1] = tree->invisibleRootItem()->child(1)->isExpanded();
		expand[2] = tree->invisibleRootItem()->child(2)->isExpanded();
		tree->clear();
	}
	QTreeWidgetItem *today = new QTreeWidgetItem(tree, QStringList(tr("Today")));
	QTreeWidgetItem *lastWeek = new QTreeWidgetItem(tree, QStringList(tr("Last week")));
	QTreeWidgetItem *older = new QTreeWidgetItem(tree, QStringList(tr("Older")));
	today->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	lastWeek->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	older->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	today->setExpanded(expand[0]);
	lastWeek->setExpanded(expand[1]);
	older->setExpanded(expand[2]);

	foreach(nHistoryEntry entry, history) {
		QTreeWidgetItem *parent = entry.getDate().date() == QDate::currentDate() ? today : (QDate::currentDate().daysTo(entry.getDate().date()) > -7 ? lastWeek : older);
		parent->insertChild(0, createItem(entry));
	}

	nApp()->debug("history widget refreshed");
}
