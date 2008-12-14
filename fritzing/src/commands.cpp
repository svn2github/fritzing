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

$Revision$:
$Author$:
$Date$

********************************************************************/


#include "commands.h"
#include "debugdialog.h"
#include "sketchwidget.h"
#include <QtDebug>

int SelectItemCommand::selectItemCommandID = 3;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BaseCommand::BaseCommand(BaseCommand::CrossViewType crossViewType, SketchWidget* sketchWidget, QUndoCommand *parent)
	: QUndoCommand(parent)
{
	m_crossViewType = crossViewType;
	m_sketchWidget = sketchWidget;
}

BaseCommand::CrossViewType BaseCommand::crossViewType() {
	return m_crossViewType;
}

SketchWidget* BaseCommand::sketchWidget() {
	return m_sketchWidget;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AddDeleteItemCommand::AddDeleteItemCommand(SketchWidget* sketchWidget, BaseCommand::CrossViewType crossViewType, QString moduleID, ViewGeometry & viewGeometry, qint64 id, QUndoCommand *parent)
    : BaseCommand(crossViewType, sketchWidget, parent)
{
    m_moduleID = moduleID;
    m_viewGeometry = viewGeometry;
    m_itemID = id;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AddItemCommand::AddItemCommand(SketchWidget* sketchWidget, BaseCommand::CrossViewType crossViewType, QString moduleID, ViewGeometry & viewGeometry, qint64 id, QUndoCommand *parent, bool updateInfoView)
    : AddDeleteItemCommand(sketchWidget, crossViewType, moduleID, viewGeometry, id, parent)
{
	m_doFirstRedo = m_firstRedo = true;
	m_updateInfoView = updateInfoView;
}

void AddItemCommand::undo()
{
    m_sketchWidget->deleteItem(m_itemID, true, true);
}

void AddItemCommand::redo()
{
	if (!m_firstRedo || m_doFirstRedo) {
		m_sketchWidget->addItem(m_moduleID, m_crossViewType, m_viewGeometry, m_itemID);
		m_sketchWidget->selectItem(m_itemID,true,m_updateInfoView);
	}
	m_firstRedo = false;
}

void AddItemCommand::turnOffFirstRedo() {
	m_doFirstRedo = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DeleteItemCommand::DeleteItemCommand(SketchWidget* sketchWidget,BaseCommand::CrossViewType crossViewType,  QString moduleID, ViewGeometry & viewGeometry, qint64 id, QUndoCommand *parent)
    : AddDeleteItemCommand(sketchWidget, crossViewType, moduleID, viewGeometry, id, parent)
{
}

void DeleteItemCommand::undo()
{
    m_sketchWidget->addItem(m_moduleID, m_crossViewType, m_viewGeometry, m_itemID);
}

void DeleteItemCommand::redo()
{
    m_sketchWidget->deleteItem(m_itemID, true, true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MoveItemCommand::MoveItemCommand(SketchWidget* sketchWidget, long itemID, ViewGeometry & oldG, ViewGeometry & newG, QUndoCommand *parent)
    : BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
    m_itemID = itemID;
    m_old = oldG;
    m_new = newG;
}

void MoveItemCommand::undo()
{
    m_sketchWidget->moveItem(m_itemID, m_old);
}

void MoveItemCommand::redo()
{
    m_sketchWidget->moveItem(m_itemID, m_new);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RotateItemCommand::RotateItemCommand(SketchWidget* sketchWidget, long itemID, qreal degrees, QUndoCommand *parent)
    : BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
    m_itemID = itemID;
    m_degrees = degrees;
}

void RotateItemCommand::undo()
{
    m_sketchWidget->rotateItem(m_itemID, -m_degrees);
}

void RotateItemCommand::redo()
{
    m_sketchWidget->rotateItem(m_itemID, m_degrees);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlipItemCommand::FlipItemCommand(SketchWidget* sketchWidget, long itemID, Qt::Orientations orientation, QUndoCommand *parent)
    : BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
    m_itemID = itemID;
    m_orientation = orientation;
}

void FlipItemCommand::undo()
{
    redo();
    //m_sketchWidget->flipItem(m_itemID, m_direction);
}

void FlipItemCommand::redo()
{
    m_sketchWidget->flipItem(m_itemID, m_orientation);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChangeConnectionCommand::ChangeConnectionCommand(SketchWidget * sketchWidget, BaseCommand::CrossViewType crossView,
												 long fromID, const QString & fromConnectorID,
												 long toID, const QString & toConnectorID,
												 bool connect, bool seekLayerKin, bool chain,
												 QUndoCommand * parent)
: BaseCommand(crossView, sketchWidget, parent)
{
	//DebugDialog::debug(QObject::tr("ccc: from %1 %2; to %3 %4").arg(fromID).arg(fromConnectorID).arg(toID).arg(toConnectorID) );
    m_fromID = fromID;
    m_fromConnectorID = fromConnectorID;
    m_toID = toID;
    m_toConnectorID = toConnectorID;
	m_connect = connect;
	m_seekLayerKin = seekLayerKin;
	m_chain = chain;
}

void ChangeConnectionCommand::undo()
{
    m_sketchWidget->changeConnection(m_fromID, m_fromConnectorID, m_toID, m_toConnectorID, !m_connect, m_crossViewType == CrossView, m_seekLayerKin, m_chain);
}

void ChangeConnectionCommand::redo()
{
    m_sketchWidget->changeConnection(m_fromID, m_fromConnectorID, m_toID, m_toConnectorID, m_connect, m_crossViewType == CrossView, m_seekLayerKin, m_chain);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChangeWireCommand::ChangeWireCommand(SketchWidget* sketchWidget, long fromID,
									 QLineF oldLine, QLineF newLine, QPointF oldPos, QPointF newPos, bool useLine,
									 QUndoCommand *parent)
    : BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
    m_fromID = fromID;
	m_oldLine = oldLine;
    m_newLine = newLine;
    m_oldPos = oldPos;
    m_newPos = newPos;
    m_useLine = useLine;
}

void ChangeWireCommand::undo()
{
    m_sketchWidget->changeWire(m_fromID, m_oldLine, m_oldPos, m_useLine);
}

void ChangeWireCommand::redo()
{
    m_sketchWidget->changeWire(m_fromID, m_newLine, m_newPos, m_useLine);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SelectItemCommand::SelectItemCommand(SketchWidget* sketchWidget, SelectItemType type, QUndoCommand *parent)
    : BaseCommand(BaseCommand::CrossView, sketchWidget, parent)
{
	m_type = type;
}

int SelectItemCommand::id() const {
	return selectItemCommandID;
}

void SelectItemCommand::setSelectItemType(SelectItemType type) {
	m_type = type;
}

void SelectItemCommand::copyUndo(SelectItemCommand * sother) {
   	this->m_undoIDs.clear();
   	for (int i = 0; i < sother->m_undoIDs.size(); i++) {
   		this->m_undoIDs.append(sother->m_undoIDs[i]);
  	}
}

void SelectItemCommand::clearRedo() {
	m_redoIDs.clear();
}

bool SelectItemCommand::mergeWith(const QUndoCommand *other)
{
	// "this" is earlier; "other" is later

    if (other->id() != id()) {
        return false;
   	}


    const SelectItemCommand * sother = dynamic_cast<const SelectItemCommand *>(other);
    if (sother == NULL) return false;

	//DebugDialog::debug(QObject::tr("merge %1 other:%2").arg(id()).arg(other->id()));

	//QString s;
   	//for (int i = 0; i < this->m_undoIDs.size(); i++) {
   		//s += QString::number(this->m_undoIDs[i]) + " ";
  	//}
	//DebugDialog::debug(QObject::tr("this undo %1").arg(s));
	//s = "";
   	//for (int i = 0; i < this->m_redoIDs.size(); i++) {
   		//s += QString::number(this->m_redoIDs[i]) + " ";
  	//}
	//DebugDialog::debug(QObject::tr("this redo %1").arg(s));

	//s = "";
   	//for (int i = 0; i < sother->m_undoIDs.size(); i++) {
   		//s += QString::number(sother->m_undoIDs[i]) + " ";
  	//}
	//DebugDialog::debug(QObject::tr("other undo %1").arg(s));
	//s = "";
   	//for (int i = 0; i < sother->m_redoIDs.size(); i++) {
   		//s += QString::number(sother->m_redoIDs[i]) + " ";
  	//}
	//DebugDialog::debug(QObject::tr("other redo %1").arg(s));



   	this->m_redoIDs.clear();
   	for (int i = 0; i < sother->m_redoIDs.size(); i++) {
   		this->m_redoIDs.append(sother->m_redoIDs[i]);
  	}

  	this->setText(sother->text());
    return true;
}

void SelectItemCommand::undo()
{
	selectAllFromStack(m_undoIDs);
}

void SelectItemCommand::redo()
{
	switch( m_type ){
		case NormalSelect:
			selectAllFromStack(m_redoIDs);
			break;
		case SelectAll: 
			m_sketchWidget->selectAllItems(true, true); 
			break;
		case DeselectAll: 
			m_sketchWidget->selectAllItems(false, true); 
			break;
	}
}

void SelectItemCommand::selectAllFromStack(QList<long> & stack) {
	m_sketchWidget->clearSelection();
	for (int i = 0; i < stack.size(); i++) {
		m_sketchWidget->selectItem(stack[i], true);
	}
}

void SelectItemCommand::addUndo(long id) {
	m_undoIDs.append(id);
}

void SelectItemCommand::addRedo(long id) {
	if(m_type == NormalSelect) {
		m_redoIDs.append(id);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChangeZCommand::ChangeZCommand(SketchWidget* sketchWidget, QUndoCommand *parent)
    : BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
}

void ChangeZCommand::addTriplet(long id, qreal oldZ, qreal newZ) {
	m_triplets.insert(id, new RealPair (oldZ, newZ));
}

void ChangeZCommand::undo()
{
   m_sketchWidget->changeZ(m_triplets, first);
}

void ChangeZCommand::redo()
{
   m_sketchWidget->changeZ(m_triplets, second);
}

qreal ChangeZCommand::first(RealPair * pair) {
	return pair->first;
}

qreal ChangeZCommand::second(RealPair * pair) {
	return pair->second;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StickyCommand::StickyCommand(SketchWidget* sketchWidget, long stickTargetID, long stickSourceID, bool stick, QUndoCommand *parent)
: BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
	m_stickTargetID = stickTargetID;
	m_stickSourceID = stickSourceID;
	m_stick = stick;
}

void StickyCommand::undo()
{
	m_sketchWidget->stickem(m_stickTargetID, m_stickSourceID, !m_stick);
}

void StickyCommand::redo()
{
	m_sketchWidget->stickem(m_stickTargetID, m_stickSourceID, m_stick);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CleanUpWiresCommand::CleanUpWiresCommand(SketchWidget* sketchWidget, bool execRedo, QUndoCommand *parent)
: BaseCommand(BaseCommand::CrossView, sketchWidget, parent)
{
	m_execRedo = execRedo;
	m_firstTime = true;
}

void CleanUpWiresCommand::undo()
{
	foreach (BaseCommand * command, m_commands) {
		command->undo();
	}
	if (!m_execRedo) {
		m_sketchWidget->cleanUpWires(true, NULL);
	}
}

void CleanUpWiresCommand::redo()
{
	foreach (BaseCommand * command, m_commands) {
		command->undo();
	}
	if (m_execRedo) {
		m_sketchWidget->cleanUpWires(true, m_firstTime ? this : NULL);
		m_firstTime = false;
	}
}

void CleanUpWiresCommand::addWire(SketchWidget * sketchWidget, Wire * wire) 
{
	m_commands.append(new WireColorChangeCommand(sketchWidget, wire->id(), wire->colorString(), wire->colorString(), wire->opacity(), wire->opacity(), NULL));
	m_commands.append(new WireWidthChangeCommand(sketchWidget, wire->id(), wire->width(), wire->width(), NULL));
	
	foreach (ConnectorItem * toConnectorItem, wire->connector0()->connectedToItems()) {	
		m_commands.append(new ChangeConnectionCommand(sketchWidget, BaseCommand::SingleView, toConnectorItem->attachedToID(), toConnectorItem->connectorStuffID(),
				wire->id(), "connector0", true, true, false, NULL));
	}
	foreach (ConnectorItem * toConnectorItem, wire->connector1()->connectedToItems()) {	
		m_commands.append(new ChangeConnectionCommand(sketchWidget, BaseCommand::SingleView, toConnectorItem->attachedToID(), toConnectorItem->connectorStuffID(),
				wire->id(), "connector1", true, true, false, NULL));
	}

	m_commands.append(new DeleteItemCommand(sketchWidget, BaseCommand::SingleView, Wire::moduleIDName, wire->getViewGeometry(), wire->id(), NULL));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SwapCommand::SwapCommand(SketchWidget* sketchWidget, long itemId, const QString &oldModID, const QString &newModID, QUndoCommand *parent)
: BaseCommand(BaseCommand::CrossView, sketchWidget, parent)
{
	m_itemId = itemId;
	m_oldModuleID = oldModID;
	m_newModuleID = newModID;
}

void SwapCommand::undo() {
	m_sketchWidget->swap(m_itemId, m_oldModuleID, true);
}

void SwapCommand::redo() {
	m_sketchWidget->swap(m_itemId, m_newModuleID, true);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WireColorChangeCommand::WireColorChangeCommand(SketchWidget* sketchWidget, long wireId, const QString &oldColor, const QString &newColor, qreal oldOpacity, qreal newOpacity, QUndoCommand *parent)
: BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
	m_wireId = wireId;
	m_oldColor = oldColor;
	m_newColor = newColor;
	m_oldOpacity = oldOpacity;
	m_newOpacity = newOpacity;
}

void WireColorChangeCommand::undo() {
	m_sketchWidget->changeWireColor(m_wireId, m_oldColor, m_oldOpacity);
}

void WireColorChangeCommand::redo() {
	m_sketchWidget->changeWireColor(m_wireId, m_newColor, m_newOpacity);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WireWidthChangeCommand::WireWidthChangeCommand(SketchWidget* sketchWidget, long wireId, int oldWidth, int newWidth, QUndoCommand *parent)
: BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
	m_wireId = wireId;
	m_oldWidth = oldWidth;
	m_newWidth = newWidth;
}

void WireWidthChangeCommand::undo() {
	m_sketchWidget->changeWireWidth(m_wireId, m_oldWidth);
}

void WireWidthChangeCommand::redo() {
	m_sketchWidget->changeWireWidth(m_wireId, m_newWidth);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WireFlagChangeCommand::WireFlagChangeCommand(SketchWidget* sketchWidget, long wireId, ViewGeometry::WireFlags oldFlags, ViewGeometry::WireFlags newFlags, QUndoCommand *parent)
: BaseCommand(BaseCommand::SingleView, sketchWidget, parent)
{
	m_wireId = wireId;
	m_oldFlags = oldFlags;
	m_newFlags = newFlags;
}

void WireFlagChangeCommand::undo() {
	m_sketchWidget->changeWireFlags(m_wireId, m_oldFlags);
}

void WireFlagChangeCommand::redo() {
	m_sketchWidget->changeWireFlags(m_wireId, m_newFlags);
}

////////////////////////////////////////////

WireChainedIDCommand::WireChainedIDCommand(SketchWidget* sketchWidget, BaseCommand::CrossViewType crossView, qint64 wireID, qint64 chainedID, QUndoCommand *parent)
: BaseCommand(crossView, sketchWidget, parent)
{
	m_wireID = wireID;
	m_chainedID = chainedID;
}

void WireChainedIDCommand::undo() {
}

void WireChainedIDCommand::redo() {
	m_sketchWidget->setChainedWireID(m_wireID, m_chainedID, m_crossViewType);
}

//////////////////////////////////////////


RatsnestCommand::RatsnestCommand(class SketchWidget * sketchWidget, BaseCommand::CrossViewType crossViewType,
									long fromID, const QString & fromConnectorID,
									long toID, const QString & toConnectorID,
									bool connect, bool seekLayerKin,
									bool chain, QUndoCommand * parent) 
: ChangeConnectionCommand(sketchWidget, crossViewType, fromID, fromConnectorID, toID, toConnectorID,
						connect, seekLayerKin, chain, parent)
{
	m_firstTime = true;
}

void RatsnestCommand::undo() {
	foreach (BaseCommand * command, m_commands) {
		command->undo();
	}
}

void RatsnestCommand::redo() {
	if (m_firstTime) {
		m_firstTime = false;
		m_sketchWidget->dealWithRatsnest(m_fromID, m_fromConnectorID, m_toID, m_toConnectorID, m_connect, this, m_crossViewType == BaseCommand::CrossView);
	}
	else {
		foreach (BaseCommand * command, m_commands) {
			command->redo();
		}
	}
}

void RatsnestCommand::addWire(SketchWidget * sketchWidget, Wire * wire, ConnectorItem * source, ConnectorItem * dest) 
{
	m_commands.append(new AddItemCommand(sketchWidget, BaseCommand::SingleView, Wire::moduleIDName, wire->getViewGeometry(), wire->id(), NULL));
	m_commands.append(new WireColorChangeCommand(sketchWidget, wire->id(), wire->colorString(), wire->colorString(), wire->opacity(), wire->opacity(), NULL));
	m_commands.append(new WireWidthChangeCommand(sketchWidget, wire->id(), wire->width(), wire->width(), NULL));
	m_commands.append(new ChangeConnectionCommand(sketchWidget, BaseCommand::SingleView, source->attachedToID(), source->connectorStuffID(),
			wire->id(), "connector0", true, true, false, NULL));
	m_commands.append(new ChangeConnectionCommand(sketchWidget, BaseCommand::SingleView, dest->attachedToID(), dest->connectorStuffID(),
			wire->id(), "connector1", true, true, false, NULL));

}



