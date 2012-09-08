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
#include <QWebSettings>
#include <QtGui>
#include <nSearchEngine.h>
#include <nDebugConsole.h>
#include <nDownloadManager.h>
#include <nTheme.h>
#include <nSettingsManager.h>
#include <nHistoryWidget.h>

#define nSettings(s) nApp()->getSettingsManager()->getSettings(s)

class nNaveoApplication : public QApplication
{
	Q_OBJECT

	public:
		nNaveoApplication(int argc, char *argv[]);

		QLibrary *getLibrary(const QString &name);

		nTheme *getTheme() const;
		nSearchEngine *getSearchEngine() const;
		QWebSettings *getWebSettings() const;
		QNetworkAccessManager *getNetworkAccessManager() const;
		nDownloadManager *getDownloadManager() const;
		nSettingsManager *getSettingsManager() const;
		nHistoryManager *getHistoryManager() const;

		QString getPath() const;
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
		nHistoryManager *historyManager;
		QMap<QString, QLibrary *> libs;
};

#endif // NNAVEOAPPLICATION_H
