#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jsonhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    json_return json_data;


private slots:

    void on_time_button_clicked();

    void on_add_button_clicked();

    void on_color_clicked();

    double luminance(int r, int g, int b);
    double contrastRatio(double luminance1, double luminance2);
    QString calculate_TextColor(int r, int g, int b);

    void on_write_to_file_clicked();

    void on_next_month_clicked();

    void on_prev_month_clicked();

private:
    Ui::MainWindow *ui;
    QString widget_background_rgb;
    QString widget_r;
    QString widget_g;
    QString widget_b;
    QStringList months = {"Январь", "Февраль",
                            "Март", "Апрель", "Май",
                            "Июнь", "Июль", "Август",
                            "Сентябрь", "Октябрь", "Ноябрь",
                            "Декабрь"};
};
#endif // MAINWINDOW_H
