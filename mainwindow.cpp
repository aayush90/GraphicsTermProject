#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Constraints*/
    ui->viewer_xtext->setValidator(new QDoubleValidator());
    ui->viewer_ytext->setValidator(new QDoubleValidator());
    ui->viewer_ztext->setValidator(new QDoubleValidator());
    ui->light_xtext->setValidator(new QDoubleValidator());
    ui->light_ytext->setValidator(new QDoubleValidator());
    ui->light_ztext->setValidator(new QDoubleValidator());
    ui->spot_xtext->setValidator(new QDoubleValidator());
    ui->spot_ytext->setValidator(new QDoubleValidator());
    ui->spot_ztext->setValidator(new QDoubleValidator());
    ui->spot_exponentText->setValidator(new QDoubleValidator());

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector3D MainWindow::getViewerPosition(){
    if(ui->viewer_xtext->text().isEmpty() || ui->viewer_ytext->text().isEmpty() || ui->viewer_ztext->text().isEmpty())
        return QVector3D();

    QVector3D point;

    point.setX(ui->viewer_xtext->text().toFloat());
    point.setY(ui->viewer_ytext->text().toFloat());
    point.setZ(ui->viewer_ztext->text().toFloat());

    return point;
}

QVector4D MainWindow::getLightPosition(){
    if(ui->light_xtext->text().isEmpty() || ui->light_ytext->text().isEmpty() || ui->light_ztext->text().isEmpty())
        return QVector3D();

    QVector4D point;

    point.setX(ui->light_xtext->text().toFloat());
    point.setY(ui->light_ytext->text().toFloat());
    point.setZ(ui->light_ztext->text().toFloat());

    if(ui->positionalLightEnable->isChecked())
        point.setW(1.0);
    else
        point.setW(0.0);

    return point;
}

pair<bool,QVector4D> MainWindow::getDiffusedParams(){
    QVector4D rgba;
    bool enable = ui->diffused_enable->isChecked();

    if(enable){
        rgba.setW(ui->diffused_Rslider->value()/(float)ui->diffused_Rslider->maximum());
        rgba.setX(ui->diffused_Gslider->value()/(float)ui->diffused_Gslider->maximum());
        rgba.setY(ui->diffused_Bslider->value()/(float)ui->diffused_Bslider->maximum());
        rgba.setZ(ui->diffused_Aslider->value()/(float)ui->diffused_Aslider->maximum());
    }
    else{
        rgba.setW(1.0);
        rgba.setX(1.0);
        rgba.setY(1.0);
        rgba.setZ(1.0);
    }

    return make_pair(enable,rgba);
}


pair<bool,QVector4D> MainWindow::getSpecularParams(){
    QVector4D rgba;

    bool enable = ui->specular_enable->isChecked();

    if(enable){
        rgba.setW(ui->specular_Rslider->value()/(float)ui->specular_Rslider->maximum());
        rgba.setX(ui->specular_Gslider->value()/(float)ui->specular_Gslider->maximum());
        rgba.setY(ui->specular_Bslider->value()/(float)ui->specular_Bslider->maximum());
        rgba.setZ(ui->specular_Aslider->value()/(float)ui->specular_Aslider->maximum());
    }
    else{
        rgba.setW(1.0);
        rgba.setX(1.0);
        rgba.setY(1.0);
        rgba.setZ(1.0);
    }

    return make_pair(enable,rgba);
}

pair<bool,QVector4D> MainWindow::getAmbientParams(){
    QVector4D rgba;
    bool enable = ui->specular_enable->isChecked();

    if(enable){
        rgba.setW(ui->ambient_Rslider->value()/(float)ui->ambient_Rslider->maximum());
        rgba.setX(ui->ambient_Gslider->value()/(float)ui->ambient_Gslider->maximum());
        rgba.setY(ui->ambient_Bslider->value()/(float)ui->ambient_Bslider->maximum());
        rgba.setZ(ui->ambient_Aslider->value()/(float)ui->ambient_Aslider->maximum());
    }
    else{
        rgba.setW(0.0);
        rgba.setX(0.0);
        rgba.setY(0.0);
        rgba.setZ(1.0);
    }

    return make_pair(enable,rgba);
}


