
#if defined(WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
#endif

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <sstream>
#include <algorithm>

#include "platformhandle.h"

#include "scene/scene.h"
#include "engine/engine.h"
#include "view/view.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "objects/instancedtrianglemesh.h"
#include "managers/meshmanager.h"
#include "managers/materialmanager.h"
#include "managers/imagemapmanager.h"
#include "materialnode/node.h"
#include "materialnode/nodetypeheader.h"
#include "materials/materialtypeheaders.h"

#include "ply/ply.h"
#include "magicenum/magic_enum.hpp"

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

    /*
        init Texture Storage
        Note the texture here is more like a textureNode, not a image.
        No Need to store this inside of material, will create EditorMaterial to manage those Texture Node for each material.
        Texture node between is not shared, no meanings to share.
    */
    std::unordered_map<std::string, lightman::Node*> mytexturesOfLucScene;
    std::unordered_map<std::string, uint32_t> mytextureUsageCount;
private:
    uint32_t myDefaultTextureCount = 0;

public:
    void ParseLuxCoreScene(const std::string& file);
    lightman::Node* GetOrCreateTextureFromAttributeValue(const std::string& attributeValue);
    void ClearTextures()
    {
        /*
        // No need to clear since those temporary texture are holded by typed materials and 
        // will be deleted internally
        for (auto iter = mytexturesOfLucScene.begin(); iter != mytexturesOfLucScene.end(); iter++)
        {
            delete iter->second;
        }*/
        mytexturesOfLucScene.clear();
        mytextureUsageCount.clear();
        
    }
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

