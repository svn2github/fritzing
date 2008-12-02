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



#include <QtGui>
#include <QSvgGenerator>

#include "mainwindow.h"
#include "debugdialog.h"
#include "waitpushundostack.h"
#include "partseditor/mainpartseditorwindow.h"
#include "partseditor/partseditormainwindow.h"
#include "version.h"
#include "bettertriggeraction.h"
#include "aboutbox.h"
#include "autorouter1.h"
#include "eventeater.h"
#include "virtualwire.h"

static QString eagleActionType = ".eagle";
static QString gerberActionType = ".gerber";
static QString jpgActionType = ".jpg";
static QString psActionType = ".ps";
static QString pdfActionType = ".pdf";
static QString pngActionType = ".png";
static QString svgActionType = ".svg";

static QHash<QString, QPrinter::OutputFormat> filePrintFormats;
static QHash<QString, QImage::Format> fileExportFormats;
static QHash<QString, QString> fileExtFormats;


void MainWindow::initExportConstants()
{
	#ifndef QT_NO_PRINTER
		filePrintFormats[pdfActionType] = QPrinter::PdfFormat;
		filePrintFormats[psActionType] = QPrinter::PostScriptFormat;
	#endif

	fileExportFormats[pngActionType] = QImage::Format_ARGB32;
	fileExportFormats[jpgActionType] = QImage::Format_RGB32;

	QString colons ="";
	#ifdef Q_WS_MAC
		colons = ";;";
	#endif

	fileExtFormats[pdfActionType] = tr("PDF (*.pdf)")+colons;
	fileExtFormats[psActionType] = tr("PostScript (*.ps)")+colons;
	fileExtFormats[pngActionType] = tr("PNG Image (*.png)")+colons;
	fileExtFormats[jpgActionType] = tr("JPEG Image (*.jpg)")+colons;
	fileExtFormats[svgActionType] = tr("SVG Image (*.svg)")+colons;
}

void MainWindow::print() {
	#ifndef QT_NO_PRINTER
		QPrinter printer(QPrinter::HighResolution);

		QPrintDialog *printDialog = new QPrintDialog(&printer, this);
		if (printDialog->exec() == QDialog::Accepted) {
			printAux(printer,tr("Printing..."));
			statusBar()->showMessage(tr("Ready"), 2000);
		} else {
			return;
		}
	#endif
}

void MainWindow::exportDiy(QAction * action) {
	Q_UNUSED(action);

	if (!m_pcbGraphicsView->ratsAllRouted()) {
		QMessageBox msgBox;
		msgBox.setText("All traces have not yet been routed.");
		msgBox.setInformativeText("Do you want to proceed anyway?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::Yes);
		int ret = msgBox.exec();
		if (ret != QMessageBox::Yes) return;
	}

	QString path = defaultSaveFolder();

	QString fileExt;
	QString extFmt = fileExtFormats.value(pdfActionType);
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Export for DIY..."),
		path+"/"+m_fileName.remove(FritzingExtension)+getExtFromFileDialog(extFmt),
		extFmt,
		&fileExt
	);

	if (fileName.isEmpty()) {
		return; //Cancel pressed
	}

	DebugDialog::debug(fileExt+" selected to export");
	fileExt = getExtFromFileDialog(fileExt);
	#ifdef Q_WS_X11
		if(!alreadyHasExtension(fileName)) {
			fileName += fileExt;
		}
	#endif

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(filePrintFormats[fileExt]);
	printer.setOutputFileName(fileName);
	QPainter painter;
	if (painter.begin(&printer)) {
		QString svg = m_pcbGraphicsView->renderToSVG(m_printerScale);
		QSvgRenderer svgRenderer;
		svgRenderer.load(svg.toLatin1());
		qreal trueWidth = m_pcbGraphicsView->scene()->width() / m_printerScale;
		qreal trueHeight = m_pcbGraphicsView->scene()->height() / m_printerScale;
		int res = printer.resolution();
		QRectF bounds(0, 0, trueWidth * res, trueHeight * res);
		svgRenderer.render(&painter, bounds);
		painter.end();
		statusBar()->showMessage(tr("Sketch exported"), 2000);
	}


/*
	QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out << svg;
	file.close();
*/


/*

	int width = m_pcbGraphicsView->width();
	if (m_pcbGraphicsView->verticalScrollBar()->isVisible()) {
		width -= m_pcbGraphicsView->verticalScrollBar()->width();
	}
	int height = m_pcbGraphicsView->height();
	if (m_pcbGraphicsView->horizontalScrollBar()->isVisible()) {
		height -= m_pcbGraphicsView->horizontalScrollBar()->height();
	}

	double trueWidth = width / m_printerScale;
	double trueHeight = height / m_printerScale;

	// set everything to a 1200 dpi resolution
	QSize imgSize(trueWidth * 1200, trueHeight * 1200);
	QImage image(imgSize, QImage::Format_RGB32);
	image.setDotsPerMeterX(1200 * 39.3700787);
	image.setDotsPerMeterY(1200 * 39.3700787);
	QPainter painter;

	QColor color;
	color = m_pcbGraphicsView->background();
	m_pcbGraphicsView->setBackground(QColor::fromRgb(255,255,255,255));

	m_pcbGraphicsView->scene()->clearSelection();
	m_pcbGraphicsView->saveLayerVisibility();
	m_pcbGraphicsView->setAllLayersVisible(false);
	m_pcbGraphicsView->setLayerVisible(ViewLayer::Copper0, true);
	m_pcbGraphicsView->hideConnectors(true);

	painter.begin(&image);
	m_pcbGraphicsView->render(&painter);
	painter.end();


	QSvgGenerator svgGenerator;
	svgGenerator.setFileName("c:/fritzing2/testsvggenerator.svg");
    svgGenerator.setSize(QSize(width * 8, height * 8));
	QPainter svgPainter(&svgGenerator);
	m_pcbGraphicsView->render(&svgPainter);
	svgPainter.end();


	m_pcbGraphicsView->hideConnectors(false);
	m_pcbGraphicsView->setBackground(color);
	m_pcbGraphicsView->restoreLayerVisibility();
	// TODO: restore the selection

	QRgb black = 0;
	for (int x = 0; x < imgSize.width(); x++) {
		for (int y = 0; y < imgSize.height(); y++) {
			QRgb p = image.pixel(x, y);
			if (p != 0xffffffff) {
				image.setPixel(x, y, black);
			}
		}
	}

	bool result = image.save(fileName);
	if (!result) {
		QMessageBox::warning(this, tr("fritzing"), tr("Unable to save %1").arg(fileName) );
	}

*/


}

void MainWindow::doExport(QAction * action) {
	QString actionType = action->data().toString();
	QString path = defaultSaveFolder();

	if (actionType.compare(eagleActionType) == 0) {
		exportToEagle();
		return;
	}

	if (actionType.compare(gerberActionType) == 0) {
		exportToGerber();
		return;
	}

	#ifndef QT_NO_PRINTER
		QString fileExt;
		QString extFmt = fileExtFormats.value(actionType);
		DebugDialog::debug(QString("file export string %1").arg(extFmt));
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Export..."), path+"/"+m_fileName.remove(FritzingExtension)+getExtFromFileDialog(extFmt),
			extFmt,
			&fileExt
		);

		if (fileName.isEmpty()) {
			return; //Cancel pressed
		} else {
			DebugDialog::debug(fileExt+" selected to export");
			fileExt = getExtFromFileDialog(fileExt);
			#ifdef Q_WS_X11
				if(!alreadyHasExtension(fileName)) {
					fileName += fileExt;
				}
			#endif

			if(filePrintFormats.contains(fileExt)) { // PDF or PS
				QPrinter printer(QPrinter::HighResolution);
				printer.setOutputFormat(filePrintFormats[fileExt]);
				printer.setOutputFileName(fileName);
				printAux(printer,tr("Exporting..."));
				statusBar()->showMessage(tr("Sketch exported"), 2000);
			} else { // PNG...
				DebugDialog::debug(tr("format: %1 %2").arg(fileExt).arg(fileExportFormats[fileExt]));
				exportAux(fileName,fileExportFormats[fileExt]);
			}
		}
	#endif
}

