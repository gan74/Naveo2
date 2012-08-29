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

#include <nWebView.h>
#include <nNaveoApplication.h>
#include <nDownloadWidget.h>

nWebView::nWebView(QWidget *parent) : QWebView(parent) {
	progress = 0;
	load(QUrl("http://qt.nokia.com/downloads/"));
	connect(this, SIGNAL(loadProgress(int)), this, SLOT(updateProgress(int)));
	page()->setForwardUnsupportedContent(true);
	connect(page(), SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(unsupportedContent(QNetworkReply*)));
}

int nWebView::getProgress() {
	return progress;
}

void nWebView::updateProgress(int pro) {
	progress = pro;
}

void nWebView::unsupportedContent(QNetworkReply *reply) {
	if(reply->error() == QNetworkReply::NoError && reply->header(QNetworkRequest::ContentLengthHeader).isValid()) {
		int size = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
		QString type = reply->header(QNetworkRequest::ContentTypeHeader).toString();
		if(size) {
			QUrl url = reply->url();
			nApp()->debug(QString("Downloading " + url.toString() + " (%1 bytes : MIME " + type + ")").arg(size));
			nDownload *dl = new nDownload(reply);
			dl->setTargetFile(nApp()->getPath() + url.toString(QUrl::RemoveQuery).split("/").last());
			if(!dl->start()) {
				nApp()->error(QString("unable to start download"));
				dl->deleteLater();
			} else {
				nApp()->getDownloadManager()->addDownload(dl);
			}
		} else {
			nApp()->error(QString("network reply with size = 0"));
			reply->deleteLater();
		}
	} else {
		nApp()->debug(QString("network code = %1").arg((int)reply->error()));
		reply->deleteLater();
	}
}
