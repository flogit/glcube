// Copyright (C) 2013  Florian GRAGLIA
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <iostream>
#include <cmath>
#include <unistd.h>

#include "GlCube.h"

const unsigned int NB_TEXTURES = 6; // it's a cube !

////////////////////////////////////////////////////////////////////////////////
/// Update of the textures of the cube (called each frame)
////////////////////////////////////////////////////////////////////////////////
void update_texture(unsigned char** in_pp_texture, unsigned int in_texture_width, unsigned int in_texture_height)
{
    static unsigned int count = 0;

    for (unsigned int id_texture = 0; id_texture < NB_TEXTURES; ++id_texture)
    {
        unsigned char *p_texture = in_pp_texture[id_texture];

        for (unsigned int i = 0; i < in_texture_width; ++i)
        {
            for (unsigned int j = 0; j < in_texture_height; ++j)
            {
                const double x_diff = (double)i - (double)in_texture_width / 2.0;
                const double y_diff = (double)j - (double)in_texture_height / 2.0;
                const double distance2_to_center = x_diff * x_diff + y_diff * y_diff;
                double psyche_value = std::abs(std::cos((distance2_to_center - (id_texture + 1) * (double)count * 15.0) / 500.0));

                *(p_texture++) = (unsigned char)(50.0 + 40.0 * psyche_value * id_texture);          // R
                *(p_texture++) = (unsigned char)(50.0  * psyche_value);                             // G
                *(p_texture++) = (unsigned char)(50.0 + 40.0 * psyche_value * (5 - id_texture));    // B
                *(p_texture++) = 0x0;                                                               // A
            }
        }
    }

    ++count;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    GlCube gl_cube;
    unsigned int texture_width  = 128;
    unsigned int texture_height = 128;

    /// Init the GlCube
    gl_cube.init("Sample of GlCube :p", 800, 800, texture_width, texture_height);

    /// Get the texture of GlCube (unsigned char* [NB_TEXTURES -> 6])
    unsigned char** pp_texture = gl_cube.get_textures();

    Uint32 last_ticks = SDL_GetTicks();
    SDL_Event event;
    double oxz_angle = 0.0;
    double oyz_angle = 0.0;
    double scale = 1.0;
    bool quit = false;

    do
    {
        /// Compute the time elapsed since the last call of the next
        Uint32 current_ticks = SDL_GetTicks();
        Uint32 time_elapsed_ms = current_ticks - last_ticks;

        /// Update the textures of the cube
        update_texture(pp_texture, texture_width, texture_height);

        /// Call next method of the GlCube
        gl_cube.next(scale, oxz_angle, oyz_angle);

        /// Event handle
        while(SDL_PollEvent(&event))
        {
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_MOUSEWHEEL)
            {
                scale += event.wheel.y / 10.0;
                if (scale < 0.1) scale = 0.1;
            }
            if (event.type == SDL_MOUSEMOTION && SDL_GetMouseState(0, 0) & SDL_BUTTON(1))
            {
                oxz_angle += (double)event.motion.xrel * time_elapsed_ms / 50.0;
                oyz_angle += (double)event.motion.yrel * time_elapsed_ms / 50.0;
                if (oyz_angle < -90.0) oyz_angle = -90.0;
                if (oyz_angle >  90.0) oyz_angle =  90.0;
            }
        }

        last_ticks = current_ticks;
    }
    while(!quit);

    /// Destroy the GlCube
    gl_cube.destroy();

    return 0;
}