lightman::Node* AppConfig::GetOrCreateTextureFromAttributeValue(const std::string& attributeValue)
{
    lightman::Node * result = nullptr;

    // from "value" to value
    std::string value = attributeValue.substr(2, attributeValue.length()-3);

    int pos = 0;
    pos = value.find_first_of(" ");
    if( pos > 0 )
    {
        // if there is white space, it is a float3 texture with no name
        std::stringstream ss(value);
        float x = 0.0, y = 0.0, z = 0.0;
        ss >> x >> y >> z;

        lightman::ConstFloat3Node * texture = new lightman::ConstFloat3Node("param" + std::to_string(myDefaultTextureCount), x, y, z);
        result = texture;

        mytexturesOfLucScene.insert({"defaultTexture_" + std::to_string(myDefaultTextureCount++), texture});
    }
    else
    {
        auto iter = mytexturesOfLucScene.find(value);
        if( iter != mytexturesOfLucScene.end())
        {
            // it is the name of the texture created previously
            result = iter->second;

            // add count: this would not be needed later, just count if the parsed textures has been used or not.
            mytextureUsageCount.find(value)->second++;
        }
        else
        {
            // it is a const float texture
            std::stringstream ss(value);
            float x = 0.0;
            ss >> x;
            lightman::ConstFloatNode * texture = new lightman::ConstFloatNode("param"+ std::to_string(myDefaultTextureCount), x);
            result = texture;

            mytexturesOfLucScene.insert({"defaultTexture_" + std::to_string(myDefaultTextureCount++), texture});
        }
    }
    
    // if texture is still empty, we must be wrong with this parsing code
    assert(result);

    return result;
}
void AppConfig::ParseLuxCoreScene(const std::string& file)
{
    // get managers
    lightman::MeshManager* mManager = lightman::MeshManager::GetInstance();
    lightman::ImagemapManager* imManager = lightman::ImagemapManager::GetInstance();
    lightman::MaterialManager* mtManager = lightman::MaterialManager::GetInstance();

    // camera paras
    lightman::Camera::CameraType projection = lightman::Camera::CameraType::PERSPECTIVE;

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
                                delete f.verts;
                            }
                        }

                        delete elem_name;
                        
                        for (int j = 0; j < nprops; j++)
                        {
                            delete plist[j];
                        }
                        delete plist;
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
                lightman::InstancedTriangleMesh * iMesh = myScene->GetMesh(objectName);
                iMesh->SetMaterial(mtManager->GetMaterial(materialName));
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
                std::transform(typeName.begin(), typeName.end(), typeName.begin(), ::toupper);

                auto typeValue = magic_enum::enum_cast<lightman::Material::MaterialType>(typeName);
                lightman::Material::MaterialType type = lightman::Material::MaterialType::MAX_MATERIALTYPE_COUNT;
                if (typeValue.has_value())
                    type = typeValue.value();

                mtManager->CreateMaterial(type,materialName);
            }
            else
            {
                lightman::Material * material = mtManager->GetMaterial(materialName);
                
                switch (material->getMaterialType())
                {
                    case lightman::Material::MaterialType::GLOSSY:
                        {
                            lightman::GlossyMaterial* typedMaterial = dynamic_cast<lightman::GlossyMaterial*>(material);
                            if (arrtibuteName == "kd")
                            {
                                typedMaterial->SetKd(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::MATTE:
                        {
                            lightman::MatteMaterial* typedMaterial = dynamic_cast<lightman::MatteMaterial*>(material);
                            if (arrtibuteName == "kd")
                            {
                                typedMaterial->SetKd(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::METAL:
                        {
                            lightman::MetalMaterial* typedMaterial = dynamic_cast<lightman::MetalMaterial*>(material);
                            if (arrtibuteName == "fresnel")
                            {
                                typedMaterial->SetFresnel(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::MATTETRANSLUCENT:
                        {
                            lightman::MatteTranslucentMaterial* typedMaterial = dynamic_cast<lightman::MatteTranslucentMaterial*>(material);
                            if (arrtibuteName == "kr")
                            {
                                typedMaterial->SetKr(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                            if (arrtibuteName == "kt")
                            {
                                typedMaterial->SetKt(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::GLASS:
                        {
                            lightman::GlassMaterial* typedMaterial = dynamic_cast<lightman::GlassMaterial*>(material);
                            if (arrtibuteName == "kr")
                            {
                                typedMaterial->SetKr(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                            if (arrtibuteName == "kt")
                            {
                                typedMaterial->SetKt(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::VELVET:
                        {
                            lightman::VelvetMaterial* typedMaterial = dynamic_cast<lightman::VelvetMaterial*>(material);
                            if (arrtibuteName == "kd")
                            {
                                typedMaterial->SetKd(GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            }
                        }
                        break;
                    case lightman::Material::MaterialType::GLOSSYCOATING:
                        {
                            lightman::GlossyCoatingMaterial* typedMaterial = dynamic_cast<lightman::GlossyCoatingMaterial*>(material);
                            if (arrtibuteName == "base")
                            {
                                // base materials
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
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
                string typeName = arrtibuteValue.substr(2,arrtibuteValue.length()-3) + "_tex";
                std::transform(typeName.begin(), typeName.end(), typeName.begin(), ::toupper);
                auto typeValue = magic_enum::enum_cast<lightman::NodeType>(typeName);
                lightman::NodeType type;
                if (typeValue.has_value())
                    type = typeValue.value();
                else
                    assert(0);
                lightman::Node * texture = nullptr;
                void* typedTexture = nullptr;
                // C++ has no mechanism to create objects whose types are determined at runtime
                // https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
                switch (type)
                {
                case lightman::NodeType::IMAGEMAP_TEX:
                    {
                        typedTexture = new lightman::ImagemapNode(texturelName);
                    }
                    break;
                case lightman::NodeType::MIX_TEX:
                    {
                        typedTexture = new lightman::MixNode(texturelName);
                    }
                    break;
                case lightman::NodeType::SCALE_TEX:
                    {
                        typedTexture = new lightman::ScaleNode(texturelName);
                    }
                    break;
                case lightman::NodeType::SUBTRACT_TEX:
                    {
                        typedTexture = new lightman::SubtractNode(texturelName);
                    }
                    break;
                case lightman::NodeType::BAND_TEX:
                    {
                        typedTexture = new lightman::BandNode(texturelName);
                    }
                    break;
                case lightman::NodeType::FRESNELCOLOR_TEX:
                    {
                        typedTexture = new lightman::FresnelColorNode(texturelName);
                    }
                    break;
                
                default:
                    assert(0);
                    break;
                }
                texture = static_cast<lightman::Node*>(typedTexture);
                mytexturesOfLucScene.insert({texturelName, texture});
                mytextureUsageCount.insert({texturelName, 0});
            }
            else
            {
                lightman::Node* texture = mytexturesOfLucScene.find(texturelName)->second;
                
                switch (texture->GetType())
                {
                case lightman::NodeType::IMAGEMAP_TEX:
                    {
                        lightman::ImagemapNode * typedTexture = dynamic_cast<lightman::ImagemapNode*>(texture);
                        if (arrtibuteName == "file")
                        {
                            std::string imageName = arrtibuteValue.substr(2,arrtibuteValue.length()-3);
                            lightman::Imagemap* image = nullptr;
                            image = imManager->GetImagemap(imageName);
                            if (!image)
                            {
                                image = imManager->LoadImagemap(filePath, imageName);
                            }
                            typedTexture->SetImageMap(image);
                        }
                        else if (arrtibuteName == "gain")
                        {
                            
                        }
                        else if (arrtibuteName == "gamma")
                        {
                            
                        }
                        else if (arrtibuteName == "storage")
                        {
                            
                        }
                        else if (arrtibuteName == "mapping.type")
                        {
                            
                        }
                        else if (arrtibuteName == "mapping.uvindex")
                        {
                            
                        }
                        else if (arrtibuteName == "mapping.rotation")
                        {
                            
                        }
                        else if (arrtibuteName == "mapping.uvscale")
                        {
                            
                        }
                        else if (arrtibuteName == "mapping.uvdelta")
                        {
                            
                        }
                        else
                            std::cout << "Unknown Parameter: "<< arrtibuteName << "For Texture Type: " << 
                            magic_enum::enum_name(texture->GetType()) << std::endl;
                    }
                    break;
                case lightman::NodeType::MIX_TEX:
                    {
                        lightman::MixNode * typedTexture = dynamic_cast<lightman::MixNode*>(texture);
                        if (arrtibuteName == "amount")
                        {
                            typedTexture->SetAmount(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                            
                        }
                        else if (arrtibuteName == "texture1")
                        {
                            typedTexture->SetTex1(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                        else if (arrtibuteName == "texture2")
                        {
                            typedTexture->SetTex2(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                    }
                    break;
                case lightman::NodeType::SCALE_TEX:
                    {
                        lightman::ScaleNode * typedTexture = dynamic_cast<lightman::ScaleNode*>(texture);
                        if (arrtibuteName == "texture1")
                        {
                            typedTexture->SetTex1(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                        else if (arrtibuteName == "texture2")
                        {
                            typedTexture->SetTex2(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                    }
                    break;
                case lightman::NodeType::SUBTRACT_TEX:
                    {
                        lightman::SubtractNode * typedTexture = dynamic_cast<lightman::SubtractNode*>(texture);
                        if (arrtibuteName == "texture1")
                        {
                            typedTexture->SetTex1(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                        else if (arrtibuteName == "texture2")
                        {
                            typedTexture->SetTex2(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                    }
                    break;
                case lightman::NodeType::BAND_TEX:
                    {
                        lightman::BandNode * typedTexture = dynamic_cast<lightman::BandNode*>(texture);
                        if (arrtibuteName == "amount")
                        {
                            typedTexture->SetAmount(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                        if (arrtibuteName.find("offset") != std::string::npos)
                        {
                            typedTexture->AddOffset(std::atoi(arrtibuteValue.c_str()));
                        }
                        if (arrtibuteName.find("value") != std::string::npos)
                        {
                            std::stringstream ss(arrtibuteValue);
                            float x = 0.0, y = 0.0, z = 0.0;
                            ss >> x >> y >> z;
                            typedTexture->AddValue({x,y,z});
                        }
                    }
                    break;
                case lightman::NodeType::FRESNELCOLOR_TEX:
                    {
                        lightman::FresnelColorNode * typedTexture = dynamic_cast<lightman::FresnelColorNode*>(texture);
                        if (arrtibuteName == "kr")
                        {
                            typedTexture->SetKr(
                                GetOrCreateTextureFromAttributeValue(arrtibuteValue));
                        }
                    }
                    break;
                
                default:
                    assert(0);
                    break;
                }
            }
        }
    }
    scnfile.close();
}

void Setup()
{
    // Engine
    myConfig.myEngine = lightman::Engine::Create(lightman::backend::BackendType::OPENGL);
    myConfig.contentScaleFactor = lightmangui::GetBackScaleFactor();

    // Scene
    myConfig.myScene = new lightman::Scene();
#if defined(WIN32)
    myConfig.ParseLuxCoreScene("C:/dev/luxcorerender/LuxCoreTestScenes/scenes/LuxCore2.1Benchmark/LuxCoreScene/scene.scn");
    // myConfig.ParseLuxCoreScene("C:/dev/luxcorerender/LuxCoreTestScenes/scenes/3-spheres/scene.scn");
#elif defined(__APPLE__)
    myConfig.ParseLuxCoreScene("/Users/XK/Downloads/LuxCore2.1Benchmark/LuxCoreScene/scene-src.scn");
#endif
    
    // Camera
    myConfig.myCamera = new lightman::PerspectiveCamera();
    myConfig.myCamera->LookAt(myConfig.myeye, myConfig.mytarget, myConfig.myup);
    myConfig.myCamera->setProjection(myConfig.myfov, myConfig.mynear, myConfig.myfar, 
        (float)myConfig.windowWidth / (float)myConfig.windowheight,lightman::Camera::FovDirection::VERTICAL);

    // View
    myConfig.myView = new lightman::View();
    myConfig.myView->SetScene(myConfig.myScene);
    myConfig.myView->SetCamera(myConfig.myCamera);
    
    // SwapChain
    void* nativeWindow = lightmangui::GetNativeWindow();
    myConfig.mySwapChain = myConfig.myEngine->CreateSwapChain(nativeWindow);

    // Renderer
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
    myConfig.ClearTextures();

    delete myConfig.myView;
    delete myConfig.myScene;
    delete myConfig.myCamera;
    
    lightman::Engine::GetInstance()->DestroyRender(myConfig.myRenderer);
    lightman::Engine::GetInstance()->DestroySwapChain(myConfig.mySwapChain);
    
    lightman::Engine::DestroyInstance();
}

#if defined(WIN32)
    int WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
    {
        lightmangui::MainWindow(0, (const char**)nullptr, Setup, Render, Destory, myConfig.windowWidth, myConfig.windowheight);
        return 0;
    }
#elif defined(__APPLE__)
    int main(int argc, const char* argv[])
    {
        lightmangui::MainWindow(argc, argv, Setup, Render, Destory, myConfig.windowWidth, myConfig.windowheight);
        return 0;
    }
#endif


