#ifndef _LIGHTMAN_BANDNODE_H
#define _LIGHTMAN_BANDNODE_H

#include "materialnode/node.h"
#include "color/color.h"

namespace lightman
{
    class BandNode : public Node
    {
    public:
        BandNode() = delete;
        BandNode(const std::string& name) : Node(name){};
        ~BandNode(){};
        void SetAmount(const Node* amount);
        void AddOffset(float offset);
        void AddValue(Color value);

        NodeType GetType() const override {return NodeType::BAND_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;}; // offsets and values woudl be treated as const for now

        bool CheckInputType(backend::UniformType) const;
    private:
        const Node* m_amount;
        std::vector<float> m_offsets;
        std::vector<Color> m_values;
    };
}

#endif // _LIGHTMAN_BANDNODE_H
