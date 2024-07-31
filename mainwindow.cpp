#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidgetItem>
#include <QRandomGenerator>
#include <QMessageBox>
// ToDO:
// Кнопки + и -
// Рассчет остатка с предыдущего месяца
// Подгрузка всех средств

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
    json_data = JsonHandler::readJson();
    for(int i = 0; i < json_data.map.length(); i++)
    {
        QHBoxLayout* mainlayout = new QHBoxLayout();
        QLabel* cur_income = new QLabel(months[json_data.map[i]["month"]-1]);
        QLabel* cur_month = new QLabel(QString::number(json_data.map[i]["month_total"]));
        mainlayout->addWidget(cur_income);
        mainlayout->addStretch();
        mainlayout->addWidget(cur_month);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        QWidget* widget = new QWidget();
        widget->setLayout(mainlayout);
        item->setSizeHint(widget->sizeHint());
        widget->setStyleSheet(
            "background-color: rgb(" + QString::number(json_data.map[i]["r"]) + "," + QString::number(json_data.map[i]["g"]) + "," + QString::number(json_data.map[i]["b"]) + ")" + ";"
            "color: " + calculate_TextColor(json_data.map[i]["r"], json_data.map[i]["g"], json_data.map[i]["b"])
            );
        ui->listWidget->setItemWidget(item, widget);
    }
    ui->moneybox->setText(QString::number(json_data.moneybox));
    ui->total_last_month->setText(QString::number(json_data.total_last_month));
    ui->parents->setText(QString::number(json_data.parents));
    ui->total->setText(QString::number(json_data.total));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Нажатие на кнопку "Сейчас"
void MainWindow::on_time_button_clicked()
{
    time_t mytime = time(NULL);
    tm *now = localtime(&mytime);
    ui->time_line->setText(months[now->tm_mon]);
}

// Функция для вычисления светлоты
double MainWindow::luminance(int r, int g, int b) {
    double a = (r / 255.0) <= 0.03928 ? (r / 255.0) / 12.92 : pow((r / 255.0 + 0.055) / 1.055, 2.4);
    double d = (g / 255.0) <= 0.03928 ? (g / 255.0) / 12.92 : pow((g / 255.0 + 0.055) / 1.055, 2.4);
    double c = (b / 255.0) <= 0.03928 ? (b / 255.0) / 12.92 : pow((b / 255.0 + 0.055) / 1.055, 2.4);

    return 0.2126 * a + 0.7152 * d + 0.0722 * c;
}

// Функция для вычисления контрастности
double MainWindow::contrastRatio(double luminance1, double luminance2) {
    double lighter = std::max(luminance1, luminance2);
    double darker = std::min(luminance1, luminance2);
    return (lighter + 0.05) / (darker + 0.05);
}

// Функция для определения цвета текста
QString MainWindow::calculate_TextColor(int r, int g, int b) {
    double backgroundLuminance = luminance(r, g, b);
    double whiteLuminance = luminance(255, 255, 255);
    double blackLuminance = luminance(0, 0, 0);

    double whiteContrast = contrastRatio(backgroundLuminance, whiteLuminance);
    double blackContrast = contrastRatio(backgroundLuminance, blackLuminance);

    return whiteContrast > blackContrast ? "rgb(255,255,255)" : "rgb(0,0,0)";
}

// При нажатии кнопки добавления записи
void MainWindow::on_add_button_clicked()
{
    QHBoxLayout* mainlayout = new QHBoxLayout();
    QLabel* cur_income = new QLabel(ui->cur_income->text());
    QLabel* cur_month = new QLabel(ui->time_line->text());
    mainlayout->addWidget(cur_month);
    mainlayout->addStretch();
    mainlayout->addWidget(cur_income);

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* widget = new QWidget();
    widget->setLayout(mainlayout);
    item->setSizeHint(widget->sizeHint());
    widget->setStyleSheet(
        "background-color: rgb(" + widget_r + "," + widget_g + "," + widget_b + ")" + ";"
        "color: " + calculate_TextColor(widget_r.toInt(), widget_g.toInt(), widget_b.toInt())
        );
    ui->listWidget->setItemWidget(item, widget);
    int cur_month_int = 0;
    for(int i = 0; i < months.length(); i++)
    {
        if(months[i] == ui->time_line->text())
        {
            cur_month_int = i+1;
            break;
        }
    }
    QMap<QString, int> new_note;
    new_note["month"] = cur_month_int;
    new_note["month_total"] = ui->cur_income->text().toInt();
    new_note["r"] = widget_r.toInt();
    new_note["g"] = widget_g.toInt();
    new_note["b"] = widget_b.toInt();
    json_data.map.append(new_note);
}

// При нажатии кнопки цвета
void MainWindow::on_color_clicked()
{
    widget_r = QString::number(rand() % 256);
    widget_g = QString::number(rand() % 256);
    widget_b = QString::number(rand() % 256);
    widget_background_rgb = "rgb(" + widget_r + "," + widget_g + "," + widget_b + ")";
    ui->color->setStyleSheet(
        "background-color: " + widget_background_rgb + ";"
        "margin: 0;"
        "border: 0;"
        "padding: 0;");
}

void MainWindow::on_write_to_file_clicked()
{
    json_data.moneybox = ui->moneybox->text().toInt();
    json_data.parents = ui->parents->text().toInt();
    json_data.total = ui->total->text().toInt();
    json_data.total_last_month = ui->total_last_month->text().toInt();
    JsonHandler::write_json(json_data);
}


void MainWindow::on_next_month_clicked()
{
    // переключение на следующий месяц
}


void MainWindow::on_prev_month_clicked()
{
    // переключение на предыдущий месяц
}

