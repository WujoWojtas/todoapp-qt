#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QJsonObject";
#include "QJsonDocument";
#include "QJsonArray";
#include "QFileDialog";

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

void MainWindow::on_exportJson_clicked()
{
    QJsonArray jsonArray;

    QList<QListWidgetItem*> items = ui->listWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    foreach (QListWidgetItem* item, items)
    {
        QJsonObject jsonItem;
        jsonItem["Value"] = item->text();
        jsonArray.append(jsonItem);
    }
    QJsonDocument doc(jsonArray);
    QString resultJSON(doc.toJson(QJsonDocument::Indented));

    QString filename = QFileDialog::getSaveFileName();
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    f.write(resultJSON.toUtf8().constData());
    f.close();
}

void MainWindow::on_loadJson_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    QFile f( filename );
    f.open( QIODevice::ReadOnly );
    QString textJSON(f.readAll());
    f.close();

    QJsonDocument JSONdocument = QJsonDocument::fromJson(textJSON.toUtf8());
    QJsonArray jsonArray = JSONdocument.array();

    foreach (QJsonValue jsonValue, jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        ui->listWidget->addItem(jsonObject["Value"].toString());
    }

    //ui->listWidget->addItem(textJSON);
}
