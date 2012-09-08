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

#include <nUrlLineEdit.h>
#include <nNaveoApplication.h>
#include <algorithm>

nUrlLineEdit::nUrlLineEdit(QWidget *parent) : QLineEdit(parent) {
	movie = new QMovie(":/loading.gif");
	connect(movie, SIGNAL(updated(QRect)), this, SLOT(repaint()));
	connect(this, SIGNAL(returnPressed()), this, SLOT(finished()));
	setTextMargins(20, 0, 40, 0);
	progress = 0;
}

bool nUrlLineEdit::isUrl(QString u) {
	return !u.contains(" ") && (u.contains(".") || u.contains("/"));
}

QUrl nUrlLineEdit::getUrl() {
	QString t = text();
	if(!isUrl(t)) {
		t = nApp()->getSearchEngine()->search(t);
	}
	if(t.left(7) != "http://" && t.left(8) != "https://") {
		t = "http://" + t;
	}
	setText(t);
	return QUrl(t);
}

void nUrlLineEdit::setIcon(QIcon icon) {
	pageIcon = icon;
	if(icon.isNull()) {
		pageIcon = nApp()->getTheme()->getIcon(nTheme::DefaultPage);
	}
}

void nUrlLineEdit::setText(const QString &t) {
	setIcon(nApp()->getWebSettings()->iconForUrl(QUrl(t)));
	QLineEdit::setText(t);
}

void nUrlLineEdit::setUrl(QUrl url) {
	setText(url.toString());
	//finished();
}

void nUrlLineEdit::finished() {
	emit urlEntered(getUrl());
}

void nUrlLineEdit::loadStarted() {
	movie->start();
}

void nUrlLineEdit::loadProgress(int pro) {
	if(movie->state() != QMovie::Running) {
		movie->start();
	}
	if(pro == 100) {
		movie->stop();
		pro = 0;
	}
	progress = pro;
	repaint();
}

void nUrlLineEdit::loadFinished(bool ok) {
	movie->stop();
	progress = -120;
	fade();
}

void nUrlLineEdit::fade() {
	if(progress < 0) {
		progress += 12;
		QTimer::singleShot(40, this, SLOT(fade()));
		repaint();
	}
}

void nUrlLineEdit::paintEvent(QPaintEvent * event) {
	QLineEdit::paintEvent(event);
	QPainter painter(this);
	QPen pen(QColor(81, 119, 189, 120));
	pen.setWidth(height());
	painter.setPen(pen);
	if(progress > 0) {
		painter.drawPixmap((height()-16)/2, (height()-16)/2, 16, 16, movie->currentPixmap());
		int chunk = std::max(1, width());
		chunk = chunk / 100 * progress;
		painter.drawLine(0, height() / 2, chunk, height() / 2);
	} else {
		if(progress) {
			pen.setColor(QColor(81, 119, 189, -progress));
			painter.setPen(pen);
			painter.drawLine(0, height() / 2, width(), height() / 2);
		}
		pageIcon.paint(&painter, (height()-16)/2, (height()-16)/2, 16, 16);
	}
}
