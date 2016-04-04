//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS CONTROL || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || IMPLEMENTATION FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Libraries
#include "control.h"
#include "model.h"
#include "view.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QPainter>


Control::Control(Model *model,View &view) :
    view(view)
{
    // The constructor set the pointers to our model and view
    this->model = model;
    view.setControl(this);  // Calls the view function to set the pointer to this control into the view
}

void Control::registerImage(const QImage &image) const
{
    if(!image.isNull()) // Checks if the image does exist
    {
        model->registerImage(image);    // To register the image visible on the view into the model
    }
}

const QImage Control::uploadBuffer() const
{
    return QImage(model->elementContent()); // To take back the image from the model's buffer
}


void Control::saveFile()    // To save our file
{
    QImage myImage(model->elementContent());    // Get the image in the model
    QString filter = "PNG Files (*.png)";
    QString folder = QFileDialog::getSaveFileName(&view, "Save Worksheet", QDir::homePath(), filter, &filter);  // Choose the location to save the file
    myImage.save(folder, "PNG");    // Register the image in PNG at the desired location
    if(!folder.isEmpty())
    {
        QMessageBox::information(&view, "Save succeeded", "The file has been successfully saved in the folder : "
                             + folder, QMessageBox::Close);
    }
    else
    {
        QMessageBox::information(&view, "Save failed", "The file has not been saved because the folder you entered is not correct.");
    }
}

void Control::openFile()
{
    model->clearAll();  // Reset all attributes in model

    QString filter = "PNG Files (*.png)";
    QString folder = QFileDialog::getOpenFileName(&view, "Open File", QDir::homePath(), filter, &filter);   // Choose the location of the file to open
    QFile file(folder); // Register the content of the file to open in a QFile
    QImage myImage;

    if(file.open(QIODevice::ReadOnly))
    {
        myImage.load(&file,"PNG");  // Load the content of the previous QFile in a QImage from PNG format

        if (myImage.size() != view.size())  // if the image loaded is different than the view size
        {
            // The background is painted in white and then we add the image
            QImage newImage(view.size(), QImage::Format_RGB32);
            newImage.fill(qRgb(255, 255, 255));
            QPainter painter(&newImage);
            painter.drawImage(QPoint(0, 0), myImage);
            myImage = newImage;
        }
        model->registerImage(myImage);  // We update the model with the new image
        view.uploadBuffer();            // Then we upload the view to show the new image
        file.close();
    }
}

void Control::newFile()
{
    // To create a new file, we reset all the attributes in model and view
    if(model->clearAll())
    {
        view.setDefaultAttributes();
        view.uploadBuffer();
    }
}

void Control::goBack()
{
    // To go back, we manipulate the historic in the model to change the buffer
    if(model->goBack())
    {
        view.uploadBuffer();    // Then we upload the view to show the new image
    }
}


void Control::clearImage()
{
    view.clearImage();  // The function to clear the image is all done in the class View
}
