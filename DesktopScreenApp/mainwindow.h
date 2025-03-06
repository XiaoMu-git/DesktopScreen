#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_to_pairing_clicked();

    void on_btn_to_dashboard_clicked();

    void on_btn_to_config_clicked();

    void on_btn_to_logs_clicked();

    void on_btn_to_help_clicked();

    void on_btn_back_dashboard_1_clicked();

    void on_btn_back_dashboard_2_clicked();

    void on_btn_back_dashboard_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
