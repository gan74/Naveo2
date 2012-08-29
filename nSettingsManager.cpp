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

	timer = new QTimer(this);
	timer->setInterval(2500);
	connect(timer, SIGNAL(timeout()), this, SLOT(save()));
	load();
}

nSettingsManager::~nSettingsManager() {
}

QVariant nSettingsManager::getSettings(Settings s) {
	return settings[s];
}

void nSettingsManager::setSettings(Settings s, QVariant v) {
	if(settings[s] != v) {
		settings[s] = v;
		emit(settingsChanged());
	}
}

void nSettingsManager::save() {
	QSettings s(nApp()->getPath() + "settings.ini", QSettings::IniFormat);
	s.setValue(names[SessionName], getSettings(SessionName).toString());

	s.beginGroup(getSettings(SessionName).toString());
	s.setValue(names[Locale], getSettings(Locale).toLocale().name());
	s.setValue(names[HideStopButton], getSettings(HideStopButton));
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
	s.endGroup();

	blockSignals(false);
	emit settingsChanged();
	nApp()->debug("Settings loaded");
}
