#ifndef _LIGHTMAN_IMAGEMAPTNODE_H
#define _LIGHTMAN_IMAGEMAPTNODE_H

#include "materialnode/node.h"
#include "materialnode/imagemap.h"
#include "backend/driverbase.h"

namespace lightman
{
    class ImagemapNode : public Node
    {
    public:
        ImagemapNode(){};
        ImagemapNode(const std::string& name) : Node(name){};
        ~ImagemapNode(){};

        NodeType GetType() const override {return NodeType::IMAGEMAP_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        void SetImageMap(Imagemap* im);
        bool IsAllowedExposeAsUniform() const override {return true;};

        void setExposeAsUniform(bool enable);
    private:
        Imagemap * m_imagemap = nullptr;
        bool m_enableExpose = false;
    };
}

#endif // _LIGHTMAN_IMAGEMAPTNODE_H
