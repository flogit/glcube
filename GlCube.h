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

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class GlCube
{
    public: // methods

        GlCube();
        ~GlCube();

        ///@brief Initialize the class. Must be called before all other methods
        ///@param in_title Title of the SDL window
        ///@param in_window_width   Width of the SDL window
        ///@param in_window_height  Height of the SDL window
        ///@param in_texture_width  Width  of the textures (see get_textures). Must be power of 2.
        ///@param in_texture_height Height of the textures (see get_textures). Must be power of 2.
        ///@return If the GlCube is correctly initialized (required to continue the workflow)
        bool init(const std::string& in_window_title,
                  unsigned int in_window_width,
                  unsigned int in_window_height,
                  unsigned int in_texture_width,
                  unsigned int in_texture_height);

        ///@brief Get a pointer to the 6 textures of the cube (Format RGBA8 : 1 char for Red, 1 for Green, 1 for Blue and 1 for alpha)
        ///@details
        /// For example, if you want to set the green of the pixel (2, 3) of the fifth textures :
        /// unsigned char** ppt = glCube.get_textures();
        /// ppt[5][(3 * width + 2) * 4 + 1] = ...;
        /// You must multiply by 4 to manage RGBA composants
        unsigned char** get_textures();

        void next(double in_scale = 1.0, double in_oxz_angle = 0.0, double in_oyz_angle = 0.0);

        void destroy();

    protected: // methods

        bool init_sdl(const std::string& in_title, unsigned int in_width, unsigned int in_height);
        bool init_gl() const;
        void set_viewport(unsigned int in_width, unsigned int in_height);

    protected: // members

        bool m_is_initialized;

        Uint32 m_last_ticks;

        SDL_Window*   m_p_window;
        SDL_Renderer* m_p_renderer;

        unsigned char* m_p_user_texture[6];
        unsigned int m_texture_width;
        unsigned int m_texture_height;

        GLfloat m_ratio;

        double m_rotation_x;
        double m_rotation_y;
};
