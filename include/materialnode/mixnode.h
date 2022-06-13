#ifndef _LIGHTMAN_MIXNODE_H
#define _LIGHTMAN_MIXNODE_H

#include "materialnode/node.h"

namespace lightman
{
    class MixNode : public Node
    {
    public:
        MixNode() = delete;
        MixNode(const std::string& name) : Node(name){};
        ~MixNode(){};
        void SetAmount(const Node * amount);
        void SetTex1(const Node * tex1);
        void SetTex2(const Node * tex2);

        NodeType GetType() const override {return NodeType::MIX_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};
    private:
        const Node* m_amount;
	    const Node* m_tex1;
	    const Node* m_tex2;
    };
}

#endif // _LIGHTMAN_MIXNODE_H
