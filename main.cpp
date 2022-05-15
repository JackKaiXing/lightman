
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <sstream>

#include "platformhandle.h"

#include "scene/scene.h"
#include "engine/engine.h"
#include "view/view.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "objects/instancedtrianglemesh.h"
#include "managers/meshmanager.h"
#include "managers/materialmanager.h"
#include "texture/texture.h"
#include "texture/texturetypeheader.h"

#include "ply/ply.h"

struct AppConfig{
public:
    lightman::Scene* myScene = nullptr;
    lightman::Engine* myEngine = nullptr;
    lightman::View *myView = nullptr;
    lightman::Camera * myCamera = nullptr;
    lightman::SwapChain * mySwapChain = nullptr;
    lightman::Renderer *myRenderer = nullptr;
    
    unsigned int windowWidth = 394;
    unsigned int windowheight = 535;
    float contentScaleFactor = 1.0;
    float myfov = 0.0, mynear = 0.0, myfar = 0.0, myfocaldistance=0.0;
    float myeye[3], mytarget[3], myup[3];
public:
    void ParseLuxCoreScene(const std::string& file);
}myConfig;

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

void AppConfig::ParseLuxCoreScene(const std::string& file)
{
    // get managers
    lightman::MeshManager* mManager = lightman::MeshManager::GetInstance();

    // camera paras
    lightman::Camera::CameraType projection = lightman::Camera::CameraType::PERSPECTIVE;

    // init Texture Storage
    // we do not store this inside lightman, a texture would belong reference by only its material,
    // while would be not be shared between materials. Bacause I could not find meanings to share texture betweens materials.
    // when the material is deleted, the textures it owns would be deleted too.
    // Note the texture here is more like a textureNode, not a image.
    std::unordered_map<std::string, lightman::Texture*> texturesOfLucScene;

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
            lightman::InstancedTriangleMesh * iMesh = myScene->GetMesh(objectName);
            if(!iMesh)
            {
                iMesh = myScene->AddGetMesh(objectName);
            }

            subline = subline.substr(pos+1,subline.length());
            pos = subline.find_first_of(" = ");
            string arrtibuteName = subline.substr(0,pos);
            string arrtibuteValue = subline.substr(pos+2, subline.length());
            if (arrtibuteName.compare("ply") == 0)
            {
                // check if the ply mesh already loaded
                string plyFileName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                if(!(mManager->HasMesh(plyFileName)))
                {
                    // load new mwah
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
                    std::vector<float> points;
                    std::vector<float> normals;
                    std::vector<float> uvs;
                    std::vector<unsigned int> triangles;
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
                                if((hasVertex && j <= 2 ) ||
                                    (hasNormal && j >= 3 && j <= 5 )
                                        || (hasUV && j >= 6 ))
                                    ply_get_property (ply, elem_name, &vert_props[j]);
                            }

                            Vertex v;
                            points.resize(num_elems * 3);
                            if (hasNormal)
                                normals.resize(num_elems * 3);
                            if (hasUV)
                                uvs.resize(num_elems * 2);

                            for (int j = 0; j < num_elems; j++)
                            {    
                                ply_get_element (ply, (void *) &v);
                                points.at(3*j+0) = v.x;
                                points.at(3*j+1) = v.y;
                                points.at(3*j+2) = v.z;
                                if (hasNormal)
                                {
                                    normals.at(3*j+0) = v.nx;
                                    normals.at(3*j+1) = v.ny;
                                    normals.at(3*j+2) = v.nz;
                                }
                                if (hasUV)
                                {
                                    uvs.at(2*j+0) = v.s;
                                    uvs.at(2*j+1) = v.t;
                                }   
                            }
                        }
                        
                        if (equal_strings((char*)"face", elem_name))
                        {
                            bool hasIntensity = false;
                            for (int j = 0; j < nprops; j++)
                            {
                                if(equal_strings(plist[j]->name,(char*)"intensity"))
                                    hasIntensity = true;
                            }
                            
                            /* set up for getting face elements */
                            if (hasIntensity) {
                               ply_get_property (ply, elem_name, &face_props[0]);
                            }
                            ply_get_property (ply, elem_name, &face_props[1]);

                            /* grab all the face elements
                             we do not know how many triangles here, use reserve instead of resize*/
                            triangles.reserve(num_elems*3);
                            for (int j = 0; j < num_elems; j++)
                            {
                                /* grab and element from the file */
                                Face f;
                                ply_get_element (ply, (void *) &f);
                                triangles.push_back(f.verts[0]);
                                triangles.push_back(f.verts[1]);
                                triangles.push_back(f.verts[2]);
                                if (f.nverts == 4)
                                {
                                    triangles.push_back(f.verts[0]);
                                    triangles.push_back(f.verts[2]);
                                    triangles.push_back(f.verts[3]);
                                }
                            }
                        }
                    }
                    
                    /* close the PLY file */
                    ply_close (ply);
                    
                    mManager->CreateTriMesh(plyFileName, triangles, points, normals, uvs);
                }
                iMesh->SetMesh(plyFileName, mManager->GetMesh(plyFileName));
            }
            else if (arrtibuteName.compare("material") == 0)
            {
                string materialName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                std::string materialInstancename = objectName + "_" + materialName;
                lightman::MaterialInstance * mi = lightman::MaterialManager::GetInstance()->CreateMaterialInstance(
                        lightman::MaterialManager::GetInstance()->GetMaterial(materialName),materialInstancename);
                lightman::InstancedTriangleMesh * iMesh = myScene->GetMesh(objectName);
                iMesh->SetMaterialInstance(mi);
            }
            else if (arrtibuteName.compare("camerainvisible") == 0)
            {
                //
            }
            else if (arrtibuteName.compare("id") == 0)
            {
                //
            }
            else if (arrtibuteName.compare("transformation") == 0)
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
                mat4.Transpose();
                
                iMesh->SetTransform(mat4);
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
                mat4.Transpose();
                
                iMesh->GetMesh()->SetAppliedTransform(mat4);
            }
        }
        else if(objectType.compare("materials") == 0)
        {
            pos = subline.find_first_of(".");
            string materialName = subline.substr(0,pos);

            subline = subline.substr(pos+1,subline.length());
            pos = subline.find_first_of(" = ");
            string arrtibuteName = subline.substr(0,pos);
            string arrtibuteValue = subline.substr(pos+2, subline.length());

            if (arrtibuteName.compare("type") == 0)
            {
                string typeName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                lightman::MaterialManager::GetInstance()->CreateMaterial(
                    lightman::Material::StringToMaterialType(typeName),materialName);
            }
            // TODO other properties
        }
        else if(objectType.compare("camera") == 0)
        {
            pos = subline.find_first_of(" = ");
            string arrtibuteName = subline.substr(0,pos);
            string arrtibuteValue = subline.substr(pos+2, subline.length());
            
            if(arrtibuteName.compare("type") == 0)
            {
                if (arrtibuteValue.compare("\"perspective\""))
                {
                    projection = lightman::Camera::CameraType::PERSPECTIVE;
                }
            }
            else if(arrtibuteName.compare("fieldofview") == 0)
            {
                myfov = std::stod(arrtibuteValue);
            }
            else if(arrtibuteName.compare("cliphither") == 0)
            {
                mynear = std::stod(arrtibuteValue);
            }
            else if(arrtibuteName.compare("clipyon") == 0)
            {
                myfar = std::stod(arrtibuteValue);
            }
            else if(arrtibuteName.compare("focaldistance") == 0)
            {
                myfocaldistance = std::stod(arrtibuteValue);
            }
            else if(arrtibuteName.compare("lookat.orig") == 0)
            {
                stringstream ss(arrtibuteValue);
                for (int i = 0; i < 3; i++)
                {
                    ss >> myeye[i];
                }
            }
            else if(arrtibuteName.compare("lookat.target") == 0)
            {
                stringstream ss(arrtibuteValue);
                for (int i = 0; i < 3; i++)
                {
                    ss >> mytarget[i];
                }
            }
            else if(arrtibuteName.compare("up") == 0)
            {
                stringstream ss(arrtibuteValue);
                for (int i = 0; i < 3; i++)
                {
                    ss >> myup[i];
                }
            }
        }
        else if(objectType.compare("lights") == 0)
        {
            //
        }
        else if(objectType.compare("textures") == 0)
        {
            pos = subline.find_first_of(".");
            string texturelName = subline.substr(0, pos);

            subline = subline.substr(pos+1,subline.length());
            pos = subline.find_first_of(" = ");
            string arrtibuteName = subline.substr(0, pos);
            string arrtibuteValue = subline.substr(pos+2, subline.length());

            if (arrtibuteName.compare("type") == 0)
            {
                // parse and construct new texture
                string typeName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                lightman::TextureType type = lightman::Texture::StringToTextureType(typeName);
                lightman::Texture * texture = nullptr;
                switch (type)
                {
                case lightman::TextureType::IMAGEMAP:
                    {
                        lightman::ImagemapTexture * typedTexture = new lightman::ImagemapTexture(texturelName);
                        texture = static_cast<lightman::Texture*>(typedTexture);
                    }
                    break;
                
                default:
                    break;
                }
                texturesOfLucScene.insert({texturelName, texture});
            }
        }
    }
    scnfile.close();
}

