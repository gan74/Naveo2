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

#include <nSettingsManager.h>
#include <nNaveoApplication.h>

nSettingsManager::nSettingsManager(QObject *parent) : QObject(parent) {
	settings = new QVariant[maxSettings];
	names = new QString[maxSettings];

	names[SessionName] = "session";
	names[Locale] = "locale";
	names[HideStopButton] = "hideStopButton";
	names[HistoryFilePath] = "historyFilePath";
    names[offlineStorage] = "OfflineStorage";
    names[offlineCache] = "OfflineCache";
    names[localeStorage] = "localeStorage";
    names[dnsPrefetch] = "dnsPrefetch";
    names[loadImages] = "loadImages";
    names[enablePlugins] = "enablePlugins";
    names[enableJavascript] = "enableJavascript";
    names[enableJava] = "enableJava";
    names[javascriptWindow] = "javascriptWindow";
    names[homePage] = "homePage";
    names[downloadDir] = "downloadDir";

	timer = new QTimer(this);
	timer->setInterval(2500);
	connect(timer, SIGNAL(timeout()), this, SLOT(save()));
	load();
}

nSettingsManager::~nSettingsManager() {
}

QVariant nSettingsManager::getSettings(Settings s) const {
	return settings[s];
}

void nSettingsManager::setSettings(Settings s, QVariant v) {
	if(settings[s] != v) {
		settings[s] = v;
        if(s == SessionName || s == Locale || s == HideStopButton || s == HistoryFilePath)
            emit(settingsChanged());
	}
}

void nSettingsManager::saveEntry(QSettings *out, Settings e) const {
	out->setValue(names[e], getSettings(e));
}

void nSettingsManager::save() const {
	QSettings s(nApp()->getPath() + "settings.ini", QSettings::IniFormat);
	s.setValue(names[SessionName], getSettings(SessionName).toString());

	s.beginGroup(getSettings(SessionName).toString());
	s.setValue(names[Locale], getSettings(Locale).toLocale().name());
	for(int i = 2; i != maxSettings; i++) {
		saveEntry(&s, (Settings)i);
	}
	s.endGroup();

	nApp()->debug("Settings saved in " + nApp()->getPath() + "settings.ini");
}

void nSettingsManager::load() {
	blockSignals(true);
	QSettings s(nApp()->getPath() + "settings.ini", QSettings::IniFormat);
	setSettings(SessionName, getSettings(SessionName).isValid() ? getSettings(SessionName).toString() : "default");

	s.beginGroup(getSettings(SessionName).toString());
	setSettings(Locale, QLocale(s.value(names[Locale], QLocale::system().name()).toString()));
	setSettings(HideStopButton, s.value(names[HideStopButton], true));
	setSettings(HistoryFilePath, s.value(names[HistoryFilePath], nApp()->getPath() + getSettings(SessionName).toString() + "_history"));
    setSettings(offlineStorage, s.value(names[offlineStorage], true));
    setSettings(offlineCache, s.value(names[offlineCache], true));
    setSettings(localeStorage, s.value(names[localeStorage], true));
    setSettings(dnsPrefetch, s.value(names[dnsPrefetch], true));
    setSettings(loadImages, s.value(names[loadImages], true));
    setSettings(enablePlugins, s.value(names[enablePlugins], true));
    setSettings(enableJavascript, s.value(names[enableJavascript], true));
    setSettings(enableJava, s.value(names[enableJava], false)); //More secure without java
    setSettings(javascriptWindow, s.value(names[javascriptWindow], true));
    setSettings(homePage, s.value(names[homePage], "http://www.google.be"));
    setSettings(downloadDir, s.value(names[downloadDir], QDir::homePath()));
    s.endGroup();

	blockSignals(false);
	emit settingsChanged();
	nApp()->debug("Settings loaded");
}
