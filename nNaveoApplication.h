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

#ifndef NNAVEOAPPLICATION_H
#define NNAVEOAPPLICATION_H

#define nApp nNaveoApplication::app
#include <QtWebKit>
#include <QtGui>
#include <nSearchEngine.h>
#include <nDebugConsole.h>
#include <nDownloadManager.h>
#include <nTheme.h>
#include <nSettingsManager.h>

#define nSettings(s) nApp()->getSettingsManager()->getSettings(s)

class nNaveoApplication : public QApplication
{
	Q_OBJECT

	public:
		nNaveoApplication(int argc, char *argv[]);

		nTheme *getTheme();
		nSearchEngine *getSearchEngine();
		QWebSettings *getWebSettings();
		QLibrary *getLibrary(QString name);
		QNetworkAccessManager *getNetworkAccessManager();
		nDownloadManager *getDownloadManager();
		nSettingsManager *getSettingsManager();

		QString getPath();
		static nNaveoApplication *app();

		void debug(QString msg);
		void error(QString err);

		int exec();

	signals:

	public slots:
		void close();
		void settingsChanged();

	private:
		void initWebSettings();

		nTheme *theme;
		nSearchEngine *engine;
		nDebugConsole *console;
		QWebSettings *webSettings;
		QNetworkAccessManager *accessManager;
		nDownloadManager *downloadManager;
		nSettingsManager *settings;
		QMap<QString, QLibrary *> libs;
};

#endif // NNAVEOAPPLICATION_H
