
#include "mainwindow.h"
#include "ui_mainwindow.h"

//Конструктор, в который передаются все параметры для рисования по-уолчанию
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , brushColor(QColor::fromRgb(85, 170, 0))
    , penColor(QColor(Qt::black))
    , penWidth(1)
    , penStyle(Qt::SolidLine)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionOpen->setDisabled(true);
    ui->actionClose->setDisabled(true);

    labelWidth = new QLabel("Pen Width : 1px   ");
    labelStyle = new QLabel("Pen Style: Solid Line   ");
    labelName = new QLabel("File Name: default.json   ");

    ui->statusbar->addWidget(labelWidth);
    ui->statusbar->addWidget(labelStyle);
    ui->statusbar->addWidget(labelName);

    QGridLayout *grid = new QGridLayout();
    tabWidget = new QTabWidget();
    diagram = new diagramPainter(brushColor, penColor, penWidth, penStyle);
    tabWidget->setTabBarAutoHide(true);
    tabWidget->addTab(diagram, "");
    grid->addWidget(tabWidget);
    setCentralWidget(tabWidget);

    //Сигналы и слоты для переприсвоения значений для paintEvent
    connect(this, SIGNAL(brushColorChanged(QColor)), this, SLOT(updateBrushColor(QColor)));
    connect(this, SIGNAL(penColorChanged(QColor)), this, SLOT(updatePenColor(QColor)));
    connect(this, SIGNAL(penWidthChanged(int)), this, SLOT(updatePenWidth(int)));
    connect(this, SIGNAL(penStyleChanged(Qt::PenStyle)), this, SLOT(updatePenStyle(Qt::PenStyle)));

    menuStyle();

    isDrawing = true;

    qDebug() << diagram->getLeft();
    qDebug() << diagram->getRight();

}


MainWindow::~MainWindow()
{
    delete ui;
}


//Меню
void MainWindow::menuStyle()
{
    tabWidget->setStyleSheet("background-color: #cccccc;"
                             "border: 1px solid #cccccc;"
                             "border-radius:10px");
    ui->menubar->setStyleSheet("background-color: #cccccc;"
                               "border: 1px solid #cccccc;"
                               "border-radius:10px");
    ui->statusbar->setStyleSheet("background-color: #cccccc;"
                                 "border: 1px solid #cccccc;"
                                 "border-radius:10px");
}


//Ивент нажатия ПКМ только в области tabWidget
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && tabWidget->rect().contains(event->pos())) {
            QMenu menu(this);
            QAction* changePenWidth = new QAction("Change Pen Width", this);
            QAction* changePenStyle = new QAction("Change Pen Style", this);
            menu.addAction(changePenWidth);
            menu.addAction(changePenStyle);
            connect(changePenWidth, &QAction::triggered, this, &MainWindow::showChangePenWidthDialog);
            connect(changePenStyle, &QAction::triggered, this, &MainWindow::showChangePenStyleDialog);
            menu.exec(event->globalPos());
        }
    }

//Диалог для смены толщины пера
void MainWindow::showChangePenWidthDialog()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Change Pen Width");
    dialog->setFixedSize(200, 120);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QButtonGroup *buttonGroup = new QButtonGroup(dialog);

    radioButton1 = new QRadioButton("1px width", dialog);
    radioButton3 = new QRadioButton("3px width", dialog);
    radioButton5 = new QRadioButton("5px width", dialog);

    switch (penWidth) {
    case 1:
        radioButton1->setChecked(true);
        break;
    case 3:
        radioButton3->setChecked(true);
        break;
    case 5:
        radioButton5->setChecked(true);
        break;
    }

    buttonGroup->addButton(radioButton1);
    buttonGroup->addButton(radioButton3);
    buttonGroup->addButton(radioButton5);

    layout->addWidget(radioButton1);
    layout->addWidget(radioButton3);
    layout->addWidget(radioButton5);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
        setPenWidth();
        dialog->close();
    });

    dialog->exec();
}


//Диалог для смены стиля пера
void MainWindow::showChangePenStyleDialog()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Change Pen Style");
    dialog->setFixedSize(200, 120);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QButtonGroup *buttonGroup = new QButtonGroup(dialog);

    radioButtonSolidLine = new QRadioButton("Solid Line", dialog);
    radioButtonDashLine = new QRadioButton("Dash Line", dialog);
    radioButtonDotLine = new QRadioButton("Dot Line", dialog);

    if(penStyle == Qt::SolidLine){
        radioButtonSolidLine->setChecked(true);
    }
    if(penStyle == Qt::DashLine){
        radioButtonDashLine->setChecked(true);
    }
    if(penStyle == Qt::DotLine){
        radioButtonDotLine->setChecked(true);
    }

    buttonGroup->addButton(radioButtonSolidLine);
    buttonGroup->addButton(radioButtonDashLine);
    buttonGroup->addButton(radioButtonDotLine);

    layout->addWidget(radioButtonSolidLine);
    layout->addWidget(radioButtonDashLine);
    layout->addWidget(radioButtonDotLine);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
        setPenStyle();
        dialog->close();
    });

    dialog->exec();
}


