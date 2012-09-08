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

#include <nHistoryWidget.h>
#include <nNaveoApplication.h>

nHistoryWidget::nHistoryWidget(nHistoryManager *man, QWidget *parent) : QWidget(parent) {
	manager = man;

	setWindowTitle(tr("Naveo history"));
	setAttribute(Qt::WA_DeleteOnClose);

	QVBoxLayout *layout = new QVBoxLayout(this);
	tree = new QTreeWidget(this);
	tree->setColumnCount(3);
	QStringList headers;
	headers<<tr("Title")<<tr("Url")<<tr("Date");
	tree->setHeaderLabels(headers);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	searchLineEdit = new QLineEdit(this);
	searchLineEdit->setPlaceholderText(tr("Search"));
	connect(searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));

	QPushButton *clearButton = new QPushButton(tr("Clear"), this);
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));

	cathegories[0] = new QTreeWidgetItem(QStringList(tr("Today")));
	cathegories[1] = new QTreeWidgetItem(QStringList(tr("Earlier this week")));
	cathegories[2] = new QTreeWidgetItem(QStringList(tr("Older")));
	cathegories[0]->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	cathegories[1]->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	cathegories[2]->setIcon(0, nApp()->getTheme()->getStyleIcon(QStyle::SP_DirIcon));
	cathegories[0]->setExpanded(true);
	cathegories[1]->setExpanded(true);
	cathegories[2]->setExpanded(true);
	tree->insertTopLevelItem(0, cathegories[2]);
	tree->insertTopLevelItem(0, cathegories[1]);
	tree->insertTopLevelItem(0, cathegories[0]);

	buttonLayout->addWidget(searchLineEdit);
	buttonLayout->addWidget(clearButton);
	layout->addWidget(tree);
	layout->addLayout(buttonLayout);

	tree->setColumnWidth(0, 150);
	tree->setColumnWidth(1, 200);
	tree->setColumnWidth(2, 150);
	resize(550, 400);

	connect(this, SIGNAL(destroyed()), manager, SLOT(save()));
	connect(manager, SIGNAL(entryAdded(nHistoryEntry*)), this, SLOT(addEntry(nHistoryEntry*)));
	#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
	connect(manager, SIGNAL(entryUpdated(nHistoryEntry*)), this, SLOT(updateEntry(nHistoryEntry*)));
	#endif
	manager->load();
}

void nHistoryWidget::clear() {
	for(int i = 0; i != 3; i++) {
		foreach(QTreeWidgetItem *item, cathegories[i]->takeChildren()) {
			delete item;
		}
	}
	manager->clear();
}

void nHistoryWidget::search(QString text) {

}

void nHistoryWidget::addEntry(nHistoryEntry *entry) {
	QIcon i = nApp()->getWebSettings()->iconForUrl(entry->getUrl());
	cathegories[entry->getDate().date() == QDate::currentDate() ? 0 : (entry->getDate().date().addDays(7) > QDate::currentDate() ? 1 : 2)]
		->insertChild(0, new nTreeWidgetItem(entry, i.isNull() ? nApp()->getTheme()->getIcon(nTheme::DefaultPage) : i));
}

#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
void nHistoryWidget::updateEntry(nHistoryEntry *entry) {
	if(entry && entry->getTreeWidgetItem()) {
		entry->getTreeWidgetItem()->setText(0, entry->getTitle());
		entry->getTreeWidgetItem()->setIcon(0, nApp()->getWebSettings()->iconForUrl(entry->getUrl()));
	}
}
#endif
