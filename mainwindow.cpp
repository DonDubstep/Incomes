#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidgetItem>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setStyleSheet(
        "QListWidget {"
        "background-color: #23262e;"
        "}"
        "QLabel {"
        "color: white;"
        "}");
    ui->color->setStyleSheet(
        "background-color: #224849;"
        "margin: 0;"
        "border: 0;"
        "padding: 0;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_time_button_clicked()
{
    time_t mytime = time(NULL);
    tm *now = localtime(&mytime);
    ui->time_line->setText(months[now->tm_mon]);
}

// Функция для вычисления светлоты
double luminance(int r, int g, int b) {
    double a = (r / 255.0) <= 0.03928 ? (r / 255.0) / 12.92 : pow((r / 255.0 + 0.055) / 1.055, 2.4);
    double d = (g / 255.0) <= 0.03928 ? (g / 255.0) / 12.92 : pow((g / 255.0 + 0.055) / 1.055, 2.4);
    double c = (b / 255.0) <= 0.03928 ? (b / 255.0) / 12.92 : pow((b / 255.0 + 0.055) / 1.055, 2.4);

    return 0.2126 * a + 0.7152 * d + 0.0722 * c;
}

// Функция для вычисления контрастности
double contrastRatio(double luminance1, double luminance2) {
    double lighter = std::max(luminance1, luminance2);
    double darker = std::min(luminance1, luminance2);
    return (lighter + 0.05) / (darker + 0.05);
}

QString calculate_TextColor(int r, int g, int b) {
    double backgroundLuminance = luminance(r, g, b);
    double whiteLuminance = luminance(255, 255, 255);
    double blackLuminance = luminance(0, 0, 0);

    double whiteContrast = contrastRatio(backgroundLuminance, whiteLuminance);
    double blackContrast = contrastRatio(backgroundLuminance, blackLuminance);

    return whiteContrast > blackContrast ? "rgb(255,255,255)" : "rgb(0,0,0)";
}

void MainWindow::on_add_button_clicked()
{
    QHBoxLayout* mainlayout = new QHBoxLayout();
    QLabel* cur_income = new QLabel(ui->cur_income->text());
    QLabel* cur_month = new QLabel(ui->time_line->text());
    mainlayout->addWidget(cur_income);
    mainlayout->addStretch();
    mainlayout->addWidget(cur_month);

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* widget = new QWidget();
    widget->setLayout(mainlayout);
    item->setSizeHint(widget->sizeHint());
    widget->setStyleSheet(
        "background-color: rgb(" + widget_r + "," + widget_g + "," + widget_b + ")" + ";"
        "color: " + calculate_TextColor(widget_r.toInt(), widget_g.toInt(), widget_b.toInt())
        );
    ui->listWidget->setItemWidget(item, widget);
}

void MainWindow::on_color_clicked()
{
    widget_r = QString::number(rand() % 256);
    widget_g = QString::number(rand() % 256);
    widget_b = QString::number(rand() % 256);
    widget_background_rgb = "rgb(" + widget_r + "," + widget_g + "," + widget_b + ")";
//    qDebug() << chooseTextColor(r.toInt(),g.toInt(),b.toInt());
    ui->color->setStyleSheet(
        "background-color: " + widget_background_rgb + ";"
        "margin: 0;"
        "border: 0;"
        "padding: 0;");
    // 16 777 215
}





