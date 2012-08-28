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

nDownloadWidget::nDownloadWidget(nDownload *dl, QWidget *parent) : QWidget(parent) {
	download = dl;

	seconds = 0;
	itCount = 0;

	progressBar = new QProgressBar(this);
	#warning creating one new QCleanlooksStyle per download
	progressBar->setStyle(new QCleanlooksStyle());
	progressBar->setRange(0, 100);

	timeLabel = new QLabel(this);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel("<strong>" + dl->getName().split("/").last() + "</strong><br><a href=\"" + dl->getUrl().toString() + "\"><sub>" + dl->getUrl().toString(QUrl::RemoveQuery) + "</sub></a>", this));
	layout->addWidget(timeLabel);
	layout->addWidget(progressBar);

	connect(dl, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
}

void nDownloadWidget::updateProgress(qint64 done, qint64 total) {
	double percent = std::max(0.001, (double)done / (double)total);
	qint64 totalTime = ((double)download->getElapsedTime() /  1000) / percent;
	seconds += totalTime;
	itCount++;
	#warning download time estimation keep all values
	qint64 remaining = seconds / itCount - download->getElapsedTime() /  1000;

	if(remaining <= 60) {
		timeLabel->setText(QString("%1s remaining").arg(remaining));
	} else {
		timeLabel->setText(QString("%1min %2s remaining").arg(remaining / 60).arg(remaining % 60));
	}
	progressBar->setValue(percent * 100);
}
