#include "nSettingsWidget.h"

nSettingsWidget::nSettingsWidget(QWidget *parent) : QWidget(parent){
}

void nSettingsWidget::createWidget(){
    nApp()->debug("Hello, we are in the settings slot.  SMILE! :D … and code it!");
    return;
}
