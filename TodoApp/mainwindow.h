#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_exportJson_clicked();

    void on_loadJson_clicked();

    void on_clearAll_clicked();

private:
    QString settingsFile;
    QString settingsKey;

    Ui::MainWindow *ui;
    void loadItemsFromSettings();
    void saveItemsToSettings();
    QString getJson();
    void loadJson(QString data);

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
