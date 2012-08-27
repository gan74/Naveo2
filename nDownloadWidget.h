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

#ifndef NDOWNLOADWIDGET_H
#define NDOWNLOADWIDGET_H

#include <QtGui>
#include <nDownload.h>

class nDownloadWidget : public QWidget
{
    Q_OBJECT
	public:
		explicit nDownloadWidget(nDownload *dl, QWidget *parent = 0);

	signals:

	public slots:

	private slots:
		void updateProgress(qint64 done, qint64 total);

	private:
		nDownload *download;
		QLabel *timeLabel;
		QProgressBar *progressBar;
		qint64 seconds;
		qint64 itCount;
};

#endif // NDOWNLOADWIDGET_H
