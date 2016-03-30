#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glut.h>
#include <vector>
#include <QVector3D>
#include <QVector4D>
#include <QGLWidget>
#include <iostream>
#include <QPoint>
#include <QMouseEvent>
#include <cstdlib>
#include <time.h>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    GLfloat **normal=NULL;
    GLfloat **vertex=NULL;
    GLint **face=NULL;
    int size=0;

    void setViewerPosition(QVector3D);
    void setLightPosition(QVector4D);
    void setDiffused(QVector4D,bool);
    void setSpecular(QVector4D,bool);
    void setSpotlight(vector<float>,bool);
    void setAmbient(QVector4D,bool);
    void setLight(bool);

    void initializeGL();
    void paintGL();
    void resizeGL(int,int);

    void clearData();

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int);
    void setYRotation(int);
    void setZRotation(int);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int);
    void yRotationChanged(int);
    void zRotationChanged(int);

private:
    QVector3D viewerPosition;
    GLfloat lightPosition[4];
    GLfloat diffusedRGBA[4];
    GLfloat specularRGBA[4];
    GLfloat spotDirection[3];
    GLfloat spotCutOff;
    GLfloat spotExponent;
    GLfloat ambientRGBA[4];
    bool isDiffused;
    bool isSpecular;
    bool isSpotlight;
    bool isAmbient;
    bool isLightON;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    float zoomfactor,scale;
    vector<QVector3D> objectColor;

    void drawbox();

};

#endif // GLWIDGET_H
