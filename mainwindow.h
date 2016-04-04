//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS MAINWINDOW || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || HEADER FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Libraries
#include <QMainWindow>
#include <QActionGroup>
#include <QMenuBar>
#include <QToolBar>
#include <QSpinBox>

#include "view.h"
#include "model.h"

// Class needed as attribute for this class
class Control;

// Definition of our class MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Functions to create interface
    void createActions();
    void createMenu();
    void createToolBar();
    void createDrawSpace();


    // Function to save before close a project
    bool beforeClose();

protected:
    // Function used with beforeClose()
    void closeEvent(QCloseEvent *event);

private slots:

    // Slots to choose tools
    void changePenColor();
    void changePenWidth();
    void scribbleTool(bool);
    void rectangleTool(bool);
    void ellipseTool(bool);
    void lineTool(bool);
    void fillingTool(bool);
    void fillMode(bool);

    // Slots to manipulate file
    void saveFile();
    void openFile();
    void newFile();
    void printFile();

    // Extra slots
    void goBack();
    void about();


private:
    // Structure MVC
    View *view;
    Model *model;
    Control *control;

    // Actions for the menus
    QActionGroup *actionGroupTools;
    QAction *actionColorPen;
    QAction *actionScribble;
    QAction *actionDrawRectangle;
    QAction *actionDrawEllipse;
    QAction *actionDrawLine;
    QAction *actionFilling;
    QAction *actionFillMode;
    QAction *actionSource;
    QAction *actionAboutQt;
    QAction *actionClear;
    QAction *actionGoBack;
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionNewFile;
    QAction *actionPrintFile;
    QAction *actionQuit;

    // Widget
    QSpinBox *boxWidthPen;
};

#endif // MAINWINDOW_H