pair<bool,vector<float> > MainWindow::getSpotlightParams(){
    vector<float> v;

    bool enable = ui->spotlight_enable->isChecked();

    v.push_back(ui->spot_xtext->text().toFloat());
    v.push_back(ui->spot_ytext->text().toFloat());
    v.push_back(ui->spot_ztext->text().toFloat());
    v.push_back(ui->spot_cutoffSlider->value()/(float)ui->spot_cutoffSlider->maximum() * 80);
    v.push_back(ui->spot_exponentText->text().toFloat());

    return make_pair(enable,v);
}


void MainWindow::print(){
    QVector4D vp = this->getViewerPosition();
    cout<<"Viewer Position: "<<vp.x()<<","<<vp.y()<<","<<vp.z()<<","<<vp.w()<<endl;

    QVector4D lp = this->getLightPosition();
    cout<<"Light Position: "<<lp.x()<<","<<lp.y()<<","<<lp.z()<<","<<lp.w()<<endl;

    pair<bool,QVector4D> d = this->getDiffusedParams();
    cout<<"Diffused: "<<d.first<<","<<d.second.x()<<","<<d.second.y()<<","<<d.second.z()<<","<<d.second.w()<<endl;
    pair<bool,QVector4D> sp = this->getSpecularParams();
    cout<<"Specular: "<<sp.first<<","<<sp.second.x()<<","<<sp.second.y()<<","<<sp.second.z()<<","<<sp.second.w()<<endl;
    pair<bool,QVector4D> a = this->getAmbientParams();
    cout<<"Ambient: "<<a.first<<","<<a.second.x()<<","<<a.second.y()<<","<<a.second.z()<<","<<a.second.w()<<endl;
    pair<bool,vector<float> > st = this->getSpotlightParams();
    cout<<"SpotLight: "<<st.first<<","<<st.second[0]<<","<<st.second[1]<<","<<st.second[2]<<","<<st.second[3]<<","<<","<<st.second[4]<<endl;
}


void MainWindow::on_ok_accepted()
{
    /*Set Viewer Position*/
    ui->openglWidget->setViewerPosition(this->getViewerPosition());

    /*Set Light*/
    ui->openglWidget->setLightPosition(this->getLightPosition());

    pair<bool,QVector4D> diffused = this->getDiffusedParams();
    ui->openglWidget->setDiffused(diffused.second,diffused.first);

    pair<bool,QVector4D> specular = this->getSpecularParams();
    ui->openglWidget->setSpecular(specular.second,specular.first);

    pair<bool,QVector4D> ambient = this->getAmbientParams();
    ui->openglWidget->setAmbient(ambient.second,ambient.first);

    pair<bool,vector<float> > spotlight = this->getSpotlightParams();
    ui->openglWidget->setSpotlight(spotlight.second,spotlight.first);

    //print();
    setUpIlluminationModel();

    ui->openglWidget->update();

}

void MainWindow::on_ok_rejected()
{
    this->close();
}

void MainWindow::on_browseButton_clicked(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),"./","Text File (*.txt);;");
    ui->inputFileText->setText(filename);
    voxelMergeModel.readInput(filename.toStdString());
    voxelMergeModel.merge();
}


QVector3D CalculateNormal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]){

    double x = (v2[1]-v3[1])*(v1[2]-v2[2]) - (v2[2]-v3[2])*(v1[1]-v2[1]) ;
    double y = (v2[2]-v3[2])*(v1[0]-v2[0]) - (v2[0]-v3[0])*(v1[2]-v2[2]) ;
    double z = (v2[0]-v3[0])*(v1[1]-v2[1]) - (v2[1]-v3[1])*(v1[0]-v2[0]) ;

    return QVector3D(x,y,z);

}


