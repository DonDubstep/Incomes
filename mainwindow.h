#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_time_button_clicked();

    void on_add_button_clicked();

    void on_color_clicked();

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