void MainWindow::exportAux(QString fileName, QImage::Format format) {
	int width = m_currentWidget->width();
	if (m_currentWidget->verticalScrollBar()->isVisible()) {
		width -= m_currentWidget->verticalScrollBar()->width();
	}
	int height = m_currentWidget->height();
	if (m_currentWidget->horizontalScrollBar()->isVisible()) {
		height -= m_currentWidget->horizontalScrollBar()->height();
	}
	QSize imgSize(width, height);
	QImage image(imgSize,format);
	image.setDotsPerMeterX(1200*254);
	image.setDotsPerMeterY(1200*254);
	QPainter painter;

	//QColor color;
	//if(true) {
		//color = m_currentWidget->background();
		//m_currentWidget->setBackground(QColor::fromRgb(255,255,255,255));
	//}

	painter.begin(&image);
	m_currentWidget->render(&painter);
	painter.end();

	//if (true) {
		//m_currentWidget->setBackground(color);
	//}

	//QImage bw = image->createHeuristicMask();  // image->createMaskFromColor (Wire::getRgb("trace"), Qt::MaskOutColor );
	//bool result = bw.save(fileName);
	bool result = image.save(fileName);
	if (!result) {
		QMessageBox::warning(this, tr("fritzing"), tr("Unable to save %1").arg(fileName) );
	}
}

void MainWindow::printAux(QPrinter &printer, QString /* message */, bool removeBackground) {
	QPainter painter;
	if (painter.begin(&printer)) {
		// scale the output
		int res = printer.resolution();

		qreal scale2 = res / m_printerScale;

		DebugDialog::debug(QObject::tr("p.w:%1 p.h:%2 pager.w:%3 pager.h:%4 paperr.w:%5 paperr.h:%6 source.w:%7 source.h:%8")
			.arg(printer.width()).arg(printer.height()).arg(printer.pageRect().width())
			.arg(printer.pageRect().height())
			.arg(printer.paperRect().width()).arg(printer.paperRect().height())
			.arg(printer.width() / scale2)
			.arg(printer.height() / scale2) );

		//QRectF target(0, 0, printer.width(), printer.height());
		//QRectF target = printer.pageRect();
		//QRectF target = printer.paperRect();
		QRectF target;
		QRectF source(0, 0, printer.width() / scale2 , printer.height() / scale2);

		QColor color;
		if(removeBackground) {
			color = m_currentWidget->background();
			m_currentWidget->setBackground(QColor::fromRgb(255,255,255,255));
		}

		QList<QGraphicsItem*> selItems = m_currentWidget->scene()->selectedItems();
		foreach(QGraphicsItem *item, selItems) {
			item->setSelected(false);
		}
		m_currentWidget->scene()->render(&painter, target, source, Qt::KeepAspectRatio);
		foreach(QGraphicsItem *item, selItems) {
			item->setSelected(true);
		}

		if(removeBackground) {
			m_currentWidget->setBackground(color);
		}

		DebugDialog::debug(QObject::tr("source w:%1 h:%2 target w:%5 h:%6 pres:%3 screenres:%4")
			.arg(source.width())
			.arg(source.height()).arg(res).arg(this->physicalDpiX())
			.arg(target.width()).arg(target.height()) );

		//#ifndef QT_NO_CONCURRENT
			//QProgressDialog dialog;
			//dialog.setLabelText(message);
	 	//
			// Create a QFutureWatcher and conncect signals and slots.
			//QFutureWatcher<void> futureWatcher;
			//QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
			//QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
			//QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
			//QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
		//
			// Start the computation.
			//futureWatcher.setFuture(QtConcurrent::run(painter,&QPainter::end));
			//dialog.exec();
		//
			//futureWatcher.waitForFinished();
		//#endif

		//#ifdef QT_NO_CONCURRENT
			painter.end();
		//#endif
	} else {
		QMessageBox::warning(this, tr("Fritzing"),
			tr("Cannot print to %1").arg("print.pdf"));
	}
}

void MainWindow::saveAsAux(const QString & fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Fritzing"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    file.close();

    setReadOnly(false);
    FritzingWindow::saveAsAux(fileName);

    QApplication::setOverrideCursor(Qt::WaitCursor);
	m_sketchModel->save(fileName);
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
    setCurrentFile(fileName);

   // mark the stack clean so we update the window dirty flag
    m_undoStack->setClean();
}

void MainWindow::load() {
	QString path;
	// if it's the first time load is called use Documents folder
	if(m_firstOpen){
		path = defaultSaveFolder();
		m_firstOpen = false;
	}
	else {
		path = "";
	}

	QString fileName = QFileDialog::getOpenFileName(
			this,
			"Select a Fritzing File to Open",
			path,
			tr("Fritzing Files (*%1 *%1z);;Fritzing (*%1);;Fritzing Shareable (*%1z)").arg(FritzingExtension)
		);
	if (fileName.isNull()) return;

    foreach (QWidget * widget, QApplication::topLevelWidgets()) {
        MainWindow * mainWindow = qobject_cast<MainWindow *>(widget);
        if (mainWindow == NULL) continue;

		// don't load two copies of the same file
		if (mainWindow->fileName().compare(fileName) == 0) {
			mainWindow->raise();
			return;
		}
    }

	QFile file(fileName);
	if (!file.exists()) {
       QMessageBox::warning(this, tr("Fritzing"),
                             tr("Cannot find file %1.")
                             .arg(fileName));


		return;
	}



    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Fritzing"),
                             tr("Cannot read file  1 %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    file.close();

    MainWindow* mw = new MainWindow(m_paletteModel, m_refModel);
    if(fileName.endsWith(FritzingExtension)) {
    	mw->load(fileName);
    } else if(fileName.endsWith(FritzingExtension+"z")) {
    	mw->loadBundledSketch(fileName);
    }
    mw->move(x()+CascadeFactor,y()+CascadeFactor);
	mw->show();

	closeIfEmptySketch();
}

void MainWindow::closeIfEmptySketch() {
	// close empty sketch window if user opens from a file
	if (isEmptyFileName(m_fileName, untitledFileName()) && this->undoStackIsEmpty())
	{
		QTimer::singleShot(10, this, SLOT(close()) );
	}
}

void MainWindow::load(const QString & fileName, bool setAsLastOpened, bool addToRecent) {
	m_sketchModel->load(fileName, m_paletteModel, true);
	//m_sketchModel->load(fileName, m_refModel, true);

	m_breadboardGraphicsView->loadFromModel();
	m_pcbGraphicsView->loadFromModel();
	m_schematicGraphicsView->loadFromModel();

	if(setAsLastOpened) {
		QSettings settings("Fritzing","Fritzing");
		settings.setValue("lastOpenSketch",fileName);
	}

	setCurrentFile(fileName, addToRecent);
	UntitledSketchIndex--;
}

void MainWindow::copy() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->copy();
}

void MainWindow::cut() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->cut();
}

void MainWindow::paste() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->paste();
}

void MainWindow::duplicate() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->duplicate();
}

void MainWindow::doDelete() {
	DebugDialog::debug(QString("invoking do delete") );

	if (m_currentWidget != NULL) {
		m_currentWidget->deleteItem();
	}
}

