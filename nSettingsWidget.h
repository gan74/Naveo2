#ifndef NSETTINGSWIDGET_H
#define NSETTINGSWIDGET_H


#include <QWidget>

#include <nNaveoApplication.h>

class nSettingsWidget : public QWidget
{
    Q_OBJECT

    public:
        nSettingsWidget(QWidget *parent = 0);

public slots:
        void createWidget();

};


#endif // NSETTINGSWIDGET_H
