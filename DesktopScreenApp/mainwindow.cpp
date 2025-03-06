#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_to_pairing_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_pairing);
}


void MainWindow::on_btn_to_dashboard_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_dashboard);
}


void MainWindow::on_btn_to_config_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_config);
}


void MainWindow::on_btn_to_logs_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_logs);
}


void MainWindow::on_btn_to_help_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_help);
}


void MainWindow::on_btn_back_dashboard_1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_dashboard);
}


void MainWindow::on_btn_back_dashboard_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_dashboard);
}


void MainWindow::on_btn_back_dashboard_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_dashboard);
}

