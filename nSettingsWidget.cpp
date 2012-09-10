#include <nSettingsWidget.h>
#include <nNaveoApplication.h>

nSettingsWidget::nSettingsWidget(nSettingsManager *man, QWidget *parent) : QWidget(parent){
	manager = man;

	setWindowTitle(tr("Naveo settings"));
	setAttribute(Qt::WA_DeleteOnClose);
}
