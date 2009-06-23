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



#include <QMessageBox>
#include <QMenu>

#include "partseditorpaletteitem.h"
#include "partseditorconnectoritem.h"
#include "partseditorview.h"
#include "partseditorlayerkinpaletteitem.h"
#include "../fsvgrenderer.h"
#include "../debugdialog.h"
#include "../layerattributes.h"
#include "../items/layerkinpaletteitem.h"


PartsEditorPaletteItem::PartsEditorPaletteItem(PartsEditorView *owner, ModelPart * modelPart, ViewIdentifierClass::ViewIdentifier viewIdentifier) :
	PaletteItem(modelPart, viewIdentifier, m_viewGeometry, ItemBase::getNextID(), NULL)
{
	m_owner = owner;

	m_svgDom = NULL;

	m_connectors = NULL;
	m_svgStrings = NULL;
	m_shouldDeletePath = true;
}

PartsEditorPaletteItem::PartsEditorPaletteItem(PartsEditorView *owner, ModelPart * modelPart, ViewIdentifierClass::ViewIdentifier viewIdentifier, SvgAndPartFilePath *path) :
	PaletteItem(modelPart, viewIdentifier, m_viewGeometry, ItemBase::getNextID(), NULL, false)
{
	m_owner = owner;

	m_svgDom = NULL;
	createSvgFile(path->absolutePath());
	m_svgStrings = path;
	m_shouldDeletePath = false;

	m_connectors = NULL;

	setAcceptHoverEvents(false);
	setSelected(false);
}

PartsEditorPaletteItem::~PartsEditorPaletteItem()
{
	if (m_svgDom) {
		delete m_svgDom;
	}
	if (m_connectors) {
		delete m_connectors;
	}
	if (m_shouldDeletePath && m_svgStrings) {
		delete m_svgStrings;
	}
	if (this->renderer()) {
		delete this->renderer();
	}
}

void PartsEditorPaletteItem::createSvgFile(QString path) {
	if (m_svgDom) {
		delete m_svgDom;
	}
    m_svgDom = new QDomDocument();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!m_svgDom->setContent(&file)) {
        file.close();
        return;
    }
    m_originalSvgPath = path;
    file.close();
}

bool PartsEditorPaletteItem::createSvgPath(const QString &modelPartSharedPath, const QString &layerFileName) {
	Q_UNUSED(modelPartSharedPath);

	if(m_shouldDeletePath && m_svgStrings) {
		delete m_svgStrings;
		m_svgStrings = NULL;
	}
	m_shouldDeletePath = true;

	if(QFileInfo(layerFileName).exists()) {
		m_svgStrings = new SvgAndPartFilePath();
		m_svgStrings->setAbsolutePath(layerFileName);
		return true; // nothing to do
	} else {
		StringPair tempPath;
		tempPath.first  = "%1/" + ItemBase::SvgFilesDir;
		tempPath.second = "%2/" + layerFileName;

		QStringList possibleRootFolders;
		possibleRootFolders << getApplicationSubFolderPath("parts") << getUserDataStorePath("parts");
		QStringList possibleFolders;
		possibleFolders << "core" << "contrib" << "user";
		foreach(QString rootFolder, possibleRootFolders) {
			foreach(QString folder, possibleFolders) {
				if (QFileInfo( tempPath.first.arg(rootFolder)+"/"+tempPath.second.arg(folder) ).exists()) {
					m_svgStrings = new SvgAndPartFilePath();
					m_svgStrings->setAbsolutePath(tempPath.first.arg(rootFolder)+"/"+tempPath.second.arg(folder));
					m_svgStrings->setRelativePath(tempPath.second.arg(folder));
					m_svgStrings->setCoreContribOrUser(folder);
					return true;
				}
			}
		}
	}
	return false;
}

void PartsEditorPaletteItem::writeXmlLocation(QXmlStreamWriter & /*streamWriter*/) {
	return;
}

void PartsEditorPaletteItem::writeXml(QXmlStreamWriter & streamWriter) {
	streamWriter.writeStartElement(ViewIdentifierClass::viewIdentifierXmlName(m_viewIdentifier));
	streamWriter.writeStartElement("layers");
	streamWriter.writeAttribute("image",m_svgStrings->relativePath());
		streamWriter.writeStartElement("layer");
		streamWriter.writeAttribute("layerId",xmlViewLayerID());
		streamWriter.writeEndElement();
	foreach (ItemBase * lkpi, m_layerKin) {
		streamWriter.writeStartElement("layer");
		streamWriter.writeAttribute("layerId",ViewLayer::viewLayerXmlNameFromID(lkpi->viewLayerID()));
		streamWriter.writeEndElement();
	}

	streamWriter.writeEndElement();
	streamWriter.writeEndElement();
}

