
#include "diagrampainter.h"

diagramPainter::diagramPainter(const QColor& brushColor, const QColor& penColor, int penWidth,
                               const Qt::PenStyle& penStyle, QWidget *parent)
    : QWidget(parent), brushColor(brushColor), penColor(penColor), penWidth(penWidth), penStyle(penStyle)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    generateRandomPercentageLeft();
    generateRandomPercentageRight();
}

diagramPainter::~diagramPainter() {}

QVector<double> diagramPainter::getLeft() const
{
    return left;
}

QVector<double> diagramPainter::getRight() const
{
    return right;
}

void diagramPainter::setLeft(const QVector<double>& newLeft)
{
    left = newLeft;
    redraw();
}

void diagramPainter::setRight(const QVector<double>& newRight)
{
    right = newRight;
    redraw();
}

void diagramPainter::generateRandomPercentageLeft()
{
    left.clear();
    int remainingPercentage = 100;
    const int minSectorPercentage = 0;

    for (int i = 0; i < numGroups - 1; i++) {
        int maxRandomPercentage = remainingPercentage - (minSectorPercentage * (numGroups - i - 1));
        int randomPercentage = QRandomGenerator::global()->bounded(minSectorPercentage, maxRandomPercentage + 1);
        left.append(randomPercentage / 100.0);
        remainingPercentage -= randomPercentage;
    }

    left.append(remainingPercentage / 100.0);
    update();
}

void diagramPainter::generateRandomPercentageRight()
{
    right.clear();
    int remainingPercentage = 100;
    const int minSectorPercentage = 1;

    for (int i = 0; i < numGroups - 1; i++) {
        int maxRandomPercentage = remainingPercentage - (minSectorPercentage * (numGroups - i - 1));
        int randomPercentage = QRandomGenerator::global()->bounded(minSectorPercentage, maxRandomPercentage + 1);
        right.append(randomPercentage / 100.0);
        remainingPercentage -= randomPercentage;
    }

    right.append(remainingPercentage / 100.0);
    update();
}

void diagramPainter::redraw()
{
    update();
}

void diagramPainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(penStyle);
    painter.setPen(pen);

    // Ширина и высота каждого столбца
    int columnWidth = width() / ((numSegments + 1) * numGroups);
    int maxHeight = height() - 20;

    // Горизонтальное смещение для центрирования групп столбцов
    int xOffset = columnWidth / 2;

    // Выбор цвета левого столбика
    QColor leftColor = brushColor;

    // Правый цвет столбика (инверсия цвета левого столбика)
    QColor rightColor = leftColor.rgb() ^ 0x00ffffff;

    // Задаем шаг изменения цвета
    int colorStep = 75;

    for (int i = 0; i < numGroups; i++) {
        // Высоты текущих столбиков
        double leftHeight = left.value(i, 0.0);
        double rightHeight = right.value(i, 0.0);

        for (int j = 0; j < numSegments; j++) {
            // Высота текущего столбика
            double columnHeight;

            // Выбор высоты в зависимости от индекса столбика
            if (j == 0)
                columnHeight = leftHeight;
            else
                columnHeight = rightHeight;

            // Координаты левого верхнего угла столбика
            int x = xOffset + (i * (numSegments + 1) + j) * columnWidth;
            int y = height() - columnHeight * maxHeight - 10;

            // Устанавливаем кисть с текущим цветом столбика
            if (j == 0)
                painter.setBrush(leftColor);
            else
                painter.setBrush(rightColor);

            // Рисуем столбик
            painter.drawRect(x, y, columnWidth, columnHeight * maxHeight);

            // Изменяем цвет для следующего столбика
            if (j == 0)
                leftColor = leftColor.lighter(colorStep);
            else
                rightColor = rightColor.lighter(colorStep);
        }
    }
}
