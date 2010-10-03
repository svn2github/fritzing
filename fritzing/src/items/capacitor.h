/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2010 Fachhochschule Potsdam - http://fh-potsdam.de

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

$Revision: 4309 $:
$Author: cohen@irascible.com $:
$Date: 2010-07-01 22:27:57 +0200 (Thu, 01 Jul 2010) $

********************************************************************/

#ifndef CAPACITOR_H
#define CAPACITOR_H

#include <QRectF>
#include <QPainterPath>
#include <QPixmap>
#include <QVariant>

#include "paletteitem.h"

struct PropertyDef {
	QString name;
	QString symbol;
	qreal minValue;
	qreal maxValue;
	qreal defaultValue;
	QList<qreal> menuItems;
};

class Capacitor : public PaletteItem 
{
	Q_OBJECT

public:
	// after calling this constructor if you want to render the loaded svg (either from model or from file), MUST call <renderImage>
	Capacitor(ModelPart *, ViewIdentifierClass::ViewIdentifier, const ViewGeometry & viewGeometry, long id, QMenu * itemMenu, bool doLabel);
	~Capacitor();

	PluralType isPlural();
	bool collectExtraInfo(QWidget * parent, const QString & family, const QString & prop, const QString & value, bool swappingEnabled, QString & returnProp, QString & returnValue, QWidget * & returnWidget);
	void setProp(const QString & prop, const QString & value);

protected:
	void loadPropertyDefs();
	void initPropertyDefs();

public slots:
	void propertyEntry(const QString & text);

protected:
	QHash<PropertyDef *, QString> m_propertyDefs;
	QHash<PropertyDef *, class FocusOutComboBox *> m_comboBoxes;
};

#endif // CAPACITOR_H