//Установка толщины пера
void MainWindow::setPenWidth()
{
    if(radioButton1->isChecked()){
        diagram->penWidth = 1;
        labelWidth->setText("Pen Width: 1px   ");
        emit penWidthChanged(diagram->penWidth);
    }
    if(radioButton3->isChecked()){
        diagram->penWidth = 3;
        labelWidth->setText("Pen Width: 3px   ");
        emit penWidthChanged(diagram->penWidth);
    }
    if(radioButton5->isChecked()){
        diagram->penWidth = 5;
        labelWidth->setText("Pen Width: 5px   ");
        emit penWidthChanged(diagram->penWidth);
    }
}


//Установка стиля пера
void MainWindow::setPenStyle()
{
    if(radioButtonSolidLine->isChecked()){
        diagram->penStyle = Qt::SolidLine;
        labelStyle->setText("Pen Style: Solid Line   ");
        emit penStyleChanged(diagram->penStyle);
    }
    if(radioButtonDashLine->isChecked()){
        diagram->penStyle = Qt::DashLine;
        labelStyle->setText("Pen Style: Dash Line   ");
        emit penStyleChanged(diagram->penStyle);
    }
    if(radioButtonDotLine->isChecked()){
        diagram->penStyle = Qt::DotLine;
        labelStyle->setText("Pen Style: Dot Line   ");
        emit penStyleChanged(diagram->penStyle);
    }
}


//Кнопка About
void MainWindow::on_actionAbout_triggered()
{
    QString text = "Малиновский Егор \n1 курс, 12 группа\nВариант 1";
    QMessageBox::about(this, "About", text);
}


//Выбор цвета кисти
void MainWindow::on_actionBrush_Color_triggered()
{
    resColor = diagram->brushColor;
    diagram->brushColor = QColorDialog::getColor(diagram->brushColor, this, tr("Select Brush Color"));
    if (diagram->brushColor.isValid()) {
        emit brushColorChanged(diagram->brushColor);
    }
    else {
        diagram->brushColor = resColor;
        emit brushColorChanged(diagram->brushColor);
    }
}


//Выбор цвета пера
void MainWindow::on_actionPen_Color_triggered()
{
    resColor = diagram->penColor;
    diagram->penColor = QColorDialog::getColor(diagram->penColor, this, tr("Select Pen Color"));
    if (diagram->penColor.isValid()) {
        emit penColorChanged(diagram->penColor);
    }
    else {
        diagram->penColor = resColor;
        emit penColorChanged(diagram->penColor);
    }
}


//Очистка поля
void MainWindow::on_actionClose_triggered()
{
    tabWidget->clear();
    isDrawing = false;
    ui->actionSave->setDisabled(true);
    ui->actionSave_As->setDisabled(true);
}


//Просмотр сочетаний клавиш для программы
void MainWindow::on_actionTutorial_triggered()
{
    QString tutorialText = "            Key Shortcuts and Functions            \n"
                           "CTRL + O - Open File\n"
                           "CTRL + ALT + S - Save to already opened file\n"
                           "CTRL + S - Save File\n"
                           "ALT + X - Exit Program\n"
                           "ALT + F1 - About\n"
                           "ALT + C - Brush Color\n"
                           "ALT + P - Pen Color\n"
                           "RMB - Open context menu with Pen Width and Style";

    QMessageBox::information(this, "Tutorial", tutorialText);
}


//Закрытие программы с окном уточнения сохранения
void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Do you want to save the file?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        on_actionSave_As_triggered();
        MainWindow::close();
    } else if (reply == QMessageBox::No) {
        MainWindow::close();
    }
}


//Сохраняем файл в формате .json
void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QJsonObject jsonObject;
    jsonObject["brushColor"] = brushColor.name();
    jsonObject["penColor"] = penColor.name();
    jsonObject["penWidth"] = penWidth;
    jsonObject["penStyle"] = penStyle;

    // Получение значений векторов left и right из diagramPainter
    QVector<double> left = diagram->getLeft();
    QVector<double> right = diagram->getRight();

    QJsonArray leftArray;
    for (int i = 0; i < left.size(); i++) {
        leftArray.append(left[i]);
    }
    jsonObject["left"] = leftArray;

    QJsonArray rightArray;
    for (int i = 0; i < right.size(); i++) {
        rightArray.append(right[i]);
    }
    jsonObject["right"] = rightArray;

    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonData);
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open file for writing.");
    }

    ui->actionClose->setEnabled(true);
    ui->actionOpen->setEnabled(true);
    // ui->actionSave->setEnabled(true);
}


