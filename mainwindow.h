
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QColorDialog>
#include <QRadioButton>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>
#include <QFileInfo>

#include <QPainter>
#include <QGridLayout>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <diagrampainter.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QColor brushColor;
    QColor penColor;
    QColor resColor;
    int penWidth;
    Qt::PenStyle penStyle;

    QRadioButton *radioButton1;
    QRadioButton *radioButton3;
    QRadioButton *radioButton5;

    QRadioButton *radioButtonSolidLine;
    QRadioButton *radioButtonDashLine;
    QRadioButton *radioButtonDotLine;

    QString currentFileName;

signals:
    void brushColorChanged(const QColor& color);
    void penColorChanged(const QColor& color);
    void penWidthChanged(int width);
    void penStyleChanged(const Qt::PenStyle& style);

private slots:
    void on_actionAbout_triggered();
    void on_actionBrush_Color_triggered();
    void on_actionPen_Color_triggered();
    void setPenWidth();
    void setPenStyle();
    void on_actionClose_triggered();
    void on_actionTutorial_triggered();
    void on_actionExit_triggered();
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void updateBrushColor(const QColor& color);
    void updatePenColor(const QColor& color);
    void updatePenWidth(int width);
    void updatePenStyle(const Qt::PenStyle& style);

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    diagramPainter *diagram;
    QLabel *labelWidth;
    QLabel *labelStyle;
    QLabel *labelName;

    void mousePressEvent(QMouseEvent *event);

    void menuStyle();
    void showChangePenWidthDialog();
    void showChangePenStyleDialog();

    bool isDrawing;


};

#endif // MAINWINDOW_H
