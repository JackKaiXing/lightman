#ifndef _LIGHTMAN_CONSTFLOATNODE_H
#define _LIGHTMAN_CONSTFLOATNODE_H

#include "materialnode/node.h"

namespace lightman
{
    class ConstFloatNode : public Node
    {
    public:
        ConstFloatNode() = delete;
        ConstFloatNode(const std::string& name, float v = 0);
        ~ConstFloatNode(){};

        NodeType GetType() const override {return NodeType::CONSTFLOAT_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return true;};

        void setExposeAsUniform(bool enable);
    private:
        float m_value;
        bool m_enableExpose = false;
    };
}

#endif //_LIGHTMAN_CONSTFLOATNODE_H
