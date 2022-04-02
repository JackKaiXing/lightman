
#include <fstream>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "ply/ply.h"

#include "geometry/point.h"
#include "geometry/normal.h"
#include "geometry/uv.h"
#include "geometry/triangle.h"
#include "scene/scene.h"
#include "math/matrix4x4.h"

using namespace std;

namespace lightman
{
namespace scene
{
// ----------------------------------------------------------------------------
typedef struct Vertex {
  float x, y ,z, nx, ny, nz, s, t;             /* the usual 3-space position, normal of a vertex */
} Vertex;
static constexpr int NUM_V_PROPS = 8;
PlyProperty vert_props[NUM_V_PROPS] = { /* list of property information for a vertex */
  {(char*)"x",  PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x),  0, 0, 0, 0},
  {(char*)"y",  PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y),  0, 0, 0, 0},
  {(char*)"z",  PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z),  0, 0, 0, 0},
  {(char*)"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
  {(char*)"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
  {(char*)"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
  {(char*)"s",  PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,s),  0, 0, 0, 0},
  {(char*)"t",  PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,t),  0, 0, 0, 0},
};
typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  unsigned int *verts;              /* vertex index list */
} Face;
PlyProperty face_props[] = { /* list of property information for a vertex */
  {(char*)"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {(char*)"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};
// ----------------------------------------------------------------------------
void Scene::Parse(const string& file)
{
    // hard coded, deprecated later
    string filePath = file.substr(0, file.find_last_of("/")+1);

    ifstream scnfile;
    scnfile.open(file);
    if(!scnfile.is_open())
        assert(0);

    std::string line;
    while (getline(scnfile,line))
    {
        if (line.length() == 0)
			continue;
        
        int pos;
        pos = line.find_first_of(".");
        string subline = line.substr(pos+1,line.length());
        pos = subline.find_first_of(".");
        string objectType;
        objectType = subline.substr(0,pos);
        subline = subline.substr(pos+1, subline.length());
        
        if (objectType.compare("objects") == 0)
        {
            pos = subline.find_first_of(".");
            string objectName = subline.substr(0,pos);
            InstancedTriangleMesh * iMesh = nullptr;
            if(m_iMeshes.find(objectName) == m_iMeshes.end())
            {
                iMesh = new InstancedTriangleMesh();
                m_iMeshes.insert({objectName,iMesh});
            }
            else
            {
                iMesh = m_iMeshes.find(objectName)->second;
            }

            subline = subline.substr(pos+1,subline.length());
            pos = subline.find_first_of(" = ");
            string arrtibuteName = subline.substr(0,pos);
            string arrtibuteValue = subline.substr(pos+2, subline.length());
            if (arrtibuteName.compare("ply") == 0)
            {
                // check if the ply mesh already loaded
                string plyFileName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                std::unordered_map<string, TriangleMesh*>::iterator iter = m_meshes.find(plyFileName);
                if(iter != m_meshes.end())
                {
                    // bind mesh to imesh
                    iMesh->setMesh(iter->second);
                }
                else
                {
                    string plyFilePath = filePath;
                    plyFilePath.append(plyFileName);

                    // parse ply file, http://paulbourke.net/dataformats/ply/plytest.c
                    int nelems;
                    char **elist;
                    int file_type;
                    float version;
                    char *elem_name;
                    PlyProperty **plist;
                    int nprops;
                    int num_elems;

                    /* open a PLY file for reading */
                    PlyFile *ply;
                    ply = ply_open_for_reading(plyFilePath.c_str(), &nelems, &elist, &file_type, &version);

                    /* read data */
                    bool hasVertex = false;
                    bool hasNormal = false;
                    bool hasUV = false;
                    std::vector<Point> points;
                    std::vector<Normal> normals;
                    std::vector<UV> uvs;
                    std::vector<Triangle> triangles;
                    for (int i = 0; i < nelems; i++)
                    {
                        /* get the description of current element */
                        elem_name = elist[i];
                        plist = ply_get_element_description(ply, elem_name, &num_elems, &nprops);

                        if (equal_strings((char*)"vertex", elem_name))
                        {
                            for (int j = 0; j < nprops; j++)
                            {
                                if(equal_strings(plist[j]->name,(char*)"x"))
                                    hasVertex = true;
                                if(equal_strings(plist[j]->name,(char*)"nx"))
                                    hasNormal = true;
                                if(equal_strings(plist[j]->name,(char*)"s"))
                                    hasUV = true;
                            }
                            assert(hasVertex);
                            
                            for (int j = 0; j < NUM_V_PROPS; j++)
                            {
                                ply_get_property (ply, elem_name, &vert_props[j]);
                            }

                            Vertex v;
                            points.resize(num_elems);
                            if (hasNormal)
                                normals.resize(num_elems);
                            if (hasUV)
                                uvs.resize(num_elems);

                            for (int j = 0; j < num_elems; j++)
                            {    
                                ply_get_element (ply, (void *) &v);
                                points.at(j) = {v.x, v.y, v.z};
                                if (hasNormal)
                                    normals.at(j) = {v.nx, v.ny, v.nz};
                                if (hasUV)
                                    uvs.at(j) = {v.s, v.t};  
                            }
                        }
                        
                        if (equal_strings((char*)"face", elem_name))
                        {
                            /* set up for getting face elements */
                            ply_get_property (ply, elem_name, &face_props[0]);
                            ply_get_property (ply, elem_name, &face_props[1]);

                            /* grab all the face elements */
                            triangles.reserve(num_elems);
                            for (int j = 0; j < num_elems; j++)
                            {
                                /* grab and element from the file */
                                Face f;
                                ply_get_element (ply, (void *) &f);
                                triangles.push_back({f.verts[0],f.verts[1],f.verts[2]});
                                if (f.nverts == 4)
                                {
                                    triangles.push_back({f.verts[0],f.verts[2],f.verts[3]});
                                }
                            }
                        }
                    }
                    
                    /* close the PLY file */
                    ply_close (ply);
                    
                    TriangleMesh * mesh = new TriangleMesh(points, triangles);
                    if (hasNormal)
                        mesh->InitNormals(normals);
                    if (hasUV)
                        mesh->InitUVs(uvs); 

                    // bind mesh to imesh
                    iMesh->setMesh(mesh);
                }
            }
            else if (arrtibuteName.compare("material") == 0)
            {
                //
            }
            else if (arrtibuteName.compare("camerainvisible") == 0)
            {
                //
            }
            else if (arrtibuteName.compare("id") == 0)
            {
                //
            }
            else if (arrtibuteName.compare("appliedtransformation") == 0)
            {
                float m[4][4];
                stringstream ss(arrtibuteValue);
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                         ss >> m[i][j];
                    }
                }
                Matrix4X4 mat4(m);
                iMesh->getTransform().setMatrix4(mat4);
            }
        }
        else if(objectType.compare("materials") == 0)
        {
            //
        }
        else if(objectType.compare("camera") == 0)
        {
            //
        }
        else if(objectType.compare("lights") == 0)
        {
            //
        }
        else if(objectType.compare("textures") == 0)
        {
            //
        }
        
    }
    scnfile.close(); 
}
// ----------------------------------------------------------------------------
} // namespace scene
} // namespace lightman
