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

#include <nDownloadManager.h>
#include <nNaveoApplication.h>

nDownloadManager::nDownloadManager(QWidget *parent) : QWidget(parent) {
	setWindowTitle(tr("Naveo2 download manager"));

	QVBoxLayout *layout = new QVBoxLayout(this);

	list = new QListWidget(this);
	list->setAlternatingRowColors(true);
	list->setIconSize(QSize(32, 32));
	layout->addWidget(list);


	resize(520, 400);
}

void nDownloadManager::addDownload(nDownload *download) {
	list->insertItem(0, new QListWidgetItem(nApp()->getTheme()->getIcon(nTheme::Download), ""));
	list->setItemWidget(list->item(0), new nDownloadWidget(download));
	show();
}