void MainWindow::setUpIlluminationModel(){
    int s = voxelMergeModel.cuboid.size();
    GLfloat n[6*s][3];
    GLint f[6*s][4];
    GLfloat v[8*s][3];
    ui->openglWidget->size = s;

    int i=0;

    for(map<int,pair<QVector3D,QVector3D> >::iterator it=voxelMergeModel.cuboid.begin();it!=voxelMergeModel.cuboid.end();it++){
        double x = ((*it).second.first.x() + (*it).second.second.x())/4.0 ;
        double y = ((*it).second.first.y() + (*it).second.second.y())/4.0 ;
        double z = ((*it).second.first.z() + (*it).second.second.z())/4.0 ;

        double width = ((*it).second.second.x() - (*it).second.first.x() + 2)/2.0 ;
        double height = ((*it).second.second.y() - (*it).second.first.y() + 2)/2.0 ;
        double depth = ((*it).second.second.z() - (*it).second.first.z() + 2)/2.0 ;


        v[i][0] = x-width/2 ; v[i][1] = y-height/2 ; v[i][2] = z-depth/2 ;
        v[i+1][0] = x+width/2 ; v[i+1][1] = y-height/2 ; v[i+1][2] = z-depth/2 ;
        v[i+2][0] = x+width/2 ; v[i+2][1] = y+height/2 ; v[i+2][2] = z-depth/2 ;
        v[i+3][0] = x-width/2 ; v[i+3][1] = y+height/2 ; v[i+3][2] = z-depth/2 ;
        v[i+4][0] = x+width/2 ; v[i+4][1] = y+height/2 ; v[i+4][2] = z+depth/2 ;
        v[i+5][0] = x+width/2 ; v[i+5][1] = y-height/2 ; v[i+5][2] = z+depth/2 ;
        v[i+6][0] = x-width/2 ; v[i+6][1] = y-height/2 ; v[i+6][2] = z+depth/2 ;
        v[i+7][0] = x-width/2 ; v[i+7][1] = y+height/2 ; v[i+7][2] = z+depth/2 ;

        i+=8;

    }
    for(i=0;i<6*s;i+=6){
        f[i][0] = 8*i/6 ; f[i][1] = 8*i/6+1 ; f[i][2] = 8*i/6+2 ; f[i][3] = 8*i/6+3 ;
        f[i+1][0] = 8*i/6+1 ; f[i+1][1] = 8*i/6+5 ; f[i+1][2] = 8*i/6+4 ; f[i+1][3] = 8*i/6+2 ;
        f[i+2][0] = 8*i/6+4 ; f[i+2][1] = 8*i/6+5 ; f[i+2][2] = 8*i/6+6 ; f[i+2][3] = 8*i/6+7 ;
        f[i+3][0] = 8*i/6 ; f[i+3][1] = 8*i/6+3 ; f[i+3][2] = 8*i/6+7 ; f[i+3][3] = 8*i/6+6 ;
        f[i+4][0] = 8*i/6+2 ; f[i+4][1] = 8*i/6+4 ; f[i+4][2] = 8*i/6+7 ; f[i+4][3] = 8*i/6+3 ;
        f[i+5][0] = 8*i/6 ; f[i+5][1] = 8*i/6+6 ; f[i+5][2] = 8*i/6+5 ; f[i+5][3] = 8*i/6+1 ;
    }

    for(i=0;i<6*s;i++){
        QVector3D p = CalculateNormal(v[f[i][0]],v[f[i][1]],v[f[i][2]]);
        n[i][0] = p.x() ; n[i][1] = p.y() ; n[i][2] = p.z() ;
    }

    ui->openglWidget->vertex  = new GLfloat*[8*s];
    ui->openglWidget->face = new GLint*[6*s];
    ui->openglWidget->normal = new GLfloat*[6*s];

    for(i=0;i<6*s;i++){
        ui->openglWidget->normal[i] = new GLfloat[3];
        ui->openglWidget->face[i] = new GLint[4];
    }
    for(i=0;i<8*s;i++)
        ui->openglWidget->vertex[i] = new GLfloat[3];

    for(i=0;i<6*s;i++){
        ui->openglWidget->normal[i][0] = n[i][0]; ui->openglWidget->normal[i][1] = n[i][1]; ui->openglWidget->normal[i][2] = n[i][2];
        ui->openglWidget->face[i][0] = f[i][0]; ui->openglWidget->face[i][1] = f[i][1]; ui->openglWidget->face[i][2] = f[i][2]; ui->openglWidget->face[i][3] = f[i][3];
    }
    for(i=0;i<8*s;i++){
        ui->openglWidget->vertex[i][0] = v[i][0]; ui->openglWidget->vertex[i][1] = v[i][1]; ui->openglWidget->vertex[i][2] = v[i][2];
    }


}
