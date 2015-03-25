#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    if(!text.isEmpty()){
        ui->listWidget->addItem(text);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
    for(int i=0; i<selectedItems.length(); i++){
        QListWidgetItem* item = selectedItems.at(i);
        delete item;
    }
}
