#ifndef ILLUMINATIONMODEL_H
#define ILLUMINATIONMODEL_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <QVector3D>
#include <QVector4D>
#include <vector>
#include <QGLWidget>

using namespace std;

class IlluminationModel;

IlluminationModel *currentInstance;

class IlluminationModel
{
public:
    GLfloat **normal;
    GLfloat **vertex;
    GLint **face;
    int size;
    IlluminationModel();

    void setViewerPosition(QVector3D);
    void setLightPosition(QVector4D);
    void setDiffused(QVector4D,bool);
    void setSpecular(QVector4D,bool);
    void setSpotlight(vector<float>,bool);
    void setAmbient(QVector4D,bool);

    void drawbox();
    void project(int,char**);

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

    void init();
    void reshape(int,int);
};

#endif // ILLUMINATIONMODEL_H
