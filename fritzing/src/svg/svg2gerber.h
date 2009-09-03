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

$Revision$:
$Author$:
$Date$

********************************************************************/

#ifndef SVG2GERBER_H
#define SVG2GERBER_H

#include <QString>
#include <QDomElement>
#include <QObject>
#include <QMatrix>

class SVG2gerber : public QObject
{
    Q_OBJECT

public:
    SVG2gerber(QString svgStr, QString debugStr="dbg");
    QString getGerber();
    QString getSolderMask();
    QString getContour();
    QString getNCDrill();

protected:
    QDomDocument m_SVGDom;
    QString m_gerber_header;
    QString m_gerber_paths;
    QString m_soldermask_header;
    QString m_soldermask_paths;
    QString m_contour_header;
    QString m_contour_paths;
    QString m_drill_header;
    QString m_drill_paths;

    qreal m_pathstart_x;
    qreal m_pathstart_y;

    void normalizeSVG();
    void convertShapes2paths(QDomNode);
    void flattenSVG(QDomNode);
    QMatrix parseTransform(QDomElement);

    QDomElement rect2path(QDomElement);
    QDomElement circle2path(QDomElement);
    QDomElement line2path(QDomElement);
    QDomElement poly2path(QDomElement);
    QDomElement ellipse2path(QDomElement);

    void copyStyles(QDomElement, QDomElement);

    void renderGerber();
    void allPaths2gerber();
    QString path2gerber(QDomElement);

protected slots:
    void path2gerbCommandSlot(QChar command, bool relative, QList<double> & args, void * userData);


};

#endif // SVG2GERBER_H
