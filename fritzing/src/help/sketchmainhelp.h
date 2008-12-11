/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-08 Fachhochschule Potsdam - http://fh-potsdam.de

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

$Revision: 1490 $:
$Author: cohen@irascible.com $:
$Date: 2008-11-13 13:10:48 +0100 (Thu, 13 Nov 2008) $

********************************************************************/

/*
 * Central sketch help
 */

#ifndef SKETCHMAINHELP_H_
#define SKETCHMAINHELP_H_

#include <QFrame>
#include <QGraphicsProxyWidget>

class SketchMainHelpPrivate : public QFrame {
public:
	SketchMainHelpPrivate(const QString &imagePath, const QString &htmlText);
};

class SketchMainHelp : public QGraphicsProxyWidget {
public:
	SketchMainHelp(const QString &imagePath, const QString &htmlText);

};

#endif /* SKETCHMAINHELP_H_ */
