#ifndef _LIGHTMAN_SCALENODE_H
#define _LIGHTMAN_SCALENODE_H

#include "materialnode/node.h"

namespace lightman
{
    class ScaleNode : public Node
    {
    public:
        ScaleNode() = delete;
        ScaleNode(const std::string& name) : Node(name){};
        ~ScaleNode();

        void SetTex1(const Node * tex1);
        void SetTex2(const Node * tex2);

        NodeType GetType() const override {return NodeType::SCALE_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};
    private:
        const Node* m_tex1;
	    const Node* m_tex2;
    };
}

#endif //_LIGHTMAN_SCALENODE_H
