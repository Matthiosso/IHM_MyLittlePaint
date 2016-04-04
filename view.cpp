//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS VIEW || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || IMPLEMENTATION FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Libraries
#include "view.h"
#include "control.h"

#include <QStack>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QtMath>

View::View(QWidget *parent)
    : QWidget(parent)
{
    setDefaultAttributes();
}

View::~View()
{

}


// Setters

void View::setDefaultAttributes()
{
    // To reset all attributes
    clearImage();
    modified = false;
    drawing = false;
    fillMode = false;
    myPenWidth = 1;
    myPenColor = Qt::black;
    myToolType = typeScribble;
    setCursor(QCursor(QPixmap(":/icones/penCur"),2,29));
}

void View::setFillMode(bool yes)
{
    if(yes == true)
    {
        fillMode = true;
    }

    else
    {
        fillMode = false;
    }
}

void View::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void View::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void View::setToolType(View::TypeTool newType)
{
    myToolType = newType;
}


// Event handling functions
void View::mousePressEvent(QMouseEvent *event)
{
    // Slot for the mouse click event
    // This function has been inspired by the scribble area example
    if (event->button() == Qt::LeftButton)
    {
        if(myToolType == typeScribble)
        {
            lastPoint = event->pos();
            drawing = true;
            drawPoint();
        }

        else if(myToolType == typeFill)
        {
            firstPoint = event->pos();
            fillExtend(firstPoint,QColor(image.pixel(firstPoint)));
            update();
            modified = true;
            registerImage();
        }

        else
        {
            firstPoint = event->pos();
            lastPoint = event->pos();
            drawing = true;
        }
    }
}


