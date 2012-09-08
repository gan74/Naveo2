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

	initWebSettings();

	engine = new nGoogleSearchEngine();
	downloadManager = new nDownloadManager();
	accessManager = new QNetworkAccessManager(this);
	historyManager = new nHistoryManager();
	QWebHistoryInterface::setDefaultInterface(historyManager);


	setWindowIcon(QIcon(":/icon.png"));
	connect(settings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
	settingsChanged();
}

nNaveoApplication *nNaveoApplication::app() {
	return (nNaveoApplication *)qApp;
}

void nNaveoApplication::debug(QString msg) {
	if(!msg.isEmpty()) {
		msg = msg.left(1).toUpper() + msg.right(msg.size() - 1);
		console->message(msg);
	}

}

void nNaveoApplication::error(QString err) {
	if(!err.isEmpty()) {
		err = err.left(1).toLower() + err.right(err.size() - 1);
		debug("Error : " + err);
	}
}

int nNaveoApplication::exec() {
	QStringList args = arguments();
	QWidget *win = activeWindow();
	if(args.contains("-console")) {
		console->show();
	}
	if(args.contains("-hmanager")) {
		(new nHistoryWidget(historyManager))->show(); // the beauty of Qt : alloc and forget
	}
	setActiveWindow(win);
	return QApplication::exec();
}


void nNaveoApplication::close() {
	settings->save();
}

void nNaveoApplication::settingsChanged() {
	QTranslator translator;
	if(!translator.load(QString(getPath() + "/locale/qt_") +  nSettings(nSettingsManager::Locale).toLocale().name())) {
		error("unable to load translation \"" + settings->getSettings(nSettingsManager::Locale).toLocale().name() + "\"");
	} else {
		debug("Translation loaded");
		installTranslator(&translator);
	}
}

QString nNaveoApplication::getPath() const {
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
	webSettings->setWebGraphic(QWebSettings::MissingPluginGraphic, theme->getPixmap(nTheme::Plugin));
	webSettings->setWebGraphic(QWebSettings::DefaultFrameIconGraphic, theme->getPixmap(nTheme::DefaultPage));
}

QLibrary *nNaveoApplication::getLibrary(const QString &name) {
	QLibrary *l = libs[name];
	if(!l) {
		l = new QLibrary(name);
		libs[name] = l;
		debug(name + " loaded");
	}
	return l;
}

nTheme *nNaveoApplication::getTheme() const {
	return theme;
}

nSearchEngine *nNaveoApplication::getSearchEngine() const {
	return engine;
}

nDownloadManager *nNaveoApplication::getDownloadManager() const {
	return downloadManager;
}

QWebSettings *nNaveoApplication::getWebSettings() const {
	return webSettings;
}

QNetworkAccessManager *nNaveoApplication::getNetworkAccessManager() const {
	return accessManager;
}

nSettingsManager *nNaveoApplication::getSettingsManager() const {
	return settings;
}

nHistoryManager *nNaveoApplication::getHistoryManager() const {
	return historyManager;
}

