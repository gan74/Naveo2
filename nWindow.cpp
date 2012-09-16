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

#include <nWindow.h>
#include <nNaveoApplication.h>

#ifdef Q_WS_WIN
	#include <windows.h>
	typedef struct {
		int cxLeftWidth;
		int cxRightWidth;
		int cyTopHeight;
		int cyBottomHeight;
	} MARGINS;
	typedef HRESULT (WINAPI *PtrDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS *margins);
	static PtrDwmExtendFrameIntoClientArea pDwmExtendFrameIntoClientArea = 0;
	typedef HRESULT (WINAPI *PtrDwmIsCompositionEnabled)(BOOL *pfEnabled);
	static PtrDwmIsCompositionEnabled pDwmIsCompositionEnabled = 0;
#endif

nWindow::nWindow(QWidget *parent) : QWidget(parent) {
	if(!parent) {
		#ifdef Q_WS_WIN
			if(QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA) {
				QLibrary *winapi = nApp()->getLibrary("dwmapi");
				pDwmIsCompositionEnabled = (PtrDwmIsCompositionEnabled)winapi->resolve("DwmIsCompositionEnabled");
				BOOL enabled = FALSE;
				pDwmIsCompositionEnabled(&enabled);
				if(enabled) {
					pDwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)winapi->resolve("DwmExtendFrameIntoClientArea");
					MARGINS margins = {-1};
					pDwmExtendFrameIntoClientArea(winId(), &margins);
					setAutoFillBackground(false);
					setAttribute(Qt::WA_TranslucentBackground);
				}
			}
		#endif
	}
	nApp()->registerWindow(this);
	current = 0;

	setWindowTitle("Naveo2");
	setMinimumSize(300, 200);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowIcon(QIcon(":/icon.png")); // change this ?

	connect(nApp()->getSettingsManager(), SIGNAL(settingsChanged()), this, SLOT(changeSettings()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	tabBar = new nTabBar(this);
	connect(tabBar, SIGNAL(tabMoved(int,int)), this, SLOT(tabMoved(int, int)));
	connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(tabBar, SIGNAL(newTabRequested()), this, SLOT(addTab()));

	backAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Back), tr("Back"), this);
	forwardAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Forward), tr("Forward"), this);
	reloadAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Reload), tr("Reload"), this);
	stopAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Stop), tr("Stop"), this);
    QAction *newTab = new QAction(nApp()->getTheme()->getIcon(nTheme::NewTab), tr("New tab"), this);
    newTab->setShortcut(QKeySequence("Ctrl+T"));
    connect(newTab, SIGNAL(triggered()), this, SLOT(addTab()));

	mainMenu = new QMenu(this);
    mainMenu->addAction(newTab);
	mainMenu->addAction(backAction);
	mainMenu->addAction(forwardAction);
	mainMenu->addAction(reloadAction);
	mainMenu->addAction(stopAction);
	mainMenu->addSeparator();
	mainMenu->addAction(tr("Preferences"), nApp(), SLOT(showSettingsPanel()));

	menuButton = new QPushButton(this);
	menuButton->setFixedSize(40, 28);
	menuButton->move(-3, -3);
	menuButton->setStyleSheet("QPushButton{ image: url('" + nApp()->getTheme()->getThemeDir() + "menu.png'); border: none; }");
	connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));

	QHBoxLayout *tabBarLayout = new QHBoxLayout();
	tabBarLayout->addSpacing(34);
	tabBarLayout->addWidget(tabBar);
	layout->addLayout(tabBarLayout);

	toolBar = new QToolBar(this);
	toolBar->setAutoFillBackground(true);
	layout->addWidget(toolBar);


	toolBar->addAction(backAction);
	toolBar->addAction(forwardAction);
	toolBar->addAction(reloadAction);
	toolBar->addAction(stopAction);

	urlEdit = new nUrlLineEdit(this);
	connect(urlEdit, SIGNAL(urlEntered(QUrl)), this, SLOT(urlEntered(QUrl)));
	toolBar->addWidget(urlEdit);

	stack = new QStackedWidget(this);
	stack->setAutoFillBackground(true);
	layout->addWidget(stack);

	//addTab();

	nApp()->debug("Window created");
}

nWindow::~nWindow() {
	nApp()->unregisterWindow(this);
}

nWebView *nWindow::addTab() {
	return addTab(new nWebView());
}

nWebView *nWindow::addTab(nWebView *view) {
	connect(view, SIGNAL(titleChanged(QString)), this, SLOT(tabTitleChanged(QString)));
	connect(view, SIGNAL(viewCreated(nWebView*)), this, SLOT(addTab(nWebView*)));
	stack->addWidget(view);
	tabIndexes.append(tabBar->count());
	int index = tabBar->addTab(tr("New tab"));
	tabBar->setCurrentIndex(index);
	if(tabBar->count() == 1) {
		connectTab(view);
	}
	return view;
}