void MainWindow::selectAll() {
	if (m_currentWidget != NULL) {
		m_currentWidget->selectDeselectAllCommand(true);
	}
}

void MainWindow::deselect() {
	if (m_currentWidget != NULL) {
		m_currentWidget->selectDeselectAllCommand(false);
	}
}

void MainWindow::about()
{
	AboutBox::showAbout();
}

void MainWindow::createActions()
{
    createFileMenuActions();
    createEditMenuActions();
    createPartMenuActions();
    createViewMenuActions();
    createWindowMenuActions();
    createHelpMenuActions();
	createTraceMenuActions();
}

void MainWindow::createFileMenuActions() {
	m_newAct = new QAction(tr("&New"), this);
	m_newAct->setShortcut(tr("Ctrl+N"));
	m_newAct->setStatusTip(tr("Create a new sketch"));
	connect(m_newAct, SIGNAL(triggered()), this, SLOT(createNewSketch()));

	/*m_newFromTemplateAct = new QAction(tr("&New from Template..."), this);
	m_newFromTemplateAct->setShortcut(tr("Shift+Ctrl+N"));
	m_newFromTemplateAct->setStatusTip(tr("Create a new sketch from template"));
	connect(m_newFromTemplateAct, SIGNAL(triggered()), this, SLOT(createNewSketchFromTemplate()));
	*/

	m_openAct = new QAction(tr("&Open..."), this);
	m_openAct->setShortcut(tr("Ctrl+O"));
	m_openAct->setStatusTip(tr("Open a sketch"));
	connect(m_openAct, SIGNAL(triggered()), this, SLOT(load()));

	m_openRecentFileMenu = new QMenu(tr("&Open Recent Files"), this);
	createOpenRecentMenu();

	m_openExampleMenu = new QMenu(tr("&Open Example"), this);
	createOpenExampleMenu(m_openExampleMenu,getApplicationSubFolderPath("examples"));

	createCloseAction();

	m_saveAct = new QAction(tr("&Save"), this);
	m_saveAct->setShortcut(tr("Ctrl+S"));
	m_saveAct->setStatusTip(tr("Save the current sketch"));
	connect(m_saveAct, SIGNAL(triggered()), this, SLOT(save()));

	m_saveAsAct = new QAction(tr("&Save As..."), this);
	m_saveAsAct->setShortcut(tr("Shift+Ctrl+S"));
	m_saveAsAct->setStatusTip(tr("Save the current sketch"));
	connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	m_saveAsBundledAct = new QAction(tr("Save As Shareable..."), this);
	m_saveAsBundledAct->setShortcut(tr("Alt+Ctrl+S"));
	m_saveAsBundledAct->setStatusTip(tr("Export current sketch and it's non-core part"));
	connect(m_saveAsBundledAct, SIGNAL(triggered()), this, SLOT(saveBundledSketch()));

	m_exportJpgAct = new BetterTriggerAction(tr("to &JPG..."), this);
	m_exportJpgAct->setData(jpgActionType);
	m_exportJpgAct->setStatusTip(tr("Export the current sketch as a JPG image"));
	connect(m_exportJpgAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportPngAct = new BetterTriggerAction(tr("to P&NG..."), this);
	m_exportPngAct->setData(pngActionType);
	m_exportPngAct->setStatusTip(tr("Export the current sketch as a PNG image"));
	connect(m_exportPngAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportPsAct = new BetterTriggerAction(tr("to Post&Script..."), this);
	m_exportPsAct->setData(psActionType);
	m_exportPsAct->setStatusTip(tr("Export the current sketch as a PostScript image"));
	connect(m_exportPsAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportPdfAct = new BetterTriggerAction(tr("to &PDF..."), this);
	m_exportPdfAct->setData(pdfActionType);
	m_exportPdfAct->setStatusTip(tr("Export the current sketch as a PDF image"));
	connect(m_exportPdfAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportEagleAct = new BetterTriggerAction(tr("to &Eagle..."), this);
	m_exportEagleAct->setData(eagleActionType);
	m_exportEagleAct->setStatusTip(tr("Export the current sketch to Eagle CAD"));
	connect(m_exportEagleAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportGerberAct = new BetterTriggerAction(tr("to &Gerber..."), this);
	m_exportGerberAct->setData(gerberActionType);
	m_exportGerberAct->setStatusTip(tr("Export the current sketch to Gerber"));
	connect(m_exportGerberAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(doExport(QAction *)));

	m_exportDiyAct = new BetterTriggerAction(tr("for &DIY production..."), this);
	m_exportDiyAct->setStatusTip(tr("Export the current sketch to PDF for DIY production"));
	connect(m_exportDiyAct, SIGNAL(betterTriggered(QAction *)), this, SLOT(exportDiy(QAction *)));

	/*m_pageSetupAct = new QAction(tr("&Page Setup..."), this);
	m_pageSetupAct->setShortcut(tr("Shift+Ctrl+P"));
	m_pageSetupAct->setStatusTip(tr("Setup the current sketch page"));
	connect(m_pageSetupAct, SIGNAL(triggered()), this, SLOT(pageSetup()));*/

	m_printAct = new QAction(tr("&Print..."), this);
	m_printAct->setShortcut(tr("Ctrl+P"));
	m_printAct->setStatusTip(tr("Print the current view"));
	connect(m_printAct, SIGNAL(triggered()), this, SLOT(print()));

	m_quitAct = new QAction(tr("&Quit"), this);
	m_quitAct->setShortcut(tr("Ctrl+Q"));
	m_quitAct->setStatusTip(tr("Quit the application"));
	connect(m_quitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createOpenExampleMenu(QMenu * parentMenu, QString path) {
	QDir *currDir = new QDir(path);
	QStringList content = currDir->entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
	if(content.size() > 0) {
		for(int i=0; i < content.size(); i++) {
			QString currFile = content.at(i);
			QString currFilePath = currDir->absolutePath()+"/"+currFile;
			if(QFileInfo(currFilePath).isDir()) {
				QMenu * currMenu = new QMenu(currFile, parentMenu);
				parentMenu->addMenu(currMenu);
				createOpenExampleMenu(currMenu, currFilePath);
			} else {
				QString actionText = currFile.remove(FritzingExtension);
				m_openExampleActions << actionText;
				QAction * currAction = new QAction(actionText, this);
				currAction->setData(currFilePath);
				connect(currAction,SIGNAL(triggered()),this,SLOT(openRecentOrExampleFile()));
				parentMenu->addAction(currAction);
			}
		}
	} else {
		parentMenu->setEnabled(false);
	}
	delete currDir;
}

void MainWindow::createOpenRecentMenu() {
	for (int i = 0; i < MaxRecentFiles; ++i) {
		m_openRecentFileActs[i] = new QAction(this);
		m_openRecentFileActs[i]->setVisible(false);
		connect(m_openRecentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentOrExampleFile()));
	}


    for (int i = 0; i < MaxRecentFiles; ++i) {
    	m_openRecentFileMenu->addAction(m_openRecentFileActs[i]);
    }
    updateRecentFileActions();
}

void MainWindow::updateRecentFileActions() {
	QSettings settings("Fritzing","Fritzing");
	QStringList files = settings.value("recentFileList").toStringList();
	if(files.size() > 0) {
		m_openRecentFileMenu->setEnabled(true);
		int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

		for (int i = 0; i < numRecentFiles; ++i) {
			QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
			m_openRecentFileActs[i]->setText(text);
			m_openRecentFileActs[i]->setData(files[i]);
			m_openRecentFileActs[i]->setVisible(true);
		}

		for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
			m_openRecentFileActs[j]->setVisible(false);
		}
	} else {
		m_openRecentFileMenu->setEnabled(false);
	}
}

void MainWindow::createEditMenuActions() {
	m_undoAct = m_undoGroup->createUndoAction(this);
	m_undoAct->setShortcuts(QKeySequence::Undo);

	m_redoAct = m_undoGroup->createRedoAction(this);
	m_redoAct->setShortcuts(QKeySequence::Redo);

	m_cutAct = new QAction(tr("&Cut"), this);
	m_cutAct->setShortcut(tr("Ctrl+X"));
	m_cutAct->setStatusTip(tr("Cut selection"));
	connect(m_cutAct, SIGNAL(triggered()), this, SLOT(cut()));

	m_copyAct = new QAction(tr("&Copy"), this);
	m_copyAct->setShortcut(tr("Ctrl+C"));
	m_copyAct->setStatusTip(tr("Copy selection"));
	connect(m_copyAct, SIGNAL(triggered()), this, SLOT(copy()));

	m_pasteAct = new QAction(tr("&Paste"), this);
	m_pasteAct->setShortcut(tr("Ctrl+V"));
	m_pasteAct->setStatusTip(tr("Paste clipboard contents"));
	connect(m_pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

	m_duplicateAct = new QAction(tr("&Duplicate"), this);
	m_duplicateAct->setShortcut(tr("Ctrl+D"));
	m_duplicateAct->setStatusTip(tr("Duplicate selection"));
	connect(m_duplicateAct, SIGNAL(triggered()), this, SLOT(duplicate()));

	m_deleteAct = new QAction(tr("&Delete"), this);

	#ifdef Q_WS_MAC
		m_deleteAct->setShortcut(Qt::Key_Backspace);
	#else
		m_deleteAct->setShortcut(QKeySequence::Delete);
	#endif

	m_deleteAct->setStatusTip(tr("Delete selection"));
	connect(m_deleteAct, SIGNAL(triggered()), this, SLOT(doDelete()));

	m_selectAllAct = new QAction(tr("&Select All"), this);
	m_selectAllAct->setShortcut(tr("Ctrl+A"));
	m_selectAllAct->setStatusTip(tr("Select all elements"));
	connect(m_selectAllAct, SIGNAL(triggered()), this, SLOT(selectAll()));

	m_deselectAct = new QAction(tr("&Deselect"), this);
	m_deselectAct->setStatusTip(tr("Deselect"));
	connect(m_deselectAct, SIGNAL(triggered()), this, SLOT(deselect()));

	m_preferencesAct = new QAction(tr("&Preferences..."), this);
	m_preferencesAct->setStatusTip(tr("Show the application's about box"));
	connect(m_preferencesAct, SIGNAL(triggered()), this, SLOT(preferences()));
}

void MainWindow::createPartMenuActions() {
	// TODO PARTS EDITOR REMOVE
    /*m_createNewPartActInOldEditor = new QAction(tr("&Create New Part in Old Editor..."), this);
	connect(m_createNewPartActInOldEditor, SIGNAL(triggered()), this, SLOT(createNewPartInOldEditor()));*/

	m_createNewPart = new QAction(tr("&Create New Part..."), this);
	m_createNewPart->setShortcut(tr("Alt+Ctrl+N"));
	m_createNewPart->setStatusTip(tr("Create new part"));
	connect(m_createNewPart, SIGNAL(triggered()), this, SLOT(createNewPart()));

	m_openInPartsEditorAct = new QAction(tr("&Open in Parts Editor"), this);
	m_openInPartsEditorAct->setShortcut(tr("Ctrl+Return"));
	m_openInPartsEditorAct->setStatusTip(tr("Open the old parts editor"));
	connect(m_openInPartsEditorAct, SIGNAL(triggered()), this, SLOT(openInPartsEditor()));

	m_addToBinAct = new QAction(tr("&Add to bin"), this);
	m_addToBinAct->setStatusTip(tr("Add selected part to bin"));
	connect(m_addToBinAct, SIGNAL(triggered()), this, SLOT(addToBin()));

	// TODO PARTS EDITOR REMOVE
	/*m_openInOldPartsEditorAct = new QAction(tr("&Open in Old Parts Editor"), this);
	connect(m_openInOldPartsEditorAct, SIGNAL(triggered()), this, SLOT(openInOldPartsEditor()));*/

#ifndef QT_NO_DEBUG
	m_infoViewOnHoverAction = new QAction(tr("Update InfoView on hover"), this);
	m_infoViewOnHoverAction->setCheckable(true);
	bool infoViewOnHover = false;
	m_infoViewOnHoverAction->setChecked(infoViewOnHover);
	setInfoViewOnHover(infoViewOnHover);
	connect(m_infoViewOnHoverAction, SIGNAL(toggled(bool)), this, SLOT(setInfoViewOnHover(bool)));
#endif

	// TODO Mariano: DEBUG ACTION
	/*m_swapPartAction = new QAction(tr("Swap part!"), this);
	m_swapPartAction->setStatusTip(tr("Swap part with selected en parts dock"));
	connect(m_swapPartAction, SIGNAL(triggered()), this, SLOT(swapSelected()));*/

	m_rotate90cwAct = new QAction(tr("&Rotate 90\x00B0 Clockwise"), this);
	m_rotate90cwAct->setShortcut(tr("Ctrl+R"));
	m_rotate90cwAct->setStatusTip(tr("Rotate the selected parts by 90 degrees clockwise"));
	connect(m_rotate90cwAct, SIGNAL(triggered()), this, SLOT(rotate90cw()));

	m_rotate180Act = new QAction(tr("&Rotate 180\x00B0"), this);
	m_rotate180Act->setStatusTip(tr("Rotate the selected parts by 180 degrees"));
	connect(m_rotate180Act, SIGNAL(triggered()), this, SLOT(rotate180()));

	m_rotate90ccwAct = new QAction(tr("&Rotate 90\x00B0 Counter Clockwise"), this);
	m_rotate90ccwAct->setShortcut(tr("Alt+Ctrl+R"));
	m_rotate90ccwAct->setStatusTip(tr("Rotate current selection 90 degrees counter clockwise"));
	connect(m_rotate90ccwAct, SIGNAL(triggered()), this, SLOT(rotate90ccw()));

	m_flipHorizontalAct = new QAction(tr("&Flip Horizontal"), this);
	m_flipHorizontalAct->setStatusTip(tr("Flip current selection horizontally"));
	connect(m_flipHorizontalAct, SIGNAL(triggered()), this, SLOT(flipHorizontal()));

	m_flipVerticalAct = new QAction(tr("&Flip Vertical"), this);
	m_flipVerticalAct->setStatusTip(tr("Flip current selection vertically"));
	connect(m_flipVerticalAct, SIGNAL(triggered()), this, SLOT(flipVertical()));

	m_bringToFrontAct = new QAction(tr("Bring to Front"), this);
	m_bringToFrontAct->setShortcut(tr("Shift+Ctrl+]"));
    m_bringToFrontAct->setStatusTip(tr("Bring selected object(s) to front of their layer"));
    connect(m_bringToFrontAct, SIGNAL(triggered()), this, SLOT(bringToFront()));

	m_bringForwardAct = new QAction(tr("Bring Forward"), this);
	m_bringForwardAct->setShortcut(tr("Ctrl+]"));
    m_bringForwardAct->setStatusTip(tr("Bring selected object(s) forward in their layer"));
    connect(m_bringForwardAct, SIGNAL(triggered()), this, SLOT(bringForward()));

	m_sendBackwardAct = new QAction(tr("Send Backward"), this);
	m_sendBackwardAct->setShortcut(tr("Ctrl+["));
    m_sendBackwardAct->setStatusTip(tr("Send selected object(s) back in their layer"));
    connect(m_sendBackwardAct, SIGNAL(triggered()), this, SLOT(sendBackward()));

	m_sendToBackAct = new QAction(tr("Send to Back"), this);
	m_sendToBackAct->setShortcut(tr("Shift+Ctrl+["));
    m_sendToBackAct->setStatusTip(tr("Send selected object(s) to the back of their layer"));
    connect(m_sendToBackAct, SIGNAL(triggered()), this, SLOT(sendToBack()));

    /*m_deleteItemAct = new QAction(tr("&Delete"), this);
    m_deleteItemAct->setShortcut(tr("Delete"));
    m_deleteItemAct->setStatusTip(tr("Delete item from sketch"));
    connect(m_deleteItemAct, SIGNAL(triggered()), this, SLOT(deleteItem()));*/

    m_groupAct = new QAction(tr("&Group"), this);
	m_groupAct->setShortcut(tr("Ctrl+G"));
	m_groupAct->setStatusTip(tr("Group multiple items"));
	connect(m_groupAct, SIGNAL(triggered()), this, SLOT(group()));

	m_showAllLayersAct = new QAction(tr("&Show All Layers"), this);
	m_showAllLayersAct->setStatusTip(tr("Show all the available layers for the current view"));
	connect(m_showAllLayersAct, SIGNAL(triggered()), this, SLOT(showAllLayers()));

	m_hideAllLayersAct = new QAction(tr("&Hide All Layers"), this);
	m_hideAllLayersAct->setStatusTip(tr("Hide all the layers of the current view"));
	connect(m_hideAllLayersAct, SIGNAL(triggered()), this, SLOT(hideAllLayers()));


}

void MainWindow::createViewMenuActions() {
	m_zoomInAct = new QAction(tr("&Zoom In"), this);
	m_zoomInAct->setShortcut(tr("Ctrl++"));
	m_zoomInAct->setStatusTip(tr("Zoom in"));
	connect(m_zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	// instead of creating a filter to grab the shortcut, let's create a new action
	// and append it to the window
	QAction *zoomInAux = new QAction(this);
	zoomInAux->setShortcut(tr("Ctrl+="));
	connect(zoomInAux, SIGNAL(triggered()), this, SLOT(zoomIn()));
	this->addAction(zoomInAux);

	m_zoomOutAct = new QAction(tr("&Zoom Out"), this);
	m_zoomOutAct->setShortcut(tr("Ctrl+-"));
	m_zoomOutAct->setStatusTip(tr("Zoom out"));
	connect(m_zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	m_fitInWindowAct = new QAction(tr("&Fit in Window"), this);
	m_fitInWindowAct->setShortcut(tr("Ctrl+0"));
	m_fitInWindowAct->setStatusTip(tr("Fit in window"));
	connect(m_fitInWindowAct, SIGNAL(triggered()), this, SLOT(fitInWindow()));

	m_actualSizeAct = new QAction(tr("&Actual Size"), this);
	m_actualSizeAct->setShortcut(tr("Shift+Ctrl+0"));
	m_actualSizeAct->setStatusTip(tr("Actual size"));
	connect(m_actualSizeAct, SIGNAL(triggered()), this, SLOT(actualSize()));

	m_showBreadboardAct = new QAction(tr("&Show Breadboard"), this);
	m_showBreadboardAct->setShortcut(tr("Ctrl+1"));
	m_showBreadboardAct->setStatusTip(tr("Show the breadboard view"));
	connect(m_showBreadboardAct, SIGNAL(triggered()), this, SLOT(showBreadboardView()));

	m_showSchematicAct = new QAction(tr("&Show Schematic"), this);
	m_showSchematicAct->setShortcut(tr("Ctrl+2"));
	m_showSchematicAct->setStatusTip(tr("Show the schematic view"));
	connect(m_showSchematicAct, SIGNAL(triggered()), this, SLOT(showSchematicView()));

	m_showPCBAct = new QAction(tr("&Show PCB"), this);
	m_showPCBAct->setShortcut(tr("Ctrl+3"));
	m_showPCBAct->setStatusTip(tr("Show the PCB view"));
	connect(m_showPCBAct, SIGNAL(triggered()), this, SLOT(showPCBView()));
}

void MainWindow::createWindowMenuActions() {
	m_minimizeAct = new QAction(tr("&Minimize"), this);
	m_minimizeAct->setShortcut(tr("Ctrl+M"));
	m_minimizeAct->setStatusTip(tr("Minimize current window"));
	connect(m_minimizeAct, SIGNAL(triggered(bool)), this, SLOT(minimize()));

	/*
	m_toggleToolbarAct = new QAction(tr("&Toolbar"), this);
	m_toggleToolbarAct->setShortcut(tr("Shift+Ctrl+T"));
	m_toggleToolbarAct->setCheckable(true);
	m_toggleToolbarAct->setChecked(true);
	m_toggleToolbarAct->setStatusTip(tr("Toggle Toolbar visibility"));
	connect(m_toggleToolbarAct, SIGNAL(triggered(bool)), this, SLOT(toggleToolbar(bool)));
	*/

    m_toggleDebuggerOutputAct = new QAction(tr("Debugger Output"), this);
    m_toggleDebuggerOutputAct->setCheckable(true);
   	connect(m_toggleDebuggerOutputAct, SIGNAL(triggered(bool)), this, SLOT(toggleDebuggerOutput(bool)));
}

void MainWindow::createHelpMenuActions() {
	m_openHelpAct = new QAction(tr("Learning Fritzing"), this);
	m_openHelpAct->setShortcut(tr("Ctrl+?"));
	m_openHelpAct->setStatusTip(tr("Open Fritzing help"));
	connect(m_openHelpAct, SIGNAL(triggered(bool)), this, SLOT(openHelp()));

	m_examplesAct = new QAction(tr("Example Projects"), this);
	m_examplesAct->setStatusTip(tr("Open Fritzing examples"));
	connect(m_examplesAct, SIGNAL(triggered(bool)), this, SLOT(openExamples()));

	m_partsRefAct = new QAction(tr("Parts Reference"), this);
	m_partsRefAct->setStatusTip(tr("Open Parts Reference"));
	connect(m_partsRefAct, SIGNAL(triggered(bool)), this, SLOT(openPartsReference()));

	m_visitFritzingDotOrgAct = new QAction(tr("Visit fritzing.org"), this);
	m_visitFritzingDotOrgAct->setStatusTip(tr("www.fritzing.org"));
	connect(m_visitFritzingDotOrgAct, SIGNAL(triggered(bool)), this, SLOT(visitFritzingDotOrg()));

	m_aboutAct = new QAction(tr("&About"), this);
	m_aboutAct->setStatusTip(tr("Show the application's about box"));
	connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
    //m_fileMenu->addAction(m_newFromTemplateAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addMenu(m_openRecentFileMenu);
    m_fileMenu->addMenu(m_openExampleMenu);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_closeAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addAction(m_saveAsBundledAct);
    m_fileMenu->addSeparator();
	m_exportMenu = m_fileMenu->addMenu(tr("&Export"));
    //m_fileMenu->addAction(m_pageSetupAct);
    m_fileMenu->addAction(m_printAct);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_quitAct);

	m_exportMenu->addAction(m_exportPdfAct);
	m_exportMenu->addAction(m_exportPsAct);
	m_exportMenu->addAction(m_exportPngAct);
	m_exportMenu->addAction(m_exportJpgAct);
	m_exportMenu->addSeparator();
	m_exportMenu->addAction(m_exportDiyAct);
	m_exportMenu->addAction(m_exportEagleAct);
	m_exportMenu->addAction(m_exportGerberAct);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAct);
    m_editMenu->addAction(m_copyAct);
    m_editMenu->addAction(m_pasteAct);
    m_editMenu->addAction(m_duplicateAct);
    m_editMenu->addAction(m_deleteAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAct);
    m_editMenu->addAction(m_deselectAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_preferencesAct);
    updateEditMenu();
    connect(m_editMenu, SIGNAL(aboutToShow()), this, SLOT(updateEditMenu()));


    m_partMenu = menuBar()->addMenu(tr("&Part"));
    // TODO PARTS EDITOR REMOVE
    //m_partMenu->addAction(m_createNewPartActInOldEditor);
    m_partMenu->addAction(m_createNewPart);

    connect(m_partMenu, SIGNAL(aboutToShow()), this, SLOT(updatePartMenu()));
    // TODO PARTS EDITOR REMOVE
	//m_partMenu->addAction(m_openInOldPartsEditorAct);
	m_partMenu->addAction(m_openInPartsEditorAct);
	m_partMenu->addAction(m_addToBinAct);
	m_partMenu->addSeparator();
	m_partMenu->addAction(m_rotate90cwAct);
	m_partMenu->addAction(m_rotate180Act);
	m_partMenu->addAction(m_rotate90ccwAct);
	m_partMenu->addAction(m_flipHorizontalAct);
	m_partMenu->addAction(m_flipVerticalAct);
	m_partMenu->addSeparator();
	m_partMenu->addAction(m_bringToFrontAct);
	m_partMenu->addAction(m_bringForwardAct);
	m_partMenu->addAction(m_sendBackwardAct);
	m_partMenu->addAction(m_sendToBackAct);
	//m_partMenu->addSeparator();
	//m_partMenu->addAction(m_groupAct);

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_zoomInAct);
    m_viewMenu->addAction(m_zoomOutAct);
    m_viewMenu->addAction(m_fitInWindowAct);
    m_viewMenu->addAction(m_actualSizeAct);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_showBreadboardAct);
    m_viewMenu->addAction(m_showSchematicAct);
    m_viewMenu->addAction(m_showPCBAct);
    m_viewMenu->addSeparator();
    connect(m_viewMenu, SIGNAL(aboutToShow()), this, SLOT(updateLayerMenu()));
    m_numFixedActionsInViewMenu = m_viewMenu->actions().size();

    m_windowMenu = menuBar()->addMenu(tr("&Window"));
	m_windowMenu->addAction(m_minimizeAct);
	m_windowMenu->addSeparator();
	//m_windowMenu->addAction(m_toggleToolbarAct);
	updateWindowMenu();

	m_traceMenu = menuBar()->addMenu(tr("&Trace"));
	m_traceMenu->addAction(m_autorouteAct);
	m_traceMenu->addAction(m_createTraceAct);
	m_traceMenu->addAction(m_createJumperAct);
	updateTraceMenu();
	connect(m_traceMenu, SIGNAL(aboutToShow()), this, SLOT(updateTraceMenu()));


    menuBar()->addSeparator();

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_openHelpAct);
    m_helpMenu->addAction(m_examplesAct);
    m_helpMenu->addAction(m_partsRefAct);
    m_helpMenu->addAction(m_visitFritzingDotOrgAct);
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_aboutAct);
}

void MainWindow::updateLayerMenu() {
	removeActionsStartingAt(m_viewMenu, m_numFixedActionsInViewMenu);
    m_viewMenu->addAction(m_showAllLayersAct);
    m_viewMenu->addAction(m_hideAllLayersAct);


	if (m_currentWidget == NULL) return;

	m_currentWidget->updateLayerMenu(m_viewMenu, m_showAllLayersAct, m_hideAllLayersAct );
}

void MainWindow::updatePartMenu() {
	if (m_currentWidget == NULL) return;

	ItemCount itemCount = m_currentWidget->calcItemCount();

	bool enable = true;

	m_groupAct->setEnabled(itemCount.selCount > 1);

	if (itemCount.selCount <= 0) {
		enable = false;
	}
	else {
		if (itemCount.itemsCount == itemCount.selCount) {
			// if all items are selected
			// z-reordering is a no-op
			enable = false;
		}
	}

	DebugDialog::debug(tr("enable layer actions %1").arg(enable));
	m_bringToFrontAct->setEnabled(enable);
	m_bringForwardAct->setEnabled(enable);
	m_sendBackwardAct->setEnabled(enable);
	m_sendToBackAct->setEnabled(enable);

	enable = (itemCount.selRotatable > 0);

	DebugDialog::debug(tr("enable rotate %1").arg(enable));
	m_rotate90cwAct->setEnabled(enable);
	m_rotate180Act->setEnabled(enable);
	m_rotate90ccwAct->setEnabled(enable);

	m_flipHorizontalAct->setEnabled((itemCount.selHFlipable > 0) && (m_currentWidget != m_pcbGraphicsView));
	m_flipVerticalAct->setEnabled((itemCount.selVFlipable > 0) && (m_currentWidget != m_pcbGraphicsView));

	updateItemMenu();
	updateEditMenu();
}

void MainWindow::updateTransformationActions() {
	if (m_currentWidget == NULL) return;

	ItemCount itemCount = m_currentWidget->calcItemCount();
	bool enable = (itemCount.selRotatable > 0);

	DebugDialog::debug(tr("enable rotate %1").arg(enable));
	m_rotate90cwAct->setEnabled(enable);
	m_rotate180Act->setEnabled(enable);
	m_rotate90ccwAct->setEnabled(enable);
	foreach(SketchToolButton* rotateButton, m_rotateButtons) {
		rotateButton->setEnabled(enable);
	}

	m_flipHorizontalAct->setEnabled((itemCount.selHFlipable > 0) && (m_currentWidget != m_pcbGraphicsView));
	m_flipVerticalAct->setEnabled((itemCount.selVFlipable > 0) && (m_currentWidget != m_pcbGraphicsView));

	enable = m_flipHorizontalAct->isEnabled() || m_flipVerticalAct->isEnabled();
	foreach(SketchToolButton* flipButton, m_flipButtons) {
		flipButton->setEnabled(enable);
	}
}

void MainWindow::updateItemMenu() {
	/*PaletteItem *selInParts = m_paletteWidget->selected();
	PaletteItem *selInSketch = m_currentWidget->selected();
	m_swapPartAction->setEnabled(selInParts && selInSketch && selInParts->family() == selInSketch->family());*/

	if (m_currentWidget == NULL) return;

	QList<QGraphicsItem *> items = m_currentWidget->scene()->selectedItems();

	if (m_currentWidget == m_pcbGraphicsView) {
		if (!m_itemMenu->actions().contains(m_createJumperAct)) {
			QAction * sep = m_itemMenu->addSeparator();
			sep->setObjectName("trace");
			m_itemMenu->addAction(m_createJumperAct);
		}
		if (!m_itemMenu->actions().contains(m_createTraceAct)) {
			m_itemMenu->addAction(m_createTraceAct);
		}

		bool enabled = true;
		int count = 0;
		foreach (QGraphicsItem * item, items) {
			VirtualWire * vw = dynamic_cast<VirtualWire *>(item);
			if (vw == NULL) {
				enabled = false;
				break;
			}

			if (!vw->getRatsnest()) {
				enabled = false;
				break;
			}

			count++;
		}

		// TODO: if there's already a trace or jumper, disable appropriately
		m_createTraceAct->setEnabled(enabled && count > 0);
		m_createJumperAct->setEnabled(enabled && count > 0);
	}
	else {
		if (m_itemMenu->actions().contains(m_createJumperAct)) {
			m_itemMenu->removeAction(m_createJumperAct);
			foreach (QAction * action, m_itemMenu->actions()) {
				if (action->objectName().compare("trace") == 0) {
					m_itemMenu->removeAction(action);
					break;
				}
			}
		}
		if (m_itemMenu->actions().contains(m_createTraceAct)) {
			m_itemMenu->removeAction(m_createTraceAct);
		}
	}

	int selCount = 0;
	ItemBase * itemBase = NULL;
	foreach(QGraphicsItem * item, items) {
		ItemBase * ib = ItemBase::extractTopLevelItemBase(item);
		if (ib == NULL) continue;

		selCount++;
		if (selCount == 1) itemBase = ib;
		else if (selCount > 1) break;
	}

	bool enabled = (selCount == 1) && (dynamic_cast<PaletteItem *>(itemBase) != NULL);

	//TODO PARTS EDITOR REMOVE
	//m_openInOldPartsEditorAct->setEnabled(enabled);
	// can't open wire in parts editor
	m_openInPartsEditorAct->setEnabled(enabled);
	m_addToBinAct->setEnabled(enabled);

}

void MainWindow::updateEditMenu() {
	QClipboard *clipboard = QApplication::clipboard();
	m_pasteAct->setEnabled(false);
	if (clipboard != NULL) {
		const QMimeData *mimeData = clipboard->mimeData(QClipboard::Clipboard);
		if (mimeData != NULL) {
			if (mimeData->hasFormat("application/x-dnditemsdata")) {
				m_pasteAct->setEnabled(true);
				//DebugDialog::debug(tr("paste enabled: true"));
			}
		}
	}

	if (m_currentWidget != NULL) {
		const QList<QGraphicsItem *> items =  m_currentWidget->scene()->selectedItems();
		bool actsEnabled = false;
		foreach (QGraphicsItem * item, items) {
			VirtualWire * wire = dynamic_cast<VirtualWire *>(item);
			if (wire == NULL) {
				ItemBase * itemBase = dynamic_cast<ItemBase *>(item);
				if (itemBase != NULL) {
					ItemBase * chief = itemBase->layerKinChief();
					if (chief != NULL) {
						actsEnabled = true;
						break;
					}
				}
			}
		}

		DebugDialog::debug(tr("enable cut/copy/duplicate/delete %1").arg(actsEnabled));
		m_deleteAct->setEnabled(actsEnabled);
		m_cutAct->setEnabled(actsEnabled);
		m_copyAct->setEnabled(actsEnabled);
		m_duplicateAct->setEnabled(actsEnabled);
	}
}

void MainWindow::updateTraceMenu() {
	bool enabled = false;

	m_autorouteAct->setEnabled(false);
	m_exportDiyAct->setEnabled(false);

	if (m_currentWidget != NULL) {
		if (m_currentWidget == this->m_pcbGraphicsView) {
			QList<QGraphicsItem *> items = m_currentWidget->scene()->items();
			foreach (QGraphicsItem * item, items) {
				VirtualWire * vw = dynamic_cast<VirtualWire *>(item);
				if (vw && vw->getRatsnest()) {
					enabled = true;
					break;
				}
			}
		}
	}

	m_autorouteAct->setEnabled(enabled);
	m_exportDiyAct->setEnabled(enabled);
}


void MainWindow::group() {
	if (m_currentWidget == NULL) return;

	notYetImplemented("Group");
	//m_currentWidget->group();
}

void MainWindow::addToBin() {
	m_paletteWidget->addPart(m_currentWidget->selectedModuleID());
}

void MainWindow::zoomIn() {
	// To zoom throw the combobox options
	zoomIn(1);

	/*if (m_currentWidget == NULL) return;
	m_currentWidget->relativeZoom(ZoomComboBox::ZoomStep);*/
}

void MainWindow::zoomIn(int steps) {
	for(int i=0; i < steps; i++) {
		currentSketchArea()->zoomComboBox()->zoomIn();
	}
}

void MainWindow::zoomOut() {
	// To zoom throw the combobox options
	zoomOut(1);

	/*if (m_currentWidget == NULL) return;
	m_currentWidget->relativeZoom(-ZoomComboBox::ZoomStep);*/
}

void MainWindow::zoomOut(int steps) {
	for(int i=0; i < steps; i++) {
		currentSketchArea()->zoomComboBox()->zoomOut();
	}
}

void MainWindow::fitInWindow() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->fitInWindow();
}

void MainWindow::actualSize() {
	if (m_currentWidget == NULL) return;
	m_currentWidget->absoluteZoom(100);
}

void MainWindow::showBreadboardView() {
	this->m_tabWidget->setCurrentIndex(0);
}

void MainWindow::showSchematicView() {
	this->m_tabWidget->setCurrentIndex(1);

}

void MainWindow::showPCBView() {
	this->m_tabWidget->setCurrentIndex(2);
}

void MainWindow::openHelp() {
	QDesktopServices::openUrl(QString("http://new.fritzing.org/learning"));
}

void MainWindow::openExamples() {
	QDesktopServices::openUrl(QString("http://new.fritzing.org/projects"));
}

void MainWindow::openPartsReference() {
	QDesktopServices::openUrl(QString("http://new.fritzing.org/parts"));
}

void MainWindow::visitFritzingDotOrg() {
	 QDesktopServices::openUrl(QString("http://www.fritzing.org"));
}

void MainWindow::createNewPartInOldEditor() {
	openOldPartsEditor(NULL);
}

// TODO PARTS EDITOR REMOVE
void MainWindow::createNewPart() {
	openPartsEditor(NULL);
}

void MainWindow::openOldPartsEditor(PaletteItem *paletteItem){
	static long nextId = -1;
	ModelPart *modelPart = NULL;
	long id = nextId--;

	if(paletteItem != NULL) {
		modelPart = paletteItem->modelPart();
		id = paletteItem->id();
	}

	MainPartsEditorWindow * mainPartsEditorWindow = new MainPartsEditorWindow(id,this,0,modelPart,modelPart!=NULL);
	connect(mainPartsEditorWindow, SIGNAL(partUpdated(QString)), this, SLOT(loadPart(QString)));
	connect(mainPartsEditorWindow, SIGNAL(closed(long)), this, SLOT(partsEditorClosed(long)));

	mainPartsEditorWindow->show();
	mainPartsEditorWindow->raise();
}

// TODO PARTS EDITOR REMOVE
void MainWindow::openPartsEditor(PaletteItem * paletteItem) {
	static long nextId = -1;
	ModelPart *modelPart = NULL;
	long id = nextId--;

	if(paletteItem != NULL) {
		modelPart = paletteItem->modelPart();
		id = paletteItem->id();
	}

	if(paletteItem != NULL) {
		modelPart = paletteItem->modelPart();
	}

	PartsEditorMainWindow * mainPartsEditorWindow = new PartsEditorMainWindow(id,this,0,modelPart,modelPart!=NULL);
	connect(mainPartsEditorWindow, SIGNAL(partUpdated(QString)), this, SLOT(loadPart(QString)));
	connect(mainPartsEditorWindow, SIGNAL(closed(long)), this, SLOT(partsEditorClosed(long)));
	connect(this, SIGNAL(aboutToClose()), mainPartsEditorWindow, SLOT(parentAboutToClose()));

	m_partsEditorWindows.insert(id, mainPartsEditorWindow);
	mainPartsEditorWindow->show();
	mainPartsEditorWindow->raise();
}

void MainWindow::partsEditorClosed(long id) {
	m_partsEditorWindows.remove(id);
}

void MainWindow::openInOldPartsEditor() {
	// TODO: check to see if part is already open in a part editor window
	if (m_currentWidget == NULL) return;
	PaletteItem *selectedPart = m_currentWidget->getSelectedPart();

	openOldPartsEditor(selectedPart);
}

// TODO PARTS EDITOR REMOVE
void MainWindow::openInPartsEditor() {
	if (m_currentWidget == NULL) return;

	PaletteItem *selectedPart = m_currentWidget->getSelectedPart();
	PartsEditorMainWindow * window = m_partsEditorWindows.value(selectedPart->id());

	if(window != NULL) {
		window->raise();
	}
	else {
		openPartsEditor(selectedPart);
	}
}

void MainWindow::createNewSketch() {
    MainWindow* mw = new MainWindow(m_paletteModel, m_refModel);
    mw->move(x()+CascadeFactor,y()+CascadeFactor);
    mw->show();

    QSettings settings("Fritzing","Fritzing");
    settings.remove("lastOpenSketch");
}

void MainWindow::createNewSketchFromTemplate() {
	notYetImplemented("Create New Sketch From Template");
}

void MainWindow::minimize() {
	this->showMinimized();
}

void MainWindow::toggleToolbar(bool toggle) {
	Q_UNUSED(toggle);
	/*if(toggle) {
		this->m_fileToolBar->show();
		this->m_editToolBar->show();
	} else {
		this->m_fileToolBar->hide();
		this->m_editToolBar->hide();
	}*/
}

void MainWindow::togglePartLibrary(bool toggle) {
	if(toggle) {
		m_paletteWidget->show();
	} else {
		m_paletteWidget->hide();
	}
}

void MainWindow::toggleInfo(bool toggle) {
	if(toggle) {
		((QDockWidget*)m_infoView->parent())->show();
	} else {
		((QDockWidget*)m_infoView->parent())->hide();
	}
}

void MainWindow::toggleNavigator(bool toggle) {
	if(toggle) {
		((QDockWidget*)m_miniViewContainerBreadboard->parent())->show();
		((QDockWidget*)m_miniViewContainerSchematic->parent())->show();
		((QDockWidget*)m_miniViewContainerPCB->parent())->show();
	} else {
		((QDockWidget*)m_miniViewContainerBreadboard->parent())->hide();
		((QDockWidget*)m_miniViewContainerSchematic->parent())->hide();
		((QDockWidget*)m_miniViewContainerPCB->parent())->hide();
	}
}

void MainWindow::toggleUndoHistory(bool toggle) {
	if(toggle) {
		((QDockWidget*)m_undoView->parent())->show();
	} else {
		((QDockWidget*)m_undoView->parent())->hide();
	}
}

void MainWindow::toggleDebuggerOutput(bool toggle) {
	if (toggle) {
		DebugDialog::showDebug();
	} else {
		DebugDialog::hideDebug();
	}
}

void MainWindow::updateWindowMenu() {
	m_toggleDebuggerOutputAct->setChecked(DebugDialog::visible());
}

void MainWindow::pageSetup() {
	notYetImplemented("Page Setup");
}

void MainWindow::preferences() {
	QString text =
		tr("This will soon provide the ability to set some preferences. "
		"such as your default sketch folder, your fritzing.org login name, etc.\n"
		"Please stay tuned.");
	QMessageBox::information(this, tr("Fritzing"), text);
}

void MainWindow::notYetImplemented(QString action) {
	QMessageBox::warning(this, tr("Fritzing"),
				tr("Sorry, \"%1\" has not been implemented yet").arg(action));
}


void MainWindow::rotate90cw() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->rotateX(90);
}

