/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2012 Fachhochschule Potsdam - http://fh-potsdam.de

Fritzing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fritzing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fritzing.  If not, see <http://www.gnu.org/licenses/>.

********************************************************************

$Revision$:
$Author$:
$Date$

********************************************************************/

#include "drcresultsdialog.h"
#include "../debugdialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>

/////////////////////////////////////

DRCResultsDialog::DRCResultsDialog(const QString & message, const QStringList & messages, QWidget *parent) : QDialog(parent) 
{
	this->setWindowTitle(tr("DRC Results"));

	QVBoxLayout * vLayout = new QVBoxLayout(this);

	QLabel * label = new QLabel(message);
	vLayout->addWidget(label);

    QListWidget *listWidget = new QListWidget();
    listWidget->addItems(messages);
    vLayout->addWidget(listWidget);

	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

	vLayout->addWidget(buttonBox);
	this->setLayout(vLayout);
}

DRCResultsDialog::~DRCResultsDialog() {
}
