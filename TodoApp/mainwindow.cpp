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
    initialize();
}

void MainWindow::initialize()
{
    this->settingsPath = QApplication::applicationDirPath().left(1) + ":/ToDoApp";
    this->settingsKey = "data";

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
    QSettings settings(this->settingsPath);
    if(settings.contains(this->settingsKey)){
        QString itemsJson = settings.value(this->settingsKey).toString();
        this->loadJson(itemsJson);
    }
}

void MainWindow::saveItemsToSettings(){
    QSettings settings(this->settingsPath);
    QString jsonText = this->getJson();
    settings.setValue(this->settingsKey, jsonText);
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

void MainWindow::loadJson(QString data){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsonArray = jsonDocument.array();

    foreach (QJsonValue jsonValue, jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        ui->listWidget->addItem(jsonObject["Value"].toString());
    }
}

void MainWindow::on_pushButton_clicked() //add new item
{
    QString text = ui->lineEdit->text();
    if(!text.isEmpty()){
        ui->listWidget->addItem(text);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_2_clicked() //delete selected
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
    int length = selectedItems.length();
    for(int i=0; i<length; i++){
        QListWidgetItem* item = selectedItems.at(i);
        delete item;
    }
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
