#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class DrawWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWindow(QWidget *parent = 0);

    void setPenColor(const QColor &newColor);
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    const QImage &getImage() const{ return image; }

signals:
    void newImage();

public slots:
    void clearImage();
    void setPenWidth(int newWidth);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void drawLineTo(const QPoint &endPoint);
    void drawPoint();
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool drawing;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
};

#endif // DRAWWINDOW_H
