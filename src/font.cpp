#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iterator>

#include "font.hpp"
#include "syui.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Syui
{

    Font::Font(const std::filesystem::path &atlas, const std::filesystem::path &data_path)
    {
        // std::ifstream atlas_f = std::ifstream(atlas, std::ios::binary);
        // std::istream_iterator<char> beg(atlas_f), end;
        // std::vector<char> pixels(beg, end);
        int w = 0, h = 0, c = 0;
        stbi_set_flip_vertically_on_load(1);
        auto *image = stbi_load(atlas.string().c_str(), &w, &h, &c, 4);

        std::ifstream data_f(data_path);
        nlohmann::json data = nlohmann::json::parse(data_f);

        //this->m_font_atlas = std::vector(image);
        this->m_font_data = data;

        Vec2 atlas_size = Vec2{.x = m_font_data["atlas"]["width"], .y = m_font_data["atlas"]["height"]};
        this->m_texture = Texture(image, atlas_size);

        // auto glyphs = m_font_data["glyphs"].array();
        for (auto &glyph : m_font_data["glyphs"])
        {
            Glyph g;
            g.unicode = glyph["unicode"].get<wchar_t>();
            g.advance = glyph["advance"].get<float>() / 32;

            if (glyph["planeBounds"].is_object())
            {
                g.planeBounds.x = (glyph["planeBounds"]["left"].get<float>() + 1.0f) / 2.0f;
                g.planeBounds.y = (glyph["planeBounds"]["bottom"].get<float>() + 1.0f) / 2.0f;
                g.planeBounds.z = (glyph["planeBounds"]["right"].get<float>() + 1.0f) / 2.0f;
                g.planeBounds.w = (glyph["planeBounds"]["top"].get<float>() + 1.0f) / 2.0f;
            }
            if (glyph["atlasBounds"].is_object())
            {
                g.atlasBounds.x = glyph["atlasBounds"]["left"].get<float>() / w;
                g.atlasBounds.y = glyph["atlasBounds"]["bottom"].get<float>() / h;
                g.atlasBounds.z = glyph["atlasBounds"]["right"].get<float>() / w;
                g.atlasBounds.w = glyph["atlasBounds"]["top"].get<float>() / h;
            }

            m_glyphs[g.unicode] = g;
        }
    }

    Font::Font()
    {
    }
}