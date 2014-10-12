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
#include <vector>

#ifdef __APPLE__
 #include "TargetConditionals.h"
 #ifdef TARGET_OS_MAC
  #include <SDL.h>
  #include <SDL_opengl.h>
  #include <glu.h>
 #endif
#else
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_opengl.h>
 #include <GL/glu.h>
#endif

#include "GlCube.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GlCube::GlCube() :
    m_is_initialized(false),
    m_p_window(0),
    m_p_renderer(0),
    m_texture_width(0),
    m_texture_height(0),
    m_pTexCoords(0),
    m_pVertices(0),
    m_rotation_x(0.0),
    m_rotation_y(0.0)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GlCube::~GlCube()
{
    if (m_is_initialized)
    {
        std::cerr << "Need to call destroy method at the end of the program !" << std::endl;
        destroy();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
GlCube::init_sdl(const std::string& in_title, unsigned int in_width, unsigned int in_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        std::cerr << "Error : " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_CreateWindowAndRenderer(in_width, in_height, SDL_WINDOW_OPENGL, &m_p_window, &m_p_renderer);
    SDL_SetWindowTitle(m_p_window, in_title.c_str());

    if (!m_p_window || !m_p_renderer)
    {
        std::cerr << "Error : " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_RendererInfo renderer_info;
    SDL_GetRendererInfo(m_p_renderer, &renderer_info);

    if ((renderer_info.flags & SDL_RENDERER_ACCELERATED)   == 0 ||
        (renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
    {
        std::cerr << "Error : Renderer without texture and acceleration" << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
GlCube::init_gl()
{
    glShadeModel(GL_SMOOTH);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Textture parameters
    glDisable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    init_gl_data_pointer();

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::init_gl_data_pointer()
{
    m_pTexCoords = new std::vector<float>();
    for (unsigned int i = 0; i < 3; ++i)
    {
        push(*m_pTexCoords, 0.0, 0.0); // Direct tex coord
        push(*m_pTexCoords, 1.0, 0.0);
        push(*m_pTexCoords, 0.0, 1.0);
        push(*m_pTexCoords, 1.0, 1.0);
        push(*m_pTexCoords, 0.0, 0.0); // Indirect tex coord
        push(*m_pTexCoords, 0.0, 1.0);
        push(*m_pTexCoords, 1.0, 0.0);
        push(*m_pTexCoords, 1.0, 1.0);
    }
    glTexCoordPointer(2, GL_FLOAT, 0, &m_pTexCoords->at(0));

    m_pVertices = new std::vector<float>();
    push(*m_pVertices, -1.0f, -1.0f, -1.0f); // Face 0
    push(*m_pVertices,  1.0f, -1.0f, -1.0f);
    push(*m_pVertices, -1.0f,  1.0f, -1.0f);
    push(*m_pVertices,  1.0f,  1.0f, -1.0f);
    push(*m_pVertices, -1.0f, -1.0f,  1.0f); // Face 1
    push(*m_pVertices, -1.0f,  1.0f,  1.0f);
    push(*m_pVertices,  1.0f, -1.0f,  1.0f);
    push(*m_pVertices,  1.0f,  1.0f,  1.0f);
    push(*m_pVertices, -1.0f,  1.0f, -1.0f); // Face 2
    push(*m_pVertices,  1.0f,  1.0f, -1.0f);
    push(*m_pVertices, -1.0f,  1.0f,  1.0f);
    push(*m_pVertices,  1.0f,  1.0f,  1.0f);
    push(*m_pVertices, -1.0f, -1.0f, -1.0f); // Face 3
    push(*m_pVertices, -1.0f, -1.0f,  1.0f);
    push(*m_pVertices,  1.0f, -1.0f, -1.0f);
    push(*m_pVertices,  1.0f, -1.0f,  1.0f);
    push(*m_pVertices, -1.0f, -1.0f, -1.0f); // Face 4
    push(*m_pVertices, -1.0f,  1.0f, -1.0f);
    push(*m_pVertices, -1.0f, -1.0f,  1.0f);
    push(*m_pVertices, -1.0f,  1.0f,  1.0f);
    push(*m_pVertices,  1.0f, -1.0f, -1.0f); // Face 5
    push(*m_pVertices,  1.0f, -1.0f,  1.0f);
    push(*m_pVertices,  1.0f,  1.0f, -1.0f);
    push(*m_pVertices,  1.0f,  1.0f,  1.0f);
    glVertexPointer  (3, GL_FLOAT, 0, &m_pVertices ->at(0));

    ///@todo What about normal...
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::set_viewport(unsigned int in_width, unsigned int in_height)
{
    m_ratio = (GLfloat)in_width / (GLfloat)in_height;

    glViewport(0, 0, (GLsizei)in_width, (GLsizei)in_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, m_ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
GlCube::init(const std::string& in_window_title,
                 unsigned int in_window_width,
                 unsigned int in_window_height,
                 unsigned int in_texture_width,
                 unsigned int in_texture_height)
{
    bool is_ok = init_sdl(in_window_title, in_window_width, in_window_height);

    if (!is_ok)
    {
        std::cerr << "Error when init sdl";
        return false;
    }

    is_ok = init_gl();

    if (!is_ok)
    {
        std::cerr << "Error when init gl";
        return false;
    }

    /// Init texture
    for (unsigned int i = 0; i < 6; ++i) // 6 sides of the cube
    {
        m_p_user_texture[i] = new unsigned char[in_texture_width * in_texture_height * 4]; // 4 for R G B A
    }
    m_texture_width  = in_texture_width;
    m_texture_height = in_texture_height;

    set_viewport(in_window_width, in_window_height);

    m_last_ticks = SDL_GetTicks();

    m_is_initialized = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::push(std::vector<float>& out_vector, float in_pt1, float in_pt2, float in_pt3)
{
    out_vector.push_back(in_pt1);
    out_vector.push_back(in_pt2);
    out_vector.push_back(in_pt3);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::push(std::vector<float>& out_vector, float in_pt1, float in_pt2)
{
    out_vector.push_back(in_pt1);
    out_vector.push_back(in_pt2);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
unsigned char**
GlCube::get_textures()
{
    unsigned char **pp_user_texture = 0;

    if (m_is_initialized)
    {
        pp_user_texture = m_p_user_texture;
    }
    else
    {
        std::cerr << "Try to get texture of non initialized class" << std::endl;
    }

    return pp_user_texture;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::next(double in_scale, double in_oxz_angle, double in_oyz_angle)
{
    if (m_is_initialized)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);          // Center the cube
        glRotated(in_oyz_angle, 1.0, 0.0, 0.0);   // Rotation of the cube
        glRotated(in_oxz_angle, 0.0, 1.0, 0.0);   //     ''     ''
        glScaled(in_scale, in_scale, in_scale);   // And the scale (from the mouse event)

        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        for (unsigned int i = 0; i < get_nb_faces(); ++i)
        {
            // Apply texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_texture_width, m_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_p_user_texture[i]);
            // And draw from the vertex & tex coord array initialized in GlCube::init_gl_data()
            glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);

        SDL_RenderPresent(m_p_renderer);
    }
    else
    {
        std::cerr << "Try to call the next on non initialized class" << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
GlCube::destroy()
{
    SDL_DestroyRenderer(m_p_renderer);
    SDL_DestroyWindow(m_p_window);

    SDL_Quit();

    m_is_initialized = false;
}
