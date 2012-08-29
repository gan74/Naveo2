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
			if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)	{
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
	current = 0;

	setWindowTitle("Naveo2");
	setMinimumSize(300, 200);

	connect(nApp()->getSettingsManager(), SIGNAL(settingsChanged()), this, SLOT(changeSettings()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	tabBar = new nTabBar(this);
	connect(tabBar, SIGNAL(tabMoved(int,int)), this, SLOT(tabMoved(int, int)));
	connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(tabBar, SIGNAL(newTabRequested()), this, SLOT(addTab()));
	layout->addWidget(tabBar);

	toolBar = new QToolBar(this);
	toolBar->setAutoFillBackground(true);
	layout->addWidget(toolBar);

	backAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Back), tr("Back"), this);
	forwardAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Forward), tr("Forward"), this);
	reloadAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Reload), tr("Reload"), this);
	stopAction = new QAction(nApp()->getTheme()->getIcon(nTheme::Stop), tr("Stop"), this);

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

	addTab();

	nApp()->debug("Window created");
}

nWindow::~nWindow() {

}

void nWindow::addTab() {
	nWebView *view = new nWebView();
	connect(view, SIGNAL(titleChanged(QString)), this, SLOT(tabTitleChanged(QString)));

	stack->addWidget(view);
	tabIndexes.append(tabBar->count());
	int index = tabBar->addTab(tr("New tab"));
	tabBar->setCurrentIndex(index);
	if(tabBar->count() == 1) {
		connectTab(view);
	}
}

void nWindow::closeTab(int index) {
	if(tabBar->count() == 1) {
		close();
		return;
	}
	tabBar->removeTab(index);
	nWebView *view = qobject_cast<nWebView *>(stack->widget(tabIndexes[index]));
	view->deleteLater();
	stack->removeWidget(view);
	tabIndexes.removeAt(index);
	for(QList<int>::iterator it = tabIndexes.begin(); it != tabIndexes.end(); it++) {
		if(*it > index) {
			(*it)--;
		}
	}
	connectTab(qobject_cast<nWebView *>(stack->currentWidget()));
}

void nWindow::tabMoved(int from, int to) {
	tabIndexes.swap(from, to);
}

void nWindow::currentTabChanged(int index) {
	stack->setCurrentIndex(tabIndexes[index]);
	connectTab(qobject_cast<nWebView *>(stack->currentWidget()));
}

void nWindow::tabTitleChanged(QString title) {
	if(!title.isEmpty()) {
		tabBar->setTabText(tabIndex(sender()), title);
	}
}

void nWindow::urlEntered(QUrl url) {
	qobject_cast<nWebView *>(stack->currentWidget())->load(url);
}

void nWindow::changeSettings() {
	stopAction->setEnabled(true);
	reloadAction->setEnabled(true);
	stopAction->setVisible(true);
	reloadAction->setVisible(true);
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
