//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TP MINIPROJ INGE 2 FOR IHM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ (My Little Paint Project) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ due on October, 20, 2015 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ by Anaïs SEYVE & Matthieu CLEMENT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class 2Y ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || CLASS VIEW || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ || HEADER FILE || ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#ifndef VIEW_H
#define VIEW_H

// Libraries
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

// Class needed as attribute for this class
class Control;

// Definition of our class View
class View : public QWidget
{
    Q_OBJECT
    Q_ENUMS(TypeTool)

public:
    explicit View(QWidget *parent = 0);
    ~View();

    // Create an enum to clarify the use of different tools in the painting program
    enum TypeTool
    {
        typeScribble,
        typeRectangle,
        typeEllipse,
        typeLine,
        typeFill
    };

    // Setters and Getters for attribute
    void setDefaultAttributes();
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setToolType(View::TypeTool);
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    const QImage &getImage() const{ return image; }

    // Functions using MVC structure
    void registerImage();
    void uploadBuffer();


    // To set the control attribute
    void setControl(Control *control) {
        this->control = control;
    }


public slots:
    // Slots
    void clearImage();
    void setFillMode(bool);


protected:
    // Handling the different event
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Control *control;   // Use an attribute control to be able to access the control

    // Functions to manipulate the view (used only in the view => private)
    void drawFigure();
    void drawLineTo(const QPoint &endPoint);
    void drawPoint();
    void fillExtend(QPoint, QColor);
    void resizeImage(QImage *image, const QSize &newSize);


    // Attributes
    bool fillMode;
    bool modified;
    bool drawing;
    int myPenWidth;
    QColor myPenColor;
    TypeTool myToolType;
    QImage image;
    QPoint lastPoint;
    QPoint firstPoint;
    QColor firstColor;

};

#endif // VIEW_H
