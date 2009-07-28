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


#ifndef PARTSEDITORMAINWINDOW_H_
#define PARTSEDITORMAINWINDOW_H_

#include <QMainWindow>
#include <QTabWidget>
#include <QStackedWidget>
#include <QtGui/qwidget.h>

#include "../fritzingwindow.h"
#include "../modelpartshared.h"
#include "connectorsinfowidget.h"
#include "editablelinewidget.h"

QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSplitter)

class PartsEditorMainWindow : public FritzingWindow
{
Q_OBJECT

public:
	PartsEditorMainWindow(long id, QWidget *parent=0, ModelPart *modelPart=0, bool fromTemplate=false);
	~PartsEditorMainWindow();

	static const QString templatePath;
	const QDir& tempDir();

	bool validateMinRequirements();

signals:
	void partUpdated(const QString &filename, long myId);
	void closed(long id);
	void changeActivationSignal(bool activate, QWidget * originator);
	void saveButtonClicked();

public slots:
	void parentAboutToClose();
	bool save();

protected slots:
	void loadPcbFootprint();
	void updateDateAndAuthor();
	bool saveAs();

protected:
	bool createTemplate();
	void saveAsAux(const QString & fileName);
	const QDir& createTempFolderIfNecessary();
	void closeEvent(QCloseEvent *event);
	bool eventFilter(QObject *object, QEvent *event);

	void createHeader(ModelPart * = 0);
	void createCenter(ModelPart * = 0);
	void connectWidgetsToSave(const QList<QWidget*> &widgets);
	void createFooter();

	ModelPartShared* modelPartShared();

	const QString untitledFileName();
	int &untitledFileCount();
	const QString fileExtension();
	const QString defaultSaveFolder();

	void updateSaveButton();
	bool wannaSaveAfterWarning();
	void updateButtons();
	const QString fritzingTitle();

	void cleanUp();
	bool event(QEvent *);

	void makeNonCore();

protected:
	long m_id;

	class PaletteModel *m_paletteModel;
	class SketchModel *m_sketchModel;

	class PartsEditorView *m_iconViewImage;
	EditableLineWidget *m_title;

	class PartsEditorViewsWidget *m_views;

	EditableLineWidget *m_label;
	class EditableTextWidget *m_description;
	//EditableLineWidget *m_taxonomy;
	EditableLineWidget *m_tags;
	class HashPopulateWidget *m_properties;
	EditableLineWidget *m_author;
	class EditableDateWidget *m_createdOn;
	QLabel *m_createdByText;

	ConnectorsInfoWidget *m_connsInfo;

	QString m_version;
	QString m_moduleId;
	QString m_uri;


	QPushButton *m_saveAsNewPartButton;
	QPushButton *m_saveButton;
	QPushButton *m_cancelCloseButton;

	QTabWidget *m_tabWidget;

	QFrame *m_mainFrame;
    QFrame *m_headerFrame;
    QFrame *m_centerFrame;
    QFrame *m_footerFrame;

    bool m_updateEnabled;
    bool m_partUpdated;

    static PartsEditorMainWindow *m_lastOpened;
    static int m_closedBeforeCount;
    static QString ___partsEditorName___;

public:
	static QString TitleFreshStartText;
	static QString LabelFreshStartText;
	static QString DescriptionFreshStartText;
	static QString TaxonomyFreshStartText;
	static QString TagsFreshStartText;
	static QString FooterText;

	static QString UntitledPartName;
	static int UntitledPartIndex;

	static QGraphicsItem *emptyViewItem(QString iconFile, QString text="");
	static void initText();
};
#endif /* PARTSEDITORMAINWINDOW_H_ */