const QList<Connector *> &PartsEditorPaletteItem::connectors() {
	if(!m_connectors) {
		m_connectors = new QList<Connector*>;
		QList<QString> connNames = modelPart()->connectors().keys();
		qSort(connNames);
		foreach(QString connName, connNames) {
			*m_connectors << modelPart()->connectors()[connName];
		}
	}
	return *m_connectors;
}

void PartsEditorPaletteItem::setConnector(const QString &id, Connector *connector) {
	Q_UNUSED(id);
	Q_UNUSED(connector);
}

bool PartsEditorPaletteItem::setUpImage(ModelPart * modelPart, ViewIdentifierClass::ViewIdentifier viewIdentifier, const LayerHash & viewLayers, ViewLayer::ViewLayerID viewLayerID, bool doConnectors)
{
    ModelPartShared * modelPartShared = modelPart->modelPartShared();
    if (modelPartShared == NULL) return false;
    if (modelPartShared->domDocument() == NULL) return false;

	setViewLayerID(viewLayerID, viewLayers);

	if (m_svgStrings == NULL) {
		// TODO Mariano: Copied from paletteitembase::setUpImage (extract what's in common)
		LayerAttributes layerAttributes;
		if (modelPartShared->domDocument() ) {
			bool result = layerAttributes.getSvgElementID(modelPartShared->domDocument(), viewIdentifier, viewLayerID);
			if (!result) return false;
		}

		if(!createSvgPath(modelPartShared->path(), layerAttributes.filename())) {
			//QMessageBox::information( NULL, QObject::tr("Fritzing"),
				//					 QObject::tr("The file %1 is not a Fritzing file (6).").arg(tempPath.arg(possibleFolders[0])));
			return false;
		}
	}


	FSvgRenderer * renderer = NULL;
	if (renderer == NULL) {
		QString fn = m_svgStrings->coreContribOrUser()+(!m_svgStrings->relativePath().isEmpty()?"/"+m_svgStrings->relativePath():"");
		renderer = FSvgRenderer::getByFilename(fn, viewLayerID);
		if (renderer == NULL) {
			renderer = new FSvgRenderer();
			if (!renderer->load(m_svgStrings->absolutePath(), false)) {
				QMessageBox::information( NULL, QObject::tr("Fritzing"),
						QObject::tr("The file %1 is not a Fritzing file (11).").arg(m_svgStrings->absolutePath()));
				delete renderer;
				return false;
			}
		}

		createSvgFile(m_svgStrings->absolutePath());
	}

	this->setZValue(this->z());

	this->setSharedRenderer(renderer);


	m_size = renderer->defaultSize();

	m_svg = true;

	if (doConnectors) {
		setUpConnectors(renderer, modelPartShared->ignoreTerminalPoints());
	}

	return true;
}
SvgAndPartFilePath* PartsEditorPaletteItem::svgFilePath() {
	return m_svgStrings;
}

void PartsEditorPaletteItem::setSvgFilePath(SvgAndPartFilePath *path) {
	if(m_shouldDeletePath && m_svgStrings) {
		delete m_svgStrings;
	}
	m_shouldDeletePath = false;
	m_svgStrings = path;
}

QDomDocument *PartsEditorPaletteItem::svgDom() {
	return m_svgDom;
}

QString PartsEditorPaletteItem::flatSvgFilePath() {
	return m_svgStrings->absolutePath();
}

ConnectorItem* PartsEditorPaletteItem::newConnectorItem(Connector *connector) {
	return new PartsEditorConnectorItem(connector,this);
}

LayerKinPaletteItem * PartsEditorPaletteItem::newLayerKinPaletteItem(
		PaletteItemBase * chief, ModelPart * modelPart, ViewIdentifierClass::ViewIdentifier viewIdentifier,
		const ViewGeometry & viewGeometry, long id,ViewLayer::ViewLayerID viewLayerID, QMenu* itemMenu, const LayerHash & viewLayers)
{
	LayerKinPaletteItem *lk = new
                PartsEditorLayerKinPaletteItem(chief, modelPart, viewIdentifier, viewGeometry, id, itemMenu);
	lk->init(viewLayerID, viewLayers);
	return lk;
}


QString PartsEditorPaletteItem::xmlViewLayerID() {
	ViewLayer::ViewLayerID viewLayerIDAux = m_viewLayerID == ViewLayer::UnknownLayer
		? SketchWidget::defaultConnectorLayer(m_viewIdentifier)
		: m_viewLayerID;
	return ViewLayer::viewLayerXmlNameFromID(viewLayerIDAux);
}

void PartsEditorPaletteItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
	setCursor(QCursor(Qt::ArrowCursor));
	GraphicsSvgLineItem::hoverEnterEvent(event);
}

void PartsEditorPaletteItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
	GraphicsSvgLineItem::hoverLeaveEvent(event);
}
