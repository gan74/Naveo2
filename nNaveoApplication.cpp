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
#include <nWindow.h>

nNaveoApplication::nNaveoApplication(int argc, char *argv[]) : QApplication(argc, argv) {
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(close()));
	console = new nDebugConsole();

	checkInstance();

	settings = new nSettingsManager(this);
	loadTranslator();

	theme = new nTheme();

	initWebSettings();

	engine = new nGoogleSearchEngine();
	downloadManager = new nDownloadManager();
	accessManager = new QNetworkAccessManager(this);
	historyManager = new nHistoryManager();
	QWebHistoryInterface::setDefaultInterface(historyManager);


	setWindowIcon(QIcon(":/icon.png"));
	connect(settings, SIGNAL(settingsChanged()), this, SIGNAL(settingsChanged()));
	connect(this, SIGNAL(settingsChanged()), this, SLOT(updateSettings()));
	updateSettings();
}

nNaveoApplication::~nNaveoApplication() {
	delete engine;
	delete downloadManager;
	delete historyManager;
	delete accessManager;
	delete settings;
	delete console;
}

void nNaveoApplication::checkInstance() {
	static const QString appName = "Naveo browser";
	server = 0;
	sharedMemory = new QSharedMemory(appName);
	if(!sharedMemory->create(sizeof(uint))) {
		if(sharedMemory->error() == QSharedMemory::AlreadyExists) {
			debug("There is already an instance running");
			QLocalSocket *socket = new QLocalSocket(this);
			socket->connectToServer(appName);
			if(socket->waitForConnected(500)) {
				QDataStream stream(socket);
				foreach(QString arg, arguments()) {
					stream.writeBytes(arg.toAscii(), arg.size());
				}
				socket->flush();
				socket->waitForBytesWritten();
			}
			throw nNaveoAlreadyRunningException();
		} else {
			error("unable to get shared memory access : " + sharedMemory->errorString());
		}
	} else {
		server = new QLocalServer(this);
		connect(server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
		if(!server->listen(appName)) {
			error("unable to setup local server " + server->errorString());
		} else {
			debug("server created");
		}
	}
}

void nNaveoApplication::newLocalConnection() {
	if(!server) {
		return;
	}
	debug("Connection etablished");
	QLocalSocket *socket = server->nextPendingConnection();
	if(socket) {
		socket->waitForReadyRead(1000);
		QDataStream stream(socket);
		QStringList args;
		while(!stream.atEnd()) {
			char *c = 0;
			uint size = 0;
			stream.readBytes(c, size);
			args.append(QString::fromAscii(c, size));
		}
		parseArguments(args);
		socket->deleteLater();
	}

}

void nNaveoApplication::showHistory() {
	(new nHistoryWidget(historyManager))->show(); // the beauty of Qt : alloc and forget
}

void nNaveoApplication::showSettingsPanel() {
	nSettingsWidget *panel = new nSettingsWidget(settings);
	connect(panel, SIGNAL(destroyed()), this, SIGNAL(settingsChanged())); // panel is deleted on close
	panel->show();
}

void nNaveoApplication::parseArguments(const QStringList &args) {
	foreach(QString arg, args) {
		if(arg == QLatin1String("-console")) {
			console->show();
		} else if(arg == QLatin1String("-hmanager")) {
			showHistory();
		} else if(arg.left(4) == QLatin1String("http")) {
			nWindow *win = new nWindow();
			win->addTab()->load(QUrl(arg));
			win->show();
		}
	}
}

nNaveoApplication *nNaveoApplication::app() {
	return (nNaveoApplication *)qApp;
}

void nNaveoApplication::debug(QString msg) {
	if(!msg.isEmpty()) {
		console->message(msg.left(1).toUpper() + msg.right(msg.size() - 1));
	}

}

void nNaveoApplication::error(QString err) {
	if(!err.isEmpty()) {
		debug("Error : " + err.left(1).toLower() + err.right(err.size() - 1));
	}
}

int nNaveoApplication::exec() {
	QWidget *win = activeWindow();
	QStringList args = arguments();
	parseArguments(args);
	setActiveWindow(win);
	debug("Naveo started");
	return QApplication::exec();
}


void nNaveoApplication::close() {
	settings->save();
}

void nNaveoApplication::updateSettings() {
	loadTranslator();
}

void nNaveoApplication::loadTranslator() {
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