void nWindow::closeTab(int index) {
	if(tabBar->count() == 1) {
		close();
		return;
	}
	tabBar->removeTab(index);
	nWebView *view = qobject_cast<nWebView *>(stack->widget(tabIndexes[index]));
	stack->removeWidget(view);
	tabIndexes.removeAt(index);
	view->deleteLater();
	for(QList<int>::iterator it = tabIndexes.begin(); it != tabIndexes.end(); it++) {
		if(*it > index) {
			(*it)--;
		}
	}
	connectTab(qobject_cast<nWebView *>(stack->currentWidget()));
}

void nWindow::tabMoved(int from, int to) {
	if(std::max(from, to) < tabBar->count()) {
		tabIndexes.swap(from, to);
	}
}

void nWindow::currentTabChanged(int index) {
	stack->setCurrentIndex(tabIndexes[index]);
	connectTab(qobject_cast<nWebView *>(stack->currentWidget()));
}

void nWindow::tabTitleChanged(const QString &title) {
	if(!title.isEmpty()) {
		tabBar->setTabText(tabIndex(sender()), title);		
		#ifndef NAVEO_DONT_USE_WEBKIT_HISTORY
		nApp()->getHistoryManager()->updateEntries(qobject_cast<QWebView *>(sender())->url(), title); // move this
		#endif
	}
}

void nWindow::urlEntered(const QUrl &url) {
	qobject_cast<nWebView *>(stack->currentWidget())->load(url);
}

void nWindow::changeSettings() {
	stopAction->setEnabled(true);
	reloadAction->setEnabled(true);
	stopAction->setVisible(true);
	reloadAction->setVisible(true);

	// icons & themes
	stopAction->setIcon(nApp()->getTheme()->getIcon(nTheme::Stop));
	reloadAction->setIcon(nApp()->getTheme()->getIcon(nTheme::Reload));
	backAction->setIcon(nApp()->getTheme()->getIcon(nTheme::Back));
	forwardAction->setIcon(nApp()->getTheme()->getIcon(nTheme::Forward));
	menuButton->setIcon(nApp()->getTheme()->getIcon(nTheme::MainMenu));
	menuButton->setStyleSheet("QPushButton{ image: url('" + nApp()->getTheme()->getThemeDir() + "menu.png'); border: none; }");

	connectTab(current);
}

void nWindow::loadProgress(int pro) {
	if(nSettings(nSettingsManager::HideStopButton).toBool()) {
		stopAction->setVisible(pro != 100);
		reloadAction->setVisible(pro == 100);
	} else {
		stopAction->setEnabled(pro != 100);
		reloadAction->setEnabled(pro == 100);
	}
}

void nWindow::connectTab(nWebView *v) {
	if(current) {
		disconnectTab(current);
	}
	if(!v) {
		return;
	}
	urlEdit->setUrl(v->url());
	urlEdit->loadProgress(v->getProgress());
	connect(v, SIGNAL(urlChanged(QUrl)), urlEdit, SLOT(setUrl(QUrl)));
	connect(backAction, SIGNAL(triggered()), v, SLOT(back()));
	connect(forwardAction, SIGNAL(triggered()), v, SLOT(forward()));
	connect(reloadAction, SIGNAL(triggered()), v, SLOT(reload()));
	connect(stopAction, SIGNAL(triggered()), v, SLOT(stop()));
	connect(v, SIGNAL(loadStarted()), urlEdit, SLOT(loadStarted()));
	connect(v, SIGNAL(loadProgress(int)), urlEdit, SLOT(loadProgress(int)));
	connect(v, SIGNAL(loadFinished(bool)), urlEdit, SLOT(loadFinished(bool)));
	connect(v, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	loadProgress(v->getProgress());
	current = v;
}

void nWindow::disconnectTab(nWebView *v) {
	disconnect(v, SIGNAL(urlChanged(QUrl)), urlEdit, SLOT(setUrl(QUrl)));
	disconnect(backAction, SIGNAL(triggered()), v, SLOT(back()));
	disconnect(forwardAction, SIGNAL(triggered()), v, SLOT(forward()));
	disconnect(reloadAction, SIGNAL(triggered()), v, SLOT(reload()));
	disconnect(stopAction, SIGNAL(triggered()), v, SLOT(stop()));
	disconnect(v, SIGNAL(loadStarted()), urlEdit, SLOT(loadStarted()));
	disconnect(v, SIGNAL(loadProgress(int)), urlEdit, SLOT(loadProgress(int)));
	disconnect(v, SIGNAL(loadFinished(bool)), urlEdit, SLOT(loadFinished(bool)));
	disconnect(v, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
}

void nWindow::showMenu() const {
	mainMenu->exec(mapToGlobal(QPoint(0, 24)));
}
