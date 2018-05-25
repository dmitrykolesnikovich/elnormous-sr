//
//  SoftwareRenderer
//

#pragma once

#include <memory>
#include "Color.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace sr
{
    class Material final
    {
    public:
        Material()
        {
            std::fill(std::begin(textures), std::end(textures), nullptr);
        }

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        Material(Material&&) = delete;
        Material& operator=(Material&&) = delete;

        //BlendState* blendState = nullptr;
        Shader* shader = nullptr;
        Texture* textures[2];
        //CullMode cullMode = CullMode::BACK;
        Color diffuseColor = Color::WHITE;
        float opacity = 1.0F;
    };
}