void View::mouseMoveEvent(QMouseEvent *event)
{
    // Slot for the mouse clicked move event
    // This function has been inspired by the scribble area example
    if ((event->buttons() & Qt::LeftButton) && drawing)
    {
        if(myToolType == typeScribble)
        {
           drawLineTo(event->pos());
        }

        else if(myToolType != typeFill)
        {
            lastPoint = event->pos();
            drawFigure();
        }
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    // Slot for the mouse click release event
    // This function has been inspired by the scribble area example
    if (event->button() == Qt::LeftButton && drawing)
    {
        if(myToolType == typeScribble)
        {
            drawLineTo(event->pos());
            drawing = false;
            registerImage();
        }

        else if(myToolType == typeFill)
        {
            drawing = false;
        }

        else if(firstPoint != lastPoint)
        {
            drawing = false;
            drawFigure();
        }
    }

}

void View::paintEvent(QPaintEvent *event)
{
    // Slot to paint on the widget from the image
    // This function has been inspired by the scribble area example
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void View::resizeEvent(QResizeEvent *event)
{
    // Slot to adapt the image to the change of the window size
    // This function has been inspired by the scribble area example
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width(), image.width());
        int newHeight = qMax(height(), image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


// Drawing functions
void View::drawLineTo(const QPoint &endPoint)
{
    // Function to scribble
    // This function has been inspired by the scribble area example
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void View::drawPoint()
{
    // Function to draw a point when the user just click with the scribble tool without moving the mouse
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(lastPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint,QSize(rad,rad)).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
}

void View::drawFigure()
{
    uploadBuffer();
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(fillMode == true)
    {
        painter.setBrush(myPenColor);   // Fill the figure (ellipse/rectangle)
    }

    if(myToolType == typeEllipse)   // Draw an ellipse/circle
    {
        if(QApplication::queryKeyboardModifiers() == Qt::ALT)   // Press ALT to draw a circle
        {
            painter.drawEllipse(firstPoint,abs(QLineF(firstPoint,lastPoint).length()),abs(QLineF(firstPoint,lastPoint).length()));
        }

        else    // if you are not pressing ALT, you will draw an ellipse
        {
            painter.drawEllipse(QRectF(firstPoint,lastPoint));
        }
    }

    else if(myToolType == typeRectangle)    // Draw a rectangle/square
    {
        if(QApplication::queryKeyboardModifiers() == Qt::ALT)   // Press ALT to draw a square
        {
            int X = lastPoint.rx() - firstPoint.rx();
            int Y = lastPoint.ry() - firstPoint.ry();
            if((X > 0) && (Y >= 0))     // The square down on the right
            {
                if(X > Y)
                {
                    painter.drawRect(QRect(firstPoint,QSize(X,X)));
                }

                else
                {
                    painter.drawRect(QRect(firstPoint,QSize(Y,Y)));
                }
            }

            else if((X <= 0) && (Y < 0))        // The square up on the left
            {
                if(X > Y)
                {
                    painter.drawRect(QRect(firstPoint,QPoint(firstPoint.rx() + Y, lastPoint.ry())));
                }

                else
                {
                    painter.drawRect(QRect(firstPoint,QPoint(lastPoint.rx(), firstPoint.ry() + X)));
                }
            }

            else if((X >= 0) && (Y < 0))        // The square up on the right
            {
                if(X > -Y)
                {

                    painter.drawRect(QRect(firstPoint,QPoint(lastPoint.rx(), firstPoint.ry() - X)));
                }

                else
                {
                    painter.drawRect(QRect(firstPoint,QPoint(firstPoint.rx() - Y, lastPoint.ry())));
                }
            }

            else        // The square down on the left
            {
                if(-X > Y)
                {
                    painter.drawRect(QRect(firstPoint,QPoint(lastPoint.rx(), firstPoint.ry() - X)));
                }

                else
                {
                    painter.drawRect(QRect(firstPoint,QPoint(firstPoint.rx() - Y, lastPoint.ry())));
                }
            }
        }

        else    // if you are not pressing ALT, you will draw a rectangle
        {
            painter.drawRect(QRect(firstPoint,lastPoint));
        }
    }

    else if(myToolType == typeLine)     // Draw a straight line
    {
        if(QApplication::queryKeyboardModifiers() == Qt::ALT)   // Press ALT to draw a vertical/horizontal line
        {
            if(abs(firstPoint.rx()-lastPoint.rx()) > abs(firstPoint.ry()-lastPoint.ry()))   // horizontal
            {
                painter.drawLine(firstPoint,QPoint(QLineF(firstPoint,lastPoint).dx()+firstPoint.rx(),firstPoint.ry()));
            }

            else    // vertical
            {
                painter.drawLine(firstPoint,QPoint(firstPoint.rx(),QLineF(firstPoint,lastPoint).dy()+firstPoint.ry()));
            }
        }

        else
        {
            painter.drawLine(firstPoint,lastPoint);
        }
    }

    modified = true;
    update();
    if(drawing == false)
    {
        registerImage();    // Register the image if the mouse has finished to draw
    }

}


void View::fillExtend(QPoint point, QColor firstColor)
{
    // Algorithm to fill a figure (seed fill)
    // This algorithm has been inspired by a known one (see flood fill on Wikipedia)
    if(image.pixel(point) != firstColor.rgba())
    {
        return;
    }

    else if(image.pixel(point) == myPenColor.rgba())
    {
        return;
    }

    else
    {
        QStack<QPoint> myQueue;
        QPoint temp;

        myQueue.push(point);
        while(!myQueue.isEmpty())
        {
            temp = myQueue.pop();
            image.setPixel(temp,myPenColor.rgba());

            if((temp.ry()+1 < height()) && (image.pixel(temp.rx(), temp.ry()+1) == firstColor.rgba()))
            {
                myQueue.push(QPoint(temp.rx(), temp.ry()+1));
            }
            if((temp.ry()-1 >= 0) && (image.pixel(temp.rx(), temp.ry()-1) == firstColor.rgba()))
            {
                myQueue.push(QPoint(temp.rx(), temp.ry()-1));
            }
            if((temp.rx()+1 < width()) && (image.pixel(temp.rx()+1, temp.ry()) == firstColor.rgba()))
            {
                myQueue.push(QPoint(temp.rx()+1, temp.ry()));
            }
            if((temp.rx()-1 >= 0) && (image.pixel(temp.rx()-1, temp.ry()) == firstColor.rgba()))
            {
                myQueue.push(QPoint(temp.rx()-1, temp.ry()));
            }
        }
    }
}

// Extra functions
void View::clearImage()
{
    // Function to clear the image
    image.fill(qRgb(255, 255, 255));
    modified = true;
    registerImage();
    update();
}

void View::resizeImage(QImage *image, const QSize &newSize)
{
    // Function to resize the image according to the window size
    // This function has been inspired by the scribble area example
    if (image->size() == newSize)
    {
        return;
    }

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void View::registerImage()
{
    // Function to send the image to the model by the control for registering it
    control->registerImage(image);
}

void View::uploadBuffer()
{
    //Replace the actual image by the buffer image into the actual imag
    image = (control->uploadBuffer());
    update();
}

