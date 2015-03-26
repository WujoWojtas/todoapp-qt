#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>;
#include <QJsonDocument>;
#include <QJsonArray>;
#include <QFileDialog>;
#include <QSettings>;
#include <QCloseEvent>;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loadItemsFromSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    this->saveItemsToSettings();
    event->accept();
}

void MainWindow::loadItemsFromSettings(){
QSettings settings(QApplication::applicationDirPath().left(1) + ":/ToDoApp");
    if(settings.contains("data")){
        QString itemsJson = settings.value("data").toString();
        this->loadJson(itemsJson);
    }
}

void MainWindow::saveItemsToSettings(){
    QSettings settings(QApplication::applicationDirPath().left(1) + ":/ToDoApp");
    QString jsonText = this->getJson();
    settings.setValue("data", jsonText);
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

QString MainWindow::getJson(){
    QJsonArray jsonArray;

    QList<QListWidgetItem*> items = ui->listWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    foreach (QListWidgetItem* item, items)
    {
        QJsonObject jsonItem;
        jsonItem["Value"] = item->text();
        jsonArray.append(jsonItem);
    }
    QJsonDocument doc(jsonArray);
    return doc.toJson(QJsonDocument::Indented);
}

void MainWindow::on_exportJson_clicked()
{
    QString resultJSON = this->getJson();

    QString filename = QFileDialog::getSaveFileName();
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    f.write(resultJSON.toUtf8().constData());
    f.close();
}

void MainWindow::loadJson(QString data){
    QJsonDocument JSONdocument = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsonArray = JSONdocument.array();

    foreach (QJsonValue jsonValue, jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        ui->listWidget->addItem(jsonObject["Value"].toString());
    }
}

void MainWindow::on_loadJson_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    QFile f( filename );
    f.open( QIODevice::ReadOnly );
    QString textJSON(f.readAll());
    f.close();
    this->loadJson(textJSON);
}

void MainWindow::on_clearAll_clicked()
{
    ui->listWidget->clear();
}
