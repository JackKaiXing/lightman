#ifndef _LIGHTMAN_FRESNELCOLORNODE_H
#define _LIGHTMAN_FRESNELCOLORNODE_H

#include "materialnode/node.h"

namespace lightman
{
    class FresnelColorNode : public Node
    {
    public:
        FresnelColorNode() = delete;
        FresnelColorNode(const std::string& name) : Node(name){};
        ~FresnelColorNode(){};

        void SetKr(const Node* kr);

        NodeType GetType() const override {return NodeType::FRESNELCOLOR_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};

    private:
        const Node* m_Kr;
    };
}

#endif // _LIGHTMAN_FRESNELCOLORNODE_H