void MainWindow::rotate90ccw() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->rotateX(-90);
}

void MainWindow::rotate180() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->rotateX(180);
}

void MainWindow::flipHorizontal() {
	m_currentWidget->flip(Qt::Horizontal);
}

void MainWindow::flipVertical() {
	m_currentWidget->flip(Qt::Vertical);
}

void MainWindow::sendToBack() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->sendToBack();
}

void MainWindow::sendBackward() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->sendBackward();
}

void MainWindow::bringForward() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->bringForward();
}

void MainWindow::bringToFront() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->bringToFront();
}

void MainWindow::showAllLayers() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->setAllLayersVisible(true);
}

void MainWindow::hideAllLayers() {
	if (m_currentWidget == NULL) return;

	m_currentWidget->setAllLayersVisible(false);
}

void MainWindow::openRecentOrExampleFile() {
	QAction *action = qobject_cast<QAction *>(sender());
	if (action) {
		MainWindow* mw = new MainWindow(m_paletteModel, m_refModel);
		bool readOnly = m_openExampleActions.contains(action->text());
		mw->setReadOnly(readOnly);
		mw->load(action->data().toString(),!readOnly,!readOnly);
		mw->move(x()+CascadeFactor,y()+CascadeFactor);
		mw->show();
	}
}

void MainWindow::removeActionsStartingAt(QMenu * menu, int start) {
	QList<QAction*> actions = menu->actions();

	if(start == 0) {
		menu->clear();
	} else {
		for(int i=start; i < actions.size(); i++) {
			menu->removeAction(actions.at(i));
		}
	}
}

