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

#include <nSearchEngine.h>
#include <nNaveoApplication.h>

nGoogleSearchEngine::nGoogleSearchEngine() {
}

QString nGoogleSearchEngine::process(const QString &s) {
	return processPrivate(s);
}

QString nGoogleSearchEngine::processPrivate(const QString &s) const {
	QString base = QString("http://www.google.com/search?hl=") + nSettings(nSettingsManager::Locale).toLocale().name().split("_").last().toLower() + QString("&q=");
	return base + s.split(" ").join("+");
}


