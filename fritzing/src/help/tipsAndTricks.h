/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2009 Fachhochschule Potsdam - http://fh-potsdam.de

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

$Revision: 2704 $:
$Author: cohen@irascible.com $:
$Date: 2009-03-23 11:28:40 +0100 (Mon, 23 Mar 2009) $

********************************************************************/


#ifndef TIPSANDTRICKS_H
#define TIPSANDTRICKS_H

#include <QDialog>
#include <QEvent>
#include <QTextEdit>
#include <QFile>

#include "../utils/misc.h"

class TipsAndTricks : public QDialog
{
	Q_OBJECT

private:
	TipsAndTricks(QWidget *parent = 0);
	~TipsAndTricks();

public:

public:
	static void hideTipsAndTricks();
	static void showTipsAndTricks();
	static void cleanup();


protected:
	static TipsAndTricks* singleton;

	QTextEdit* m_textEdit;

};

#endif
