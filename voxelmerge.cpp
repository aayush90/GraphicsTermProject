#include "voxelmerge.h"

VoxelMerge::VoxelMerge()
{
}

void VoxelMerge::findLocalMaxCuboid(QVector3D point){
    if(voxel[point])
            return;
    n_cuboids++;
    voxel[point] = this->n_cuboids;
    int x_opt = point.x();
    int y_opt = point.y();
    int z_opt = point.z();
    int max_width = 2,max_height = 2,max_depth = 2;

    for(int k=0;k<6;k++){
        int width = 2, height = 2, depth = 2;
        int x = point.x(), y = point.y(), z = point.z();
        bool temp;
        switch(k){
        case 0:// -X
            temp=true;
            while(temp){
                for(int i=y-height/2+1;i<=y+height/2-1;i++)
                    for(int j=z-depth/2+1;j<=z+depth/2-1;j++){
                        if(voxel.find(QVector3D(x-width/2-1,i,j))!=voxel.end()){
                            if(voxel[QVector3D(x-width/2-1,i,j)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp=false;
                    }
                if(temp){
                    x--;
                    width+=2;
                }
            }
            break;
        case 1: // +X
            temp = true;
            while(temp){
                for(int i=y-height/2+1;i<=y+height/2-1;i++)
                    for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
                        if(voxel.find(QVector3D(x+width/2+1,i,j))!=voxel.end()){
                            if(voxel[QVector3D(x+width/2+1,i,j)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp = false;
                if(temp){
                    x++;
                    width+=2;
                }
            }
            break;
        case 2: // -Y
            temp = true;
            while(temp){
                for(int i=x-width/2+1;i<=x+width/2-1;i++)
                    for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
                        if(voxel.find(QVector3D(i,y-height/2-1,j))!=voxel.end()){
                            if(voxel[QVector3D(i,y-height/2-1,j)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp = false;
                if(temp){
                    y--;
                    height+=2;
                }
            }
            break;
        case 3: // +Y
            temp = true;
            while(temp){
                for(int i=x-width/2+1;i<=x+width/2-1;i++)
                    for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
                        if(voxel.find(QVector3D(i,y+height/2+1,j))!=voxel.end()){
                            if(voxel[QVector3D(i,y+height/2+1,j)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp = false;
                if(temp){
                    y++;
                    height+=2;
                }
            }
            break;
        case 4: // -Z
            temp = true;
            while(temp){
                for(int i=x-width/2+1;i<=x+width/2-1;i++)
                    for(int j=y-height/2+1;j<=y+height/2-1;j++)
                        if(voxel.find(QVector3D(i,j,z-depth/2-1))!=voxel.end()){
                            if(voxel[QVector3D(i,j,z-depth/2-1)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp = false;
                if(temp){
                    z--;
                    depth+=2;
                }
            }
            break;
        case 5: // +Z
            temp = true;
            while(temp){
                for(int i=x-width/2+1;i<=x+width/2-1;i++)
                    for(int j=y-height/2+1;j<=y+height/2-1;j++)
                        if(voxel.find(QVector3D(i,j,z+depth/2+1))!=voxel.end()){
                            if(voxel[QVector3D(i,j,z+depth/2+1)]){
                                temp = false;
                                break;
                            }
                        }
                        else
                            temp = false;
                if(temp){
                    z++;
                    depth+=2;
                }
            }
            break;
        }
        if(width*height*depth > max_width*max_height*max_depth){
            x_opt = x;
            y_opt = y;
            z_opt = z;
            max_width = width;
            max_height = height;
            max_depth = depth;
        }
    }

    /*Update voxels*/
    if(max_width>2)
        for(int i=x_opt-max_width/2+1;i<=x_opt+max_width/2-1;i+=2){
            voxel[QVector3D(i,y_opt,z_opt)] = n_cuboids;
        }

    if(max_height>2)
        for(int i=y_opt-max_height/2+1;i<=y_opt+max_height/2-1;i+=2){
            voxel[QVector3D(x_opt,i,z_opt)] = n_cuboids;
        }

    if(max_depth>2)
        for(int i=z_opt-max_depth/2+1;i<=z_opt+max_depth/2-1;i+=2){
            voxel[QVector3D(x_opt,y_opt,i)] = n_cuboids;
        }
}



void VoxelMerge::findCuboidDimension(){
    for(unordered_map<QVector3D,int>::iterator it = voxel.begin();it!=voxel.end();it++){
        if(cuboid.find(it->second)==cuboid.end())
            cuboid[it->second] = make_pair(it->first,it->first);
        else{
            if(cuboid[it->second].first.x() > (*it).first.x()) cuboid[it->second].first.setX((*it).first.x());
            if(cuboid[it->second].second.x() < (*it).first.x()) cuboid[it->second].second.setX((*it).first.x());
            if(cuboid[it->second].first.y() > (*it).first.y()) cuboid[it->second].first.setY((*it).first.y());
            if(cuboid[it->second].second.y() < (*it).first.y()) cuboid[it->second].second.setY((*it).first.y());
            if(cuboid[it->second].first.z() > (*it).first.z()) cuboid[it->second].first.setZ((*it).first.z());
            if(cuboid[it->second].second.z() < (*it).first.z()) cuboid[it->second].second.setZ((*it).first.z());
        }
    }
}


void VoxelMerge::readInput(string filename){
    ifstream myFile(filename.c_str());
    string line;
    while(!myFile.eof()){
        getline(myFile,line);
        if(line.empty())
            continue;
        istringstream iss(line);
        int value;
        QVector3D p;
        if(iss >> value)
            p.setX(2*value);
        if(iss >> value)
            p.setY(2*value);
        if(iss >> value)
            p.setZ(2*value);
        voxel[p] = 0;
    }
    myFile.close();
}

void VoxelMerge::merge(){
    for(unordered_map<QVector3D,int>::iterator it = voxel.begin();it!=voxel.end();it++)
        findLocalMaxCuboid(it->first);

    findCuboidDimension();
}

void VoxelMerge::clearData(){
    voxel.clear();
    cuboid.clear();
    n_cuboids = 0;
}




