#ifndef NSETTINGSWIDGET_H
#define NSETTINGSWIDGET_H

#include <QtGui>
#include <nSettingsManager.h>

class nSettingsWidget : public QWidget
{
    Q_OBJECT

    public:
	   explicit nSettingsWidget(nSettingsManager *man, QWidget *parent = 0);

private slots:
    void saveSettings();

	private:
		nSettingsManager *manager;
        QCheckBox *checkOfflineStorage, *checkOfflineCache, *checkLocaleStorage, *checkDnsPrefetch, *checkLoadImages, *checkEnablePlugins, *checkEnableJavascript, *checkEnableJava, *checkJavascriptWindow;
        QLineEdit *homePageEdit, *downloadDirectoryEdit;

};


#endif // NSETTINGSWIDGET_H
