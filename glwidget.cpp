#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoomfactor = 1.0;
    scale = 1.0;
    srand(time(NULL));
    isLightON = true;
    isSpotlight = isAmbient = isSpecular = isDiffused = false;
}

void GLWidget::setViewerPosition(QVector3D pos){
    this->viewerPosition = pos;
}

void GLWidget::setLight(bool _isLightON){
    this->isLightON = _isLightON;
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


QVector4D RandomNumber(){
    QVector4D v;
    v.setW((float)rand()/RAND_MAX);
    v.setX((float)rand()/RAND_MAX);
    v.setY((float)rand()/RAND_MAX);
    v.setZ((float)rand()/RAND_MAX);
    return v;
}


void GLWidget::initializeGL(){
    GLfloat mat_shininess[] = { 50.0 };
    qglClearColor (Qt::black);
    glShadeModel (GL_SMOOTH);


    /*Spot Light*/
    if(this->isSpotlight){
        cout<<"yes\n";
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
        GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glLightfv(GL_LIGHT0,GL_SPECULAR,this->specularRGBA);
    }

    GLfloat mat_emission[] = {0.0,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission);

    glLightfv(GL_LIGHT0,GL_POSITION,this->lightPosition);

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    if(!isLightON)
        glDisable(GL_LIGHTING);

    gluLookAt(this->viewerPosition.x(),this->viewerPosition.y(),this->viewerPosition.z(),/*eye*/
              0.0, 0.0, 0.0,/*center*/
              0.0, 1.0, 0.); /*up direction*/
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void GLWidget::drawbox(){
    if(vertex==NULL || face==NULL || normal==NULL)
        return;
    glLoadIdentity();
    glTranslatef(-1.5f,0.0f,-6.0f);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(scale,scale,scale);
    for (int i = 0; i < 6*size; i++) {
        glBegin(GL_QUADS);
        if(i%6==0 && (int)objectColor.size()<size){
            QVector3D v;
            v.setX((float)rand()/RAND_MAX);
            v.setY((float)rand()/RAND_MAX);
            v.setZ((float)rand()/RAND_MAX);
            objectColor.push_back(v);
        }
        if(i%6==0)
            glColor3f(objectColor[i/6].x(),objectColor[i/6].y(),objectColor[i/6].z());

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

void getOpenglCoordinates(int x,int y){
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    cout<<posX<<","<<posY<<","<<posZ<<endl;
}


void GLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();
    //getOpenglCoordinates(lastPos.x(),lastPos.y());
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
    if(zoomfactor<0)
        scale =1/(-zoomfactor);
    else
        scale = zoomfactor;
    update();
}

void GLWidget::clearData(){
    for(int i=0;i<6*size;i++){
        delete[] face[i];
        delete[] normal[i];
    }
    delete[] face;
    delete[] normal;

    for(int i=0;i<8*size;i++){
        delete[] vertex[i];
    }
    delete[] vertex;
    size = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoomfactor = 1.0;
    scale = 1.0;
    //objectColor.clear();
}



