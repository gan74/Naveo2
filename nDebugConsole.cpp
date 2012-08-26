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

#include <nDebugConsole.h>

nDebugConsole::nDebugConsole(QWidget *parent) : QWidget(parent) {
	setWindowTitle("Naveo debug console");
	QVBoxLayout *layout = new QVBoxLayout(this);

	textEdit = new QPlainTextEdit(this);
	textEdit->setReadOnly(true);
	layout->addWidget(textEdit);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	QPushButton *clearButton = new QPushButton(tr("Clear"));
	connect(clearButton, SIGNAL(clicked()), textEdit, SLOT(clear()));
	buttonLayout->addWidget(clearButton);

	layout->addLayout(buttonLayout);
	resize(500, 400);
}

void nDebugConsole::message(QString msg) {
	textEdit->setPlainText(textEdit->toPlainText() + msg + "\n");
	qDebug(msg.toAscii());
}
