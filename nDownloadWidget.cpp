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

	setFixedHeight(32);
	setMinimumWidth(300);

	progressBar = new QProgressBar(this);
	#warning creating one new QCleanlooksStyle per download
	progressBar->setStyle(new QCleanlooksStyle());
	progressBar->setAlignment(Qt::AlignHCenter);
	progressBar->setRange(0, 0);


	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 1, 0, 1);
	const int linkMaxLength = 75;
	QString link = dl->getUrl().toString(QUrl::RemoveQuery);
	layout->addWidget(new QLabel("<strong>" + dl->getName().split("/").last() + "</strong><br><a href=\"" + dl->getUrl().toString() + "\"><sub>" + (link.length() > linkMaxLength ? link.left(linkMaxLength - 3) + "..." : link) + "</sub></a>", this), 0);
	layout->addWidget(progressBar, 1);
	layout->addSpacing(5);

	connect(dl, SIGNAL(progress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
}

QString nDownloadWidget::formatFileSize(quint64 size) {
	QString s = size >= 1048576 ? QString("%1Mo").arg(round((double)size / 104857.6) / 10) : (size >= 1024 ? QString("%1Ko").arg(round((double)size / 102.4) / 10) : QString("%1o").arg(size));
	return s.contains(".") ? s : s + ".0";
}

void nDownloadWidget::updateProgress(qint64 done, qint64 total) {
	double percent = std::max(0.001, (double)done / (double)total);
	qint64 totalTime = ((double)download->getElapsedTime() /  1000) / percent;
	seconds += totalTime;
	itCount++;
	#warning download time estimation keep all values
	// why not use Naveo1 time estimation which is much more stable ?
	qint64 remaining = seconds / itCount - download->getElapsedTime() /  1000;
	progressBar->setFormat(QString("%p% : %1 (%2)  ").arg(remaining < 60 ? QString("%1s").arg(remaining) : QString("%1min %2s").arg(remaining / 60).arg(remaining % 60)).arg(formatFileSize(done)));
	progressBar->setRange(0, total);
	progressBar->setValue(done);
}
