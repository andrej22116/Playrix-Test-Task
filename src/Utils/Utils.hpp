#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <vector>
#include <array>
#include <cinttypes>
#include <type_traits>

namespace utils {

	template<typename T>
    inline bool testVertexInPolygone(const sf::Vector2<T>& vertex, const std::vector<sf::Vector2<T>>& polygon)
    {
        static_assert(std::is_fundamental<T>::value, "Invalid type of vertex. The vertex type must have a fundamental type!");

        /* Алгоритм с хабра https ://habr.com/ru/post/125356/ */
        const std::array<const std::array<uint8_t, 2>, 2> q_patt = {
            std::array<uint8_t, 2>{0,1},
            std::array<uint8_t, 2>{3,2}
        };

        sf::Vector2<T> pred_pt = polygon.back();
        pred_pt -= vertex;

        int pred_q = q_patt[pred_pt.y < 0 ? 1 : 0][pred_pt.x < 0 ? 1 : 0];

        int w = 0;

        for ( auto cur_pt : polygon ) {
            cur_pt -= vertex;

            int q = q_patt[cur_pt.y < 0 ? 1 : 0][cur_pt.x < 0 ? 1 : 0];

            switch (q - pred_q) {
            case -3: {
                ++w; break;
            }
            case 3: {
                --w; break;
            }
            case -2: {
                if (pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x) {
                    ++w;
                }
                break;
            }
            case 2: {
                if (!(pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x)) {
                    --w;
                }
                break;
            }
            }

            pred_pt = cur_pt;
            pred_q = q;
        }

        return w != 0;
    }

}

