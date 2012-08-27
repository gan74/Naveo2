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

#ifndef NDOWNLOAD_H
#define NDOWNLOAD_H

#include <QtNetwork>

class nDownload : public QObject
{
	Q_OBJECT

	public:
		nDownload(QNetworkReply *rep = 0, QObject *parent = 0);
		nDownload(QUrl url, QObject *parent = 0);
		~nDownload();

		void setTargetFile(QString path);
		void setStream(QNetworkReply *rep);
		void setStreamUrl(QUrl url);

		void setAutoDelete(bool enable);

		QString getName();
		QUrl getUrl();

		QTime getTimer();
		int getElapsedTime();

		bool isSuccessful();
		bool isRunning();

	public slots:
		bool start();
		void cancel();

	signals:
		void progress(qint64, qint64);
		void downloadFinished(bool);

	private slots:
		void write();
		void error(QNetworkReply::NetworkError err);
		void finished();

	private:
		QNetworkReply *reply;
		QTime timer;
		QFile file;
		bool failed;
		bool running;
};

#endif // NDOWNLOAD_H
