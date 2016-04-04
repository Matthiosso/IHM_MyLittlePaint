//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS MAINWINDOW || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || IMPLEMENTATION FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Libraries
#include "MainWindow.h"
#include "control.h"

#include <QApplication>
#include <QMessageBox>
#include <QColorDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Generate MVC
    model = new Model;
    view = new View;
    control = new Control(model, *view);

    // Create interface
    createActions();
    createMenu();
    createToolBar();

    // Create table
    createDrawSpace();


    // Window design
    setWindowTitle("My Catsome Painter");
    setWindowIcon(QIcon(":/icones/finalChaton"));
    setWindowState(this->windowState() ^ Qt::WindowMaximized);

}

MainWindow::~MainWindow()
{
    // To avoid memory leak
    delete control;
    delete view;
    delete model;
}

//Generate interface
void MainWindow::createActions()
{
    // Actions ( <=> buttons in menu )

    // Group of actions for tools
    actionGroupTools = new QActionGroup(this);

    // Action to change the pen color
    actionColorPen = new QAction("Choose c&olor",this);
    actionColorPen->setShortcut(QKeySequence("CTRL+C"));
    actionColorPen->setIcon(QIcon(":/icones/palette"));

    // Action to clear the image
    actionClear = new QAction("&Clear screen",this);
    actionClear->setShortcut(QKeySequence(QKeySequence::Delete));
    actionClear->setIcon(QIcon(":/icones/ben"));

    // Action to choose the scribble tool
    actionScribble = new QAction("Scri&bble",this);
    actionScribble->setIcon(QIcon(":/icones/pen"));
    actionScribble->setCheckable(true);
    actionScribble->setChecked(true);
    actionGroupTools->addAction(actionScribble);

    // Action to choose the rectangle tool
    actionDrawRectangle = new QAction("Draw a &rectangle",this);
    actionDrawRectangle->setIcon(QIcon(":/icones/rectangle"));
    actionDrawRectangle->setCheckable(true);
    actionGroupTools->addAction(actionDrawRectangle);

    // Action to choose the ellipse tool
    actionDrawEllipse = new QAction("Draw an &ellipse",this);
    actionDrawEllipse->setIcon(QIcon(":/icones/circle"));
    actionDrawEllipse->setCheckable(true);
    actionGroupTools->addAction(actionDrawEllipse);

    // Action to choose the line tool
    actionDrawLine = new QAction("Draw a &line",this);
    actionDrawLine->setIcon(QIcon(":/icones/line"));
    actionDrawLine->setCheckable(true);
    actionGroupTools->addAction(actionDrawLine);

    // Action to choose the filling tool
    actionFilling = new QAction("Fill &zone",this);
    actionFilling->setIcon(QIcon(":/icones/fill"));
    actionFilling->setCheckable(true);
    actionGroupTools->addAction(actionFilling);

    // Action to set the fill mode (to draw a full figure)
    actionFillMode = new QAction("&Fill figure", this);
    actionFillMode->setCheckable(true);
    actionFillMode->setIcon(QIcon(":/icones/empty"));

    // Action to learn more about the program
    actionSource = new QAction("About &sources",this);
    actionSource->setShortcut(QKeySequence("CTRL+I"));

    // Action to learn more about Qt
    actionAboutQt = new QAction("About &Qt",this);

    // Action to go back (example: after a mistake)
    actionGoBack = new QAction("Go &back",this);
    actionGoBack->setShortcut(QKeySequence::Undo);
    actionGoBack->setIcon(QIcon(":/icones/back"));

    // Action to save the image
    actionSave = new QAction("&Save as...",this);
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setIcon(QIcon(":/icones/save"));

    // Action to open an image
    actionOpen = new QAction("&Open file",this);
    actionOpen->setShortcut(QKeySequence::Open);
    actionOpen->setIcon(QIcon(":/icones/open"));

    // Action to create a new image
    actionNewFile = new QAction("&New file", this);
    actionNewFile->setShortcut(QKeySequence::New);
    actionNewFile->setIcon(QIcon(":/icones/new"));

    // Action to print the image
    actionPrintFile = new QAction("&Print", this);
    actionPrintFile->setShortcut(QKeySequence::Print);
    actionPrintFile->setIcon(QIcon(":/icones/print"));

    // Action to quit the program
    actionQuit = new QAction("&Quit", this);
    actionQuit->setShortcut(QKeySequence::Quit);

    // Action to choose the pen width
    boxWidthPen = new QSpinBox;
    boxWidthPen->setValue(view->penWidth());
    boxWidthPen->setFocusPolicy(Qt::NoFocus);
    boxWidthPen->setMinimum(1);

    //Group Of Tools properties:
    actionGroupTools->setExclusive(true);


    // Connections
    connect(actionColorPen, SIGNAL(triggered(bool)), this, SLOT(changePenColor()));
    connect(boxWidthPen, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth()));
    connect(actionClear, SIGNAL(triggered(bool)), view, SLOT(clearImage()));
    connect(actionSource, SIGNAL(triggered(bool)), SLOT(about()));
    connect(actionScribble, SIGNAL(toggled(bool)), this, SLOT(scribbleTool(bool)));
    connect(actionDrawRectangle, SIGNAL(toggled(bool)), this, SLOT(rectangleTool(bool)));
    connect(actionDrawEllipse, SIGNAL(toggled(bool)), this, SLOT(ellipseTool(bool)));
    connect(actionDrawLine, SIGNAL(toggled(bool)), this, SLOT(lineTool(bool)));
    connect(actionFilling, SIGNAL(toggled(bool)), this, SLOT(fillingTool(bool)));
    connect(actionFillMode, SIGNAL(toggled(bool)), this, SLOT(fillMode(bool)));
    connect(actionSave, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(actionGoBack, SIGNAL(triggered(bool)), this, SLOT(goBack()));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(actionNewFile, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(actionPrintFile, SIGNAL(triggered(bool)), this, SLOT(printFile()));
    connect(actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void MainWindow::createMenu()
{
    // The menu for action about the project:
    QMenu *menuFile = menuBar()->addMenu("&File");
    menuFile->addAction(actionSave);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionNewFile);
    menuFile->addAction(actionPrintFile);
    menuFile->addAction(actionQuit);

    // For tools
    QMenu *menuTools = menuBar()->addMenu("&Tools");
    menuTools->addAction(actionClear);
    menuTools->addAction(actionScribble);
    menuTools->addAction(actionDrawRectangle);
    menuTools->addAction(actionDrawEllipse);
    menuTools->addAction(actionDrawLine);
    menuTools->addAction(actionFilling);
    menuTools->addAction(actionGoBack);

    // For properties
    QMenu *menuProperties = menuBar()->addMenu("&Properties");
    menuProperties->addAction(actionColorPen);
    menuProperties->addAction(actionFillMode);

    // For help
    QMenu *menuHelp = menuBar()->addMenu("&?");
    menuHelp->addAction(actionSource);
    menuHelp->addAction(actionAboutQt);

}

void MainWindow::createToolBar()
{
    //  To add our actions to the toolBar:
    QToolBar *toolBar = addToolBar("Tools");
    QWidget *spacer;
    spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addAction(actionScribble);
    toolBar->addAction(actionDrawRectangle);
    toolBar->addAction(actionDrawEllipse);
    toolBar->addAction(actionDrawLine);
    toolBar->addAction(actionFilling);
    toolBar->addSeparator();
    toolBar->addAction(actionFillMode);
    toolBar->addAction(actionColorPen);
    toolBar->addWidget(boxWidthPen);
    toolBar->addSeparator();
    toolBar->addAction(actionClear);
    toolBar->addAction(actionGoBack);
    toolBar->addWidget(spacer);
    toolBar->addAction(actionPrintFile);
    toolBar->addAction(actionNewFile);
    toolBar->addAction(actionSave);
    toolBar->addAction(actionOpen);
}


//Generate table
void MainWindow::createDrawSpace()
{
    // To put the view in the center of the main window
    setCentralWidget(view);
}

//Handling events
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Slot activated when right before closing the program
    // This function has been inspired by the scribble area example
    if(beforeClose())   // check if the user want to save
    {
        event->accept();
    }

    else
    {
        event->ignore();
    }
}

//Slots for actions
void MainWindow::changePenColor()
{
    // To change the pen color
    QColor color = QColorDialog::getColor(view->penColor());    // To show the palette of color
    view->setPenColor(QColor(color));
}

void MainWindow::changePenWidth()
{
    // To change the pen width
    view->setPenWidth(boxWidthPen->value());
}

void MainWindow::scribbleTool(bool checked)
{
    // To choose the scribble tool
    if(checked == true)
    {
        view->setCursor(QCursor(QPixmap(":/icones/penCur"),2,29));
        view->setToolType(View::typeScribble);
    }
}

void MainWindow::rectangleTool(bool checked)
{
    // To choose the rectangle tool
    if(checked == true)
    {
        view->setCursor(QCursor(Qt::CrossCursor));
        view->setToolType(View::typeRectangle);
    }
}

void MainWindow::ellipseTool(bool checked)
{
    // To choose the ellipse tool
    if(checked == true)
    {
        view->setCursor(QCursor(Qt::CrossCursor));
        view->setToolType(View::typeEllipse);
    }
}

void MainWindow::lineTool(bool checked)
{
    // To choose the line tool
    if(checked == true)
    {
        view->setCursor(QCursor(QPixmap(":/icones/penCur"),2,29));
        view->setToolType(View::typeLine);
    }
}

void MainWindow::fillingTool(bool checked)
{
    // To choose the filling tool
    if(checked == true)
    {
        view->setCursor(QCursor(QPixmap(":/icones/fillCur"),26,30));
        view->setToolType(View::typeFill);
    }

    else
    {
        view->setCursor(QCursor(QPixmap(":/icones/penCur"),2,29));
    }
}

void MainWindow::fillMode(bool checked)
{
    // To choose the fill mode
    view->setFillMode(checked);
    if(checked == true)
    {
        actionFillMode->setIcon(QIcon(":/icones/full"));
    }

    else
    {
        actionFillMode->setIcon(QIcon(":/icones/empty"));
    }
}

void MainWindow::about()
{
    // Slot to inform the user about this program
        QMessageBox::about(this, tr("About My Little Paint"),
            tr("<p><b>My Catsome Painter</b> has been designed by Anaïs SEYVE and Matthieu CLEMENT for the 20th October 2015."
               "This program allows you to paint on an image either free or with specifics shapes (rectangle, ellipse), "
               "to fill a zone, to register your work wherever you want in PNG, to print it and to open a PNG image...</p><p> "
               "For this purpose, different tools have been used, as the Qt example named Scribble Area to start, "
               "the wikipedia algorithm to fill a zone, the website OpenClassrooms and the Qt documentation.</p><p> "
               "The icons have been entirely drawn by ourselves. All rights are reserved.</p><p>"
               "We thank you for using our program, and hope it suits you.</p><p></p><p> "
               "<b>My Catsome Painter</b> by <i>Anaïs & Matthieu</i> </p>"));
}

void MainWindow::saveFile()
{
    // Slot to ask the control to save the project
    control->saveFile();
}

void MainWindow::openFile()
{
    // Slot to ask the control to open a project
    if(beforeClose())
    {
        control->openFile();
    }
}

void MainWindow::newFile()
{
    // Slot to ask the control to make a new project
    if(beforeClose())
    {
        control->newFile();
    }
}

void MainWindow::printFile()
{
    // Slot to print the project
    QPrinter printer(QPrinter::HighResolution);
    QImage image(model->elementContent());
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}

void MainWindow::goBack()
{
    // Slot to ask the control to load the previous image
    control->goBack();
}

bool MainWindow::beforeClose()
{
    // Slot to ask the user to save before closing the program
    if (view->isModified())
    {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, "Warning","The image has been modified.\n"
        "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            saveFile();
            return true;
        }

        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}
