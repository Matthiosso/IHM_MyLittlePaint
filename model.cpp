//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS MODEL || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || IMPLEMENTATION FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Libraries
#include "model.h"

Model::Model()
{
    clearAll();
}


bool Model::clearAll()
{
    // Reset all attributes of the model
    QSize size(1366,651);
    QImage newImage(size, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    buffer = newImage;
    historic.clear();
    historic.push_back(buffer);
    return true;
}


const QImage &Model::elementContent() const
{
    // To get the actual image registered in the model into the buffer
    return buffer;
}


//
bool Model::registerImage(const QImage &image)
{
    // Update the buffer
    buffer = QImage(image);

    // Register the buffer in a historic:
    historic.push_back(image);
    if(historic.size() > 20)    // The historic can contain no more than 20 images maximum
    {
        historic.pop_front();   // if the historic is full, we erase the oldest image registered
    }
    return true;
}


bool Model::goBack()
{
    // For accessing the historic
    if(historic.size() - 1 > 0)
    {
        buffer = historic[historic.size()-2];
        historic.pop_back();
    }
    return true;
}