void Setup()
{
    myConfig.myEngine = lightman::Engine::Create(lightman::backend::BackendType::OPENGL);
    myConfig.contentScaleFactor = lightmangui::GetBackScaleFactor();
    myConfig.myScene = new lightman::Scene();

    myConfig.ParseLuxCoreScene("/Users/XK/Downloads/LuxCore2.1Benchmark/LuxCoreScene/scene-src.scn");

    myConfig.myView = new lightman::View();
    myConfig.myView->SetScene(myConfig.myScene);

    myConfig.myCamera = new lightman::PerspectiveCamera();
    myConfig.myCamera->LookAt(myConfig.myeye, myConfig.mytarget, myConfig.myup);
    myConfig.myCamera->setProjection(myConfig.myfov, myConfig.mynear, myConfig.myfar, 
        (float)myConfig.windowWidth / (float)myConfig.windowheight,lightman::Camera::FovDirection::VERTICAL);
    myConfig.myView->SetCamera(myConfig.myCamera);
    
    void* nativeWindow = lightmangui::GetNativeWindow();
    myConfig.mySwapChain = myConfig.myEngine->CreateSwapChain(nativeWindow);
    myConfig.myRenderer = myConfig.myEngine->CreateRender(uint32_t(myConfig.windowWidth*myConfig.contentScaleFactor), 
        uint32_t(myConfig.windowheight*myConfig.contentScaleFactor), lightman::RenderType::RASTER_GPU);
}
void PreRender()
{

}

void PostRender()
{

}

void Render()
{
    PreRender();
    if (myConfig.myRenderer->BeginFrame(myConfig.mySwapChain))
    {
        myConfig.myRenderer->RenderFrame(myConfig.myView);
        myConfig.myRenderer->EndFrame();
        PostRender();
    }
}

void Destory()
{
    delete myConfig.myView;
    delete myConfig.myScene;
    delete myConfig.myCamera;

    lightman::Engine::DestroyInstance();
}

int main(int argc, const char* argv[])
{
    lightmangui::MainWindow(argc, argv, Setup, Render, myConfig.windowWidth, myConfig.windowheight);
    Destory();
    return 0;
}
