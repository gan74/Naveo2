#ifndef NSETTINGSWIDGET_H
#define NSETTINGSWIDGET_H

#include <QtGui>
#include <nSettingsManager.h>

class nSettingsWidget : public QWidget
{
    Q_OBJECT

    public:
	   explicit nSettingsWidget(nSettingsManager *man, QWidget *parent = 0);

	private:
		nSettingsManager *manager;

};


#endif // NSETTINGSWIDGET_H
