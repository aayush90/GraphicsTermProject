#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QVector3D>
#include <QVector4D>
#include <vector>
#include <QFileDialog>
#include "voxelmerge.h"
#include "glwidget.h"


using namespace std;

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
    void on_ok_accepted();

    void on_ok_rejected();

    void on_browseButton_clicked();

private:
    Ui::MainWindow *ui;
    VoxelMerge voxelMergeModel;
    QVector3D getViewerPosition();
    QVector4D getLightPosition();
    pair<bool,QVector4D> getDiffusedParams();
    pair<bool,QVector4D> getSpecularParams();
    pair<bool,vector<float> > getSpotlightParams();
    pair<bool,QVector4D> getAmbientParams();
    void setUpIlluminationModel();
    void print();
};

#endif // MAINWINDOW_H
