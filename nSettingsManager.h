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

#ifndef NSETTINGS_H
#define NSETTINGS_H

#include <QtCore>

class nSettingsManager : public QObject
{
	Q_OBJECT

	public:
        static const int maxSettings = 15;
		enum Settings {
			SessionName = 0,
			Locale = 1,
			HideStopButton = 2,
            HistoryFilePath = 3,
            OfflineStorage = 4,
            OfflineCache = 5,
            localeStorage = 6,
            dnsPrefetch = 7,
            loadImages = 8,
            enablePlugins = 9,
            enableJavascript = 10,
            enableJava = 11,
            javascriptWindow = 12,
            homePage = 13,
            downloadDir = 14
		};


		nSettingsManager(QObject *parent);
		~nSettingsManager();

		QVariant getSettings(Settings s) const;
        //QVariant getSettings(QString s) const; //Overload.  See below for further information
		void setSettings(Settings s, QVariant v);
        //void setSettings(QString s, QVariant v); //Overload to set a setting witch isn't in Settings

	public slots:
		void save() const;
		void load();

	signals:
		void settingsChanged();

	private:
		void saveEntry(QSettings *out, Settings e) const;
		QTimer *timer;
		QVariant *settings;
		QString *names;
};

#endif // NSETTINGS_H
