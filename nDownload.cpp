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

#include <nDownload.h>
#include <nNaveoApplication.h>

nDownload::nDownload(QNetworkReply *rep, QObject *parent) : QObject(parent) {
	file.setFileName(nApp()->getPath() + "unnamed");
	reply = rep;
	url = reply->url();
	failed = false;
	running = false;
}

nDownload::nDownload(QUrl url, QObject *parent) : QObject(parent) {
	file.setFileName(nApp()->getPath() + "unnamed");
	this->url = url;
	reply = nApp()->getNetworkAccessManager()->get(QNetworkRequest(url));
	failed = false;
	running = false;
}

nDownload::~nDownload() {
	cancel();
}

void nDownload::setTargetFile(QString path) {
	file.setFileName(path);
}

void nDownload::setStream(QNetworkReply *rep) {
	cancel();
	reply = rep;
	url = reply->url();
}

void nDownload::setStreamUrl(QUrl url) {
	cancel();
	this->url = url;
	reply = nApp()->getNetworkAccessManager()->get(QNetworkRequest(url));
}

QString nDownload::getName() {
	return file.fileName();
}

QUrl nDownload::getUrl() {
	return url;
}

QTime nDownload::getTimer() {
	return timer;
}

int nDownload::getElapsedTime() {
	return running ? timer.elapsed() : 0;
}

bool nDownload::isSuccessful() {
	return !failed;
}

bool nDownload::isRunning() {
	return running;
}

void nDownload::setAutoDelete(bool enable) {
	disconnect(this, SIGNAL(downloadFinished(bool)), this, SLOT(deleteLater()));
	if(enable) {
		connect(this, SIGNAL(downloadFinished(bool)), this, SLOT(deleteLater()));
	}
}

bool nDownload::start() {
	failed = true;
	if(!file.open(QIODevice::WriteOnly)) {
		nApp()->error("unable to open " + file.fileName());
		return false;
	}
	if(!reply || reply->error() != QNetworkReply::NoError) {
		nApp()->error(QString("network error : code %1").arg(reply ? (int)reply->error() : -1));
		file.close();
		return false;
	}
	reply->setParent(this);
	failed = false;

	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(write()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(progress(qint64, qint64)));
	connect(reply, SIGNAL(finished()), this, SLOT(finished()));
	timer.start();
	running = true;
	return true;
}

void nDownload::write() {
	if(running) {
		if(!reply) {
			nApp()->error(QString("stream closed"));
			cancel();
		} else {
			if(file.write(reply->readAll()) < 0) {
				nApp()->error(QString("unable to read data from stream"));
				cancel();
			}
		}
	}
}


void nDownload::error(QNetworkReply::NetworkError err) {
	nApp()->error(QString("network error : code %1").arg((int)err));
	cancel();
}

void nDownload::finished() {
	if(running)  {
		running = false;
		file.close();
		if(reply) {
			if(failed) {
				reply->abort();
			} else {
				reply->close();
			}
			reply->deleteLater();
			reply = 0;
		}
		nApp()->debug("Download of \"" + file.fileName() + "\" " + (failed ? "failed" : "finished"));
		emit downloadFinished(!failed);
	}
}

void nDownload::cancel() {
	if(running && reply) {
		failed = true;
		disconnect(reply);
		finished();
	}
}
