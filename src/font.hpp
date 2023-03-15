#pragma once

#include <vector>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "texture.hpp"
#include "syui.hpp"

namespace Syui
{
    struct Glyph{
        wchar_t unicode;
        float advance;
        Vec4 planeBounds;
        Vec4 atlasBounds;
    };
    struct Font {
        std::vector<unsigned char> m_font_atlas;
        nlohmann::json m_font_data;
        std::array<Glyph, 1025> m_glyphs;

        Texture m_texture;
        
        Font();
        Font(const std::filesystem::path& atlas, const std::filesystem::path& data);
    };
}