#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include"ImageInfoList.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr): QMainWindow(parent){
        imageInfoList=new ImageInfoList(this);
        setCentralWidget(imageInfoList);
    }
    ~MainWindow(){}
private:
    ImageInfoList* imageInfoList;
};
#endif // MAINWINDOW_H
