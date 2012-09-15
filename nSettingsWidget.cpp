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
    QLineEdit *homePageEdit = new QLineEdit("http://www.google.be", generalSettings);
    QGridLayout *generalLayout = new QGridLayout(generalSettings);
    QLabel *downloadDirectoryLabel = new QLabel(tr("Save downloads in : "));
    QLineEdit *downloadDirectoryEdit = new QLineEdit; //GET DEFAULT DIRECTORY (will be saved in QSettings)
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
    checkOfflineCache = new QCheckBox;
    checkLocaleStorage = new QCheckBox;
    checkDnsPrefetch = new QCheckBox;
    checkLoadImages = new QCheckBox;
    checkEnablePlugins = new QCheckBox;
    checkEnableJavascript = new QCheckBox;
    checkEnableJava = new QCheckBox;
    checkJavascriptWindow = new QCheckBox;
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

    return;
}
