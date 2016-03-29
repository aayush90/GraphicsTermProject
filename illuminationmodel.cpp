#include "illuminationmodel.h"

IlluminationModel::IlluminationModel(){
    currentInstance = this;
}

void IlluminationModel::setViewerPosition(QVector3D pos){
    this->viewerPosition = pos;
}

void IlluminationModel::setLightPosition(QVector4D pos){
    this->lightPosition[0] = pos.x();
    this->lightPosition[1] = pos.y();
    this->lightPosition[2] = pos.z();
    this->lightPosition[3] = pos.w();
}

void IlluminationModel::setDiffused(QVector4D rgba, bool isEnable){
    this->isDiffused = isEnable;
    this->diffusedRGBA[0] = rgba.w();
    this->diffusedRGBA[1] = rgba.x();
    this->diffusedRGBA[2] = rgba.y();
    this->diffusedRGBA[3] = rgba.z();
}

void IlluminationModel::setSpecular(QVector4D rgba, bool isEnable){
    this->isSpecular = isEnable;
    this->specularRGBA[0] = rgba.w();
    this->specularRGBA[1] = rgba.x();
    this->specularRGBA[2] = rgba.y();
    this->specularRGBA[3] = rgba.z();
}

void IlluminationModel::setAmbient(QVector4D rgba, bool isEnable){
    this->isAmbient = isEnable;
    this->ambientRGBA[0] = rgba.w();
    this->ambientRGBA[1] = rgba.x();
    this->ambientRGBA[2] = rgba.y();
    this->ambientRGBA[3] = rgba.z();
}

void IlluminationModel::setSpotlight(vector<float> v, bool isEnable){
    this->isSpotlight = isEnable;
    this->spotDirection[0] = v[0];
    this->spotDirection[1] = v[1];
    this->spotDirection[2] = v[2];
    this->spotCutOff = v[3];
    this->spotExponent = v[4];
}


void IlluminationModel::init(){

  GLfloat mat_shininess[] = { 50.0 };
  glClearColor (0.5, 0.5, 0.5, 0.0);
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

  glMatrixMode(GL_PROJECTION);
  gluPerspective(50.0,1.0,1.0,10.0);
  glMatrixMode(GL_MODELVIEW);

  gluLookAt(this->viewerPosition.x(),this->viewerPosition.y(),this->viewerPosition.z(),/*eye*/
            0.0, 0.0, 0.0,/*center*/
            0.0, 1.0, 0.); /*up direction*/

  /* Adjust cube position to be asthetic angle. */
  //glTranslatef(0.0, 0.0, -1.0);
  /*glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-40, 0.0, 0.0, 1.0);*/
}


void IlluminationModel::drawbox(){

  for (int i = 0; i < 6*this->size; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&normal[i][0]);
    glVertex3fv(&vertex[face[i][0]][0]);
    glVertex3fv(&vertex[face[i][1]][0]);
    glVertex3fv(&vertex[face[i][2]][0]);
    glVertex3fv(&vertex[face[i][3]][0]);
    glEnd();
  }
}


void display(){

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  currentInstance->drawbox();

  glutSwapBuffers();
  glFlush ();
}

void IlluminationModel::reshape(int w,int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
    glOrtho (-1.5*(GLfloat)w/(GLfloat)h,1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void IlluminationModel::project(int argc,char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Testing OpenGL");
  this->init();

  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  glutMainLoop();
}
