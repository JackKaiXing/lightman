#ifndef _LIGHTMAN_CONSTFLOAT3NODE_H
#define _LIGHTMAN_CONSTFLOAT3NODE_H

#include "materialnode/node.h"

namespace lightman
{
    class ConstFloat3Node : public Node
    {
    public:
        ConstFloat3Node() = delete;
        ConstFloat3Node(const std::string& name, float x = 0, float y = 0, float z = 0);
        ~ConstFloat3Node(){};

        NodeType GetType() const override {return NodeType::CONSTFLOAT3_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return true;};

        void setExposeAsUniform(bool enable);
    private:
        float m_value[3];
        bool m_enableExpose = false;
    };
}

#endif //_LIGHTMAN_CONSTFLOAT3NODE_H