//Сохраняем файл путем перезаписи содержимого уже открытого файла
void MainWindow::on_actionSave_triggered()
{
    QString fileName = currentFileName;
    if (fileName.isEmpty()) {
        // Если имя файла не задано, вызываем диалог "Сохранить как"
        on_actionSave_As_triggered();
        return;
    }
    QJsonObject jsonObject;
    jsonObject["brushColor"] = brushColor.name();
    jsonObject["penColor"] = penColor.name();
    jsonObject["penWidth"] = penWidth;
    jsonObject["penStyle"] = penStyle;

    // Получение значений векторов left и right из diagramPainter
    QVector<double> left = diagram->getLeft();
    QVector<double> right = diagram->getRight();

    QJsonArray leftArray;
    for (int i = 0; i < left.size(); i++) {
        leftArray.append(left[i]);
    }
    jsonObject["left"] = leftArray;

    QJsonArray rightArray;
    for (int i = 0; i < right.size(); i++) {
        rightArray.append(right[i]);
    }
    jsonObject["right"] = rightArray;

    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonData);
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open file for writing.");
    }
}

void MainWindow::updateBrushColor(const QColor &color)
{
    brushColor = color;
}

void MainWindow::updatePenColor(const QColor &color)
{
    penColor = color;
}

void MainWindow::updatePenWidth(int width)
{
    penWidth = width;
}

void MainWindow::updatePenStyle(const Qt::PenStyle &style)
{
    penStyle = style;
}


//Открываем файл и рисуем по данным из него диаграму
void MainWindow::on_actionOpen_triggered()
{
    if (isDrawing) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save", "Do you want to save the current drawing?",
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) {
            return; // Отмена открытия нового файла
        } else if (reply == QMessageBox::Yes) {
            // Сохранить текущий рисунок
            on_actionSave_triggered();
        }
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open", "", "JSON Files (*.json)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData);
        if (!jsonDocument.isNull() && jsonDocument.isObject()) {
            QJsonObject jsonObject = jsonDocument.object();

            if (jsonObject.contains("brushColor") && jsonObject["brushColor"].isString()) {
                brushColor = QColor(jsonObject["brushColor"].toString());
                emit brushColorChanged(brushColor);
            }

            if (jsonObject.contains("penColor") && jsonObject["penColor"].isString()) {
                penColor = QColor(jsonObject["penColor"].toString());
                emit penColorChanged(penColor);
            }

            if (jsonObject.contains("penWidth") && jsonObject["penWidth"].isDouble()) {
                penWidth = jsonObject["penWidth"].toInt();
                labelWidth->setText("Pen Width: " + QString::number(penWidth) + "px   ");
                emit penWidthChanged(penWidth);
            }

            if (jsonObject.contains("penStyle") && jsonObject["penStyle"].isDouble()) {
                int penStyleCode = jsonObject["penStyle"].toInt();
                switch (penStyleCode) {
                case 1:
                    penStyle = Qt::SolidLine;
                    break;
                case 2:
                    penStyle = Qt::DashLine;
                    break;
                case 3:
                    penStyle = Qt::DotLine;
                    break;
                default:
                    penStyle = Qt::DashLine;
                    break;
                }
                QString penStyleStr = penStyleCode == 1 ? "SolidLine" :
                                          penStyleCode == 2 ? "DashLine" :
                                          penStyleCode == 3 ? "DotLine" : "SolidLine";
                labelStyle->setText("Pen Style: " + penStyleStr + "   ");
                emit penStyleChanged(penStyle);
            }

            if (jsonObject.contains("left") && jsonObject["left"].isArray()) {
                QJsonArray leftArray = jsonObject["left"].toArray();
                QVector<double> left;

                for (const QJsonValue& value : leftArray) {
                    if (value.isDouble()) {
                        left.append(value.toDouble());
                    }
                }

                if (diagram) {
                    delete diagram;
                }
                diagram = new diagramPainter(brushColor, penColor, penWidth, penStyle, this);
                diagram->setLeft(left);
                tabWidget->addTab(diagram, "");
                isDrawing = true;
            }

            if (jsonObject.contains("right") && jsonObject["right"].isArray()) {
                QJsonArray rightArray = jsonObject["right"].toArray();
                QVector<double> right;

                for (const QJsonValue& value : rightArray) {
                    if (value.isDouble()) {
                        right.append(value.toDouble());
                    }
                }

                if (diagram) {
                    diagram->setRight(right);
                }
            }

            if(!isDrawing)
            {
                diagram = new diagramPainter(brushColor, penColor, penWidth, penStyle);
                tabWidget->addTab(diagram, "");
                isDrawing = true;
            }

            //Изменение метки labelName
            QFileInfo fileInfo(file.fileName());
            QString fileName = fileInfo.fileName();
            labelName->setText("File Name: " + fileName + "   ");

        } else {
            QMessageBox::critical(this, "Error", "Invalid JSON file format.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to open file.");
    }
    currentFileName = fileName;

    // Отладочный вывод для проверки значений left и right
    qDebug() << "Left:" << diagram->getLeft();
    qDebug() << "Right:" << diagram->getRight();

    ui->actionSave->setDisabled(false);
    ui->actionSave_As->setDisabled(false);
}

