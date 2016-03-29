#ifndef VOXELMERGE_H
#define VOXELMERGE_H

#include <map>
#include <unordered_map>
#include <QVector3D>
#include <fstream>
#include <sstream>


using namespace std;


namespace std {

  template <>
  struct hash<QVector3D>
  {
    std::size_t operator()(const QVector3D& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      return ((hash<int>()(k.x())
               ^ (hash<int>()(k.y()) << 1)) >> 1)
               ^ (hash<int>()(k.z()) << 1);
    }
  };
}



class VoxelMerge
{
public:
    map<int,pair<QVector3D,QVector3D> > cuboid;
    VoxelMerge();
    void merge();
    void readInput(string filename);

private:
    int n_cuboids;
    unordered_map<QVector3D,int> voxel;
    void findLocalMaxCuboid(QVector3D);
    void findCuboidDimension();
    void createOBJ();

};

#endif // VOXELMERGE_H
