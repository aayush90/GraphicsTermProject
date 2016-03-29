#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoomfactor = 1.0;
}

void GLWidget::setViewerPosition(QVector3D pos){
    this->viewerPosition = pos;
}

void GLWidget::setLightPosition(QVector4D pos){
    this->lightPosition[0] = pos.x();
    this->lightPosition[1] = pos.y();
    this->lightPosition[2] = pos.z();
    this->lightPosition[3] = pos.w();
}

void GLWidget::setDiffused(QVector4D rgba, bool isEnable){
    this->isDiffused = isEnable;
    this->diffusedRGBA[0] = rgba.w();
    this->diffusedRGBA[1] = rgba.x();
    this->diffusedRGBA[2] = rgba.y();
    this->diffusedRGBA[3] = rgba.z();
}

void GLWidget::setSpecular(QVector4D rgba, bool isEnable){
    this->isSpecular = isEnable;
    this->specularRGBA[0] = rgba.w();
    this->specularRGBA[1] = rgba.x();
    this->specularRGBA[2] = rgba.y();
    this->specularRGBA[3] = rgba.z();
}

void GLWidget::setAmbient(QVector4D rgba, bool isEnable){
    this->isAmbient = isEnable;
    this->ambientRGBA[0] = rgba.w();
    this->ambientRGBA[1] = rgba.x();
    this->ambientRGBA[2] = rgba.y();
    this->ambientRGBA[3] = rgba.z();
}

void GLWidget::setSpotlight(vector<float> v, bool isEnable){
    this->isSpotlight = isEnable;
    this->spotDirection[0] = v[0];
    this->spotDirection[1] = v[1];
    this->spotDirection[2] = v[2];
    this->spotCutOff = v[3];
    this->spotExponent = v[4];
}


void GLWidget::initializeGL(){
    GLfloat mat_shininess[] = { 50.0 };
    qglClearColor (Qt::yellow);
    glShadeModel (GL_SMOOTH);

    /*Spot Light*/
    if(this->isSpotlight){
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,this->spotDirection);
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,this->spotCutOff);
        glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,this->spotExponent);
    }

    /*Diffused Light*/
    if(this->isDiffused){
        glLightfv(GL_LIGHT0,GL_DIFFUSE,this->diffusedRGBA);
    }

    /*Ambient Light*/
    if(this->isAmbient){
        glLightfv(GL_LIGHT0,GL_AMBIENT,this->ambientRGBA);
    }

    /*Specular Light*/
    if(this->isSpecular){
        glLightfv(GL_LIGHT0,GL_SPECULAR,this->specularRGBA);
    }

    glLightfv(GL_LIGHT0,GL_POSITION,this->lightPosition);

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

//    glMatrixMode(GL_PROJECTION);
    //gluPerspective(50.0,1.0,1.0,10.0);
//    glMatrixMode(GL_MODELVIEW);

    gluLookAt(this->viewerPosition.x(),this->viewerPosition.y(),this->viewerPosition.z(),/*eye*/
              0.0, 0.0, 0.0,/*center*/
              0.0, 1.0, 0.); /*up direction*/

    /* Adjust cube position to be asthetic angle. */
    //glTranslatef(0.0, 0.0, -1.0);
    /*glRotatef(60, 1.0, 0.0, 0.0);
    glRotatef(-40, 0.0, 0.0, 1.0);*/
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    gluPerspective(100.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    //glOrtho(0,w, 0,h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//}

//    int side = min(w, h);
//    glViewport((w - side) / 2, (h - side) / 2, side, side);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//#else
//    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//#endif
//    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::drawbox(){
    if(vertex==NULL || face==NULL || normal==NULL)
        return;
    glLoadIdentity();
    glTranslatef(-1.5f,0.0f,-6.0f);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(zoomfactor,zoomfactor,1.0);
    //glScalef(0.5,0.5,0.5);
    for (int i = 0; i < 6*this->size; i++) {
        glBegin(GL_QUADS);
        glColor3f(0.5,0.5,0.5);
        glNormal3fv(&normal[i][0]);
        glVertex3fv(&vertex[face[i][0]][0]);
        glVertex3fv(&vertex[face[i][1]][0]);
        glVertex3fv(&vertex[face[i][2]][0]);
        glVertex3fv(&vertex[face[i][3]][0]);
        glEnd();
    }
}

void GLWidget::paintGL(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    initializeGL();
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    drawbox();
}


static void qNormalizeAngle(int &angle){
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event){
    int numDegrees = event->delta()/8;
    zoomfactor += numDegrees/15;
    update();
}

