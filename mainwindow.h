#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <filesystem>
#include <ios>
#include <fstream>
#include <iostream>

#include <QMainWindow>
#include <QFileDialog>

#include <qdebug.h>

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
    void saveFile();
    void openFile();
    void filenameExists();
    void closeFile();

private:
    Ui::MainWindow* ui;
    bool fileExist(std::string path);
};
#endif // MAINWINDOW_H