void MainWindow::exportToGerber() {
	QString svg = m_pcbGraphicsView->renderToSVG(m_printerScale);
	if (svg.isEmpty()) {
		// tell the user something reasonable
		return;
	}

	QDomDocument domDocument;
	QString errorStr;
	int errorLine;
	int errorColumn;
	bool result = domDocument.setContent(svg, &errorStr, &errorLine, &errorColumn);
	if (!result) {
		// tell the user something reasonable
		return;
	}

	// Brendan's work starts here

}

void MainWindow::exportToEagle() {
	QString text =
		tr("This will soon provide an export of your Fritzing sketch to the EAGLE layout "
		"software. If you'd like to have more exports to your favourite EDA tool, please let "
		"us know, or contribute.");
	QMessageBox::information(this, tr("Fritzing"), text);
}

void MainWindow::hideShowTraceMenu() {
	m_traceMenu->menuAction()->setVisible(m_currentWidget == m_pcbGraphicsView);
}

void MainWindow::createTraceMenuActions() {
	m_autorouteAct = new QAction(tr("&Autoroute"), this);
	m_autorouteAct->setStatusTip(tr("Autoroute..."));
	connect(m_autorouteAct, SIGNAL(triggered()), this, SLOT(autoroute()));

	m_createTraceAct = new QAction(tr("&Create Trace"), this);
	m_createTraceAct->setStatusTip(tr("Create a trace from the selected wire"));
	connect(m_createTraceAct, SIGNAL(triggered()), this, SLOT(createTrace()));

	m_createJumperAct = new QAction(tr("&Create Jumper Wire"), this);
	m_createJumperAct->setStatusTip(tr("Create a jumper wire from the selected wire"));
	connect(m_createJumperAct, SIGNAL(triggered()), this, SLOT(createJumper()));
}

void MainWindow::autoroute() {
	m_routingStatusLabel->setText(tr("Autorouting..."));
	m_dontClose = true;
	EventEater eater(this);
	qApp->installEventFilter(&eater);
	QProgressDialog progress(QObject::tr("Autorouting..."), QObject::tr("Cancel"), 0, 1, this);
	progress.show();

	eater.allowEventsIn(&progress);

	m_pcbGraphicsView->scene()->clearSelection();
	QApplication::processEvents();
	m_pcbGraphicsView->setIgnoreSelectionChangeEvents(true);
	Autorouter1 * autorouter1 = new Autorouter1(m_pcbGraphicsView);
	autorouter1->start(&progress);
	m_pcbGraphicsView->setIgnoreSelectionChangeEvents(false);
	qApp->removeEventFilter(&eater);
	m_dontClose = false;

	m_pcbGraphicsView->updateRatsnestStatus();
}

void MainWindow::createTrace() {
	m_pcbGraphicsView->createTrace();
}

void MainWindow::createJumper() {
	m_pcbGraphicsView->createJumper();
}

void MainWindow::setDontClose(bool dontClose) {
	m_dontClose = dontClose;
}

void MainWindow::ensureClosable() {
	m_dontClose = false;
}
