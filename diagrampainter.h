#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>

#include <QRandomGenerator>

#include <QDebug>
#include <QtMath>

class diagramPainter : public QWidget
{
public:
    diagramPainter(const QColor& brushColor, const QColor& penColor, int penWidth,
                   const Qt::PenStyle& penStyle, QWidget *parent = nullptr);
    ~diagramPainter();

    QVector<double> getLeft() const;
    QVector<double> getRight() const;
    void setLeft(const QVector<double>& newLeft);
    void setRight(const QVector<double>& newRight);

    void generateRandomPercentageLeft();
    void generateRandomPercentageRight();

    void redraw();

    QColor brushColor;
    QColor penColor;
    int penWidth;
    Qt::PenStyle penStyle;

    const int numSegments = 2;
    const int numGroups = 5;


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> left;
    QVector<double> right;
};
