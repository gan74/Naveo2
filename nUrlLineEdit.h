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

#ifndef NLINEEDIT_H
#define NLINEEDIT_H

#include <QtGui>

class nUrlLineEdit : public QLineEdit
{
    Q_OBJECT
	public:
		explicit nUrlLineEdit(QWidget *parent = 0);

		QUrl getUrl();

	public slots:
		void setUrl(QUrl url);
		void setText(const QString &t);
		void setIcon(QIcon icon);

		void loadProgress(int pro);

	signals:
		void urlEntered(QUrl url);

	private slots:
		void finished();
		void loadStarted();
		void loadFinished(bool ok);

		void fade();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		bool isUrl(QString u);
		QIcon pageIcon;
		QMovie *movie;
		int progress;

};

#endif // NLINEEDIT_H
