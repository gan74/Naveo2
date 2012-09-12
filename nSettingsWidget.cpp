#include <nSettingsWidget.h>
#include <nNaveoApplication.h>

nSettingsWidget::nSettingsWidget(nSettingsManager *man, QWidget *parent) : QWidget(parent){
	manager = man;

    //UTF-8 support
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	setWindowTitle(tr("Naveo settings"));
	setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(280, 280);

    QTabWidget *tabSettings = new QTabWidget(this);
    tabSettings->setTabPosition(QTabWidget::West);


    QPushButton *validate, *quit;
    validate = new QPushButton(tr("Validate"), this); //NEED ICON
    quit = new QPushButton(tr("Quit"), this); //NEED ICON
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));

    //-----------------------------------------------------------
    // TAB : GENERAL SETTINGS
    //-----------------------------------------------------------
    QWidget *generalSettings = new QWidget(tabSettings);
    tabSettings->addTab(generalSettings, tr("General")); //CHANGE ICON
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
    QWidget *internetSettings = new QWidget(tabSettings);
    tabSettings->addTab(internetSettings, tr("Internet")); //ICON
    QLabel *offlineStorage, *offlineCache, *localeStorage, *dnsPrefetch, *loadImages, *enablePlugins, *enableJavascript, *enableJava, *javascriptWindow;
    QCheckBox *checkOfflineStorage, *checkOfflineCache, *checkLocaleStorage, *checkDnsPrefetch, *checkLoadImages, *checkEnablePlugins, *checkEnableJavascript, *checkEnableJava, *checkJavascriptWindow;
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
    internetLayout->addWidget(offlineStorage, 0, 0);
    internetLayout->addWidget(offlineCache, 1, 0);
    internetLayout->addWidget(localeStorage, 2, 0);
    internetLayout->addWidget(dnsPrefetch, 3, 0);
    internetLayout->addWidget(loadImages, 4, 0);
    internetLayout->addWidget(enablePlugins, 5, 0);
    internetLayout->addWidget(enableJavascript, 6, 0);
    internetLayout->addWidget(enableJava, 7, 0);
    internetLayout->addWidget(javascriptWindow, 8, 0);

    internetLayout->addWidget(checkOfflineStorage, 0, 1);
    internetLayout->addWidget(checkOfflineCache, 1, 1);
    internetLayout->addWidget(checkLocaleStorage, 2, 1);
    internetLayout->addWidget(checkDnsPrefetch, 3, 1);
    internetLayout->addWidget(checkLoadImages, 4, 1);
    internetLayout->addWidget(checkEnablePlugins, 5, 1);
    internetLayout->addWidget(checkEnableJavascript, 6, 1);
    internetLayout->addWidget(checkEnableJava, 7, 1);
    internetLayout->addWidget(checkJavascriptWindow, 8, 1);

    //Buttons
    validate->move(30, 275);
    quit->move(180, 275);
}
