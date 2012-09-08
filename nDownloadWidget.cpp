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

#include <nDownloadWidget.h>
#include <nNaveoApplication.h>
#include <algorithm>

#define KILO 1024
#define MEGA KILO * KILO

QCleanlooksStyle *nDownloadWidget::progressBarStyle = new QCleanlooksStyle();

nDownloadWidget::nDownloadWidget(nDownload *dl, QWidget *parent) : QWidget(parent) {
	download = dl;

	seconds = 0;
	itCount = 0;

	setFixedHeight(32);
	setMinimumWidth(300);

	progressBar = new QProgressBar(this);
	progressBar->setStyle(progressBarStyle);
	progressBar->setAlignment(Qt::AlignHCenter);
	progressBar->setRange(0, 0);

	openButton = new QPushButton(tr("Open"), this);
	retryButton = new QPushButton(tr("Retry"), this);
	cancelButton = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 1, 0, 1);
	const int linkMaxLength = 75;
	QString link = dl->getUrl().toString(QUrl::RemoveQuery);
	layout->addWidget(new QLabel("<strong>" + dl->getName().split("/").last() + "</strong><br><a href=\"" + dl->getUrl().toString() + "\"><sub>" + (link.length() > linkMaxLength ? link.left(linkMaxLength - 3) + "..." : link) + "</sub></a>", this), 0);
	layout->addWidget(progressBar, 1);
	layout->addStretch();
	layout->addWidget(cancelButton);
	layout->addWidget(openButton);
	layout->addWidget(retryButton);

	connect(dl, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
	connect(dl, SIGNAL(downloadFinished(bool)), this, SLOT(finished(bool)));
	connect(cancelButton, SIGNAL(clicked()), dl, SLOT(cancel()));
	connect(openButton, SIGNAL(clicked()), this, SLOT(open()));
	connect(retryButton, SIGNAL(clicked()), this, SLOT(retry()));

	openButton->hide();
	retryButton->hide();
}

QString nDownloadWidget::formatFileSize(quint64 size, bool forceMb) {
	return size >= MEGA || forceMb ? tr("%1Mb").arg(round((double)size / (MEGA / 10)) / 10) : (size >= KILO ? tr("%1Kb").arg(round((double)size / (KILO / 10)) / 10) : tr("%1 bytes").arg(size));
}

void nDownloadWidget::updateProgress(qint64 done, qint64 total) {
	double percent = std::max(0.001, (double)done / (double)total);
	qint64 totalTime = ((double)download->getElapsedTime() /  1000) / percent;
	seconds += totalTime;
	itCount++;
	#warning download time estimation keep all values
	// why not use Naveo1 time estimation which is much more stable ?
	qint64 remaining = seconds / itCount - download->getElapsedTime() /  1000;
	progressBar->setFormat(QString("%p% : %1 (%2)  ").arg(remaining < 60 ? QString("%1s").arg(remaining) : QString("%1min %2s").arg(remaining / 60).arg(remaining % 60)).arg(formatFileSize(done, total > MEGA)));
	progressBar->setRange(0, total);
	progressBar->setValue(done);
}

void nDownloadWidget::open() {
	QDesktopServices::openUrl(QUrl(download->getName())); // ask for root ?
}

void nDownloadWidget::retry() {
	nDownload *dl = new nDownload(download->getUrl());
	dl->setTargetFile(download->getName());
	dl->start();
	disconnect(cancelButton, SIGNAL(clicked()), download, SLOT(cancel()));
	download = dl;
	connect(cancelButton, SIGNAL(clicked()), dl, SLOT(cancel()));
	connect(dl, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
	connect(dl, SIGNAL(downloadFinished(bool)), this, SLOT(finished(bool)));
	progressBar->show();
	progressBar->setRange(0, 0);
	cancelButton->show();
	openButton->hide();
	retryButton->hide();
	download->deleteLater();
}


void nDownloadWidget::finished(bool ok) {
	progressBar->hide();
	cancelButton->hide();
	openButton->setVisible(ok);
	retryButton->setVisible(!ok);
}
