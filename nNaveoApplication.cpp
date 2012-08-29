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

#include <nNaveoApplication.h>

nNaveoApplication::nNaveoApplication(int argc, char *argv[]) : QApplication(argc, argv) {
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(close()));

	console = new nDebugConsole();
	settings = new nSettingsManager(this);
	theme = new nTheme();
	engine = new nGoogleSearchEngine();
	downloadManager = new nDownloadManager();
	accessManager = new QNetworkAccessManager(this);

	initWebSettings();
	setWindowIcon(QIcon(":/icon.png"));
	connect(settings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
	settingsChanged();
}

nNaveoApplication *nNaveoApplication::app() {
	return (nNaveoApplication *)qApp;
}

void nNaveoApplication::debug(QString msg) {
	console->message(msg);
}

void nNaveoApplication::error(QString err) {
	debug("Error : " + err);
}

int nNaveoApplication::exec() {
	QStringList args = arguments();
	if(args.contains("-console")) {
		QWidget *win = activeWindow();
		console->show();
		setActiveWindow(win);
	}
	return QApplication::exec();
}


void nNaveoApplication::close() {
	settings->save();
}

void nNaveoApplication::settingsChanged() {
	QTranslator translator;
	if(!translator.load(QString(getPath() + "/locale/qt_") +  nSettings(nSettingsManager::Locale).toLocale().name())) {
		error("Unable to load translation \"" + settings->getSettings(nSettingsManager::Locale).toLocale().name() + "\"");
	} else {
		debug("translation loaded");
		installTranslator(&translator);
	}
}

QString nNaveoApplication::getPath() {
	#ifdef Q_WS_WIN
		return applicationDirPath() + "/";
	#else
		return QDir::homePath() + "/.Naveo2/";
	#endif
}

void nNaveoApplication::initWebSettings() {
	QNetworkProxyFactory::setUseSystemConfiguration(true);
	webSettings = QWebSettings::globalSettings();

	webSettings->setMaximumPagesInCache(40);
	webSettings->setIconDatabasePath(getPath() + "cache/");
	webSettings->setOfflineStoragePath(getPath() + "cache/");
	webSettings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
	webSettings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
	webSettings->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
	webSettings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);
	webSettings->setAttribute(QWebSettings::AutoLoadImages, true);
	webSettings->setAttribute(QWebSettings::PluginsEnabled, true);
	webSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
	webSettings->setAttribute(QWebSettings::JavascriptCanOpenWindows , true);
	webSettings->setAttribute(QWebSettings::JavaEnabled, true);
	webSettings->setAttribute(QWebSettings::PrivateBrowsingEnabled , false);
	QWebSettings::setWebGraphic(QWebSettings::MissingPluginGraphic, theme->getPixmap(nTheme::Plugin));
	QWebSettings::setWebGraphic(QWebSettings::DefaultFrameIconGraphic, theme->getPixmap(nTheme::DefaultPage));
}

QLibrary *nNaveoApplication::getLibrary(QString name) {
	QLibrary *l = libs[name];
	if(!l) {
		l = new QLibrary(name);
		libs[name] = l;
	}
	return l;
}

nTheme *nNaveoApplication::getTheme() {
	return theme;
}

nSearchEngine *nNaveoApplication::getSearchEngine() {
	return engine;
}

nDownloadManager *nNaveoApplication::getDownloadManager() {
	return downloadManager;
}

QWebSettings *nNaveoApplication::getWebSettings() {
	return webSettings;
}

QNetworkAccessManager *nNaveoApplication::getNetworkAccessManager() {
	return accessManager;
}

nSettingsManager *nNaveoApplication::getSettingsManager() {
	return settings;
}
