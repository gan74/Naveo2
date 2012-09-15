#include <nSettingsWidget.h>
#include <nNaveoApplication.h>

nSettingsWidget::nSettingsWidget(nSettingsManager *man, QWidget *parent) : QWidget(parent){
	manager = man;

	setWindowTitle(tr("Naveo settings"));
	setAttribute(Qt::WA_DeleteOnClose);

    QListWidget *settingsList = new QListWidget;
    settingsList->setMaximumWidth(100);
    QStackedWidget *settingsView = new QStackedWidget(this);
    QPushButton *validate, *quit;
    validate = new QPushButton(QIcon(":/validate.png"), tr("Validate"), this);
    quit = new QPushButton(QIcon(":/exit.png"), tr("Quit"), this);
    QGridLayout *settingsWidgetLayout = new QGridLayout(this);

    QListWidgetItem *itemInternet = new QListWidgetItem(QIcon(":/earth.png"), tr("Internet"));
    QListWidgetItem *itemGeneral = new QListWidgetItem(QIcon(":/settings.png"), tr("General"));
    settingsList->addItem(itemInternet);
    settingsList->addItem(itemGeneral);

    settingsWidgetLayout->addWidget(settingsList, 0, 0, 1, 1);
    settingsWidgetLayout->addWidget(settingsView, 0, 1, 1, 2);
    settingsWidgetLayout->addWidget(validate, 1, 1);
    settingsWidgetLayout->addWidget(quit, 1, 2);

    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(settingsList, SIGNAL(currentRowChanged(int)), settingsView, SLOT(setCurrentIndex(int)));

    //-----------------------------------------------------------
    // TAB : GENERAL SETTINGS
    //-----------------------------------------------------------
    QWidget *generalSettings = new QWidget(settingsView);
    settingsView->addWidget(generalSettings);
    QLabel *homePage = new QLabel(tr("Home page : "));
    homePageEdit = new QLineEdit("http://www.google.be", generalSettings);
    QGridLayout *generalLayout = new QGridLayout(generalSettings);
    QLabel *downloadDirectoryLabel = new QLabel(tr("Save downloads in : "));
    downloadDirectoryEdit = new QLineEdit; //GET DEFAULT DIRECTORY (will be saved in QSettings)
    downloadDirectoryEdit->setEnabled(false);
    QPushButton *downloadDirectoryButton = new QPushButton("…");
    downloadDirectoryButton->setMaximumWidth(25);
    generalLayout->addWidget(homePage, 0, 0);
    generalLayout->addWidget(homePageEdit, 1, 0);
    generalLayout->addWidget(downloadDirectoryLabel, 2, 0);
    generalLayout->addWidget(downloadDirectoryEdit, 3, 0);
    generalLayout->addWidget(downloadDirectoryButton, 3, 1);

    //-----------------------------------------------------------
    // TAB : INTERNET SETTINGS
    //-----------------------------------------------------------
    QWidget *internetSettings = new QWidget(settingsView);
    settingsView->addWidget(internetSettings);
    QLabel *offlineStorage, *offlineCache, *localeStorage, *dnsPrefetch, *loadImages, *enablePlugins, *enableJavascript, *enableJava, *javascriptWindow;
    checkOfflineStorage = new QCheckBox;
    checkOfflineStorage->setChecked(manager->getSettings(nSettingsManager::OfflineStorage).toBool());
    checkOfflineCache = new QCheckBox;
    checkOfflineCache->setChecked(manager->getSettings(nSettingsManager::OfflineCache).toBool());
    checkLocaleStorage = new QCheckBox;
    checkLocaleStorage->setChecked(manager->getSettings(nSettingsManager::localeStorage).toBool());
    checkDnsPrefetch = new QCheckBox;
    checkDnsPrefetch->setChecked(manager->getSettings(nSettingsManager::dnsPrefetch).toBool());
    checkLoadImages = new QCheckBox;
    checkLoadImages->setChecked(manager->getSettings(nSettingsManager::loadImages).toBool());
    checkEnablePlugins = new QCheckBox;
    checkEnablePlugins->setChecked(manager->getSettings(nSettingsManager::enablePlugins).toBool());
    checkEnableJavascript = new QCheckBox;
    checkEnableJavascript->setChecked(manager->getSettings(nSettingsManager::enableJavascript).toBool());
    checkEnableJava = new QCheckBox;
    checkEnableJava->setChecked(manager->getSettings(nSettingsManager::enableJava).toBool());
    checkJavascriptWindow = new QCheckBox;
    checkJavascriptWindow->setChecked(manager->getSettings(nSettingsManager::javascriptWindow).toBool());
    offlineStorage = new QLabel(tr("Offline storage"));
    offlineCache = new QLabel(tr("Enable cache"));
    localeStorage = new QLabel(tr("Local Storage"));
    dnsPrefetch = new QLabel(tr("Enable DNS prefetch"));
    loadImages = new QLabel(tr("Load images"));
    enablePlugins = new QLabel(tr("Enable plugins (flash,…)"));
    enableJavascript = new QLabel(tr("Enable Javascript"));
    enableJava = new QLabel(tr("Enable Java"));
    javascriptWindow = new QLabel(tr("Javascript can open windows"));

    QGridLayout *internetLayout = new QGridLayout(internetSettings);
    internetLayout->addWidget(offlineStorage, 0, 1);
    internetLayout->addWidget(offlineCache, 1, 1);
    internetLayout->addWidget(localeStorage, 2, 1);
    internetLayout->addWidget(dnsPrefetch, 3, 1);
    internetLayout->addWidget(loadImages, 4, 1);
    internetLayout->addWidget(enablePlugins, 5, 1);
    internetLayout->addWidget(enableJavascript, 6, 1);
    internetLayout->addWidget(enableJava, 7, 1);
    internetLayout->addWidget(javascriptWindow, 8, 1);

    internetLayout->addWidget(checkOfflineStorage, 0, 0);
    internetLayout->addWidget(checkOfflineCache, 1, 0);
    internetLayout->addWidget(checkLocaleStorage, 2, 0);
    internetLayout->addWidget(checkDnsPrefetch, 3, 0);
    internetLayout->addWidget(checkLoadImages, 4, 0);
    internetLayout->addWidget(checkEnablePlugins, 5, 0);
    internetLayout->addWidget(checkEnableJavascript, 6, 0);
    internetLayout->addWidget(checkEnableJava, 7, 0);
    internetLayout->addWidget(checkJavascriptWindow, 8, 0);

    //Buttons
    validate->move(30, 275);
    quit->move(180, 275);
    connect(validate, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
}

void nSettingsWidget::saveSettings(){
    manager->setSettings(nSettingsManager::OfflineStorage, checkOfflineStorage->isChecked());
    manager->setSettings(nSettingsManager::OfflineCache, checkOfflineCache->isChecked());
    manager->setSettings(nSettingsManager::localeStorage, checkLocaleStorage->isChecked());
    manager->setSettings(nSettingsManager::dnsPrefetch, checkDnsPrefetch->isChecked());
    manager->setSettings(nSettingsManager::loadImages, checkLoadImages->isChecked());
    manager->setSettings(nSettingsManager::enablePlugins, checkEnablePlugins->isChecked());
    manager->setSettings(nSettingsManager::enableJavascript, checkEnableJavascript->isChecked());
    manager->setSettings(nSettingsManager::enableJava, checkEnableJava->isChecked());
    manager->setSettings(nSettingsManager::javascriptWindow, checkJavascriptWindow->isChecked());
    manager->setSettings(nSettingsManager::homePage, ((!homePageEdit->text().isEmpty() && QUrl(homePageEdit->text()).isValid()) ? homePageEdit->text() : "http://www.google.be"));
    manager->setSettings(nSettingsManager::downloadDir, ((!downloadDirectoryEdit->text().isEmpty()) ? downloadDirectoryEdit->text() : QDir::homePath()));
    return;
}
