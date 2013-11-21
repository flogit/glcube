GlCube
======

For the moment, GlCube is a smart implementation of a textured cube with OpenGL and the last version of the library SDL2. In the future, GlCube will be an OpenGL demonstration for a bigger project.

Using GlCube
------------

1. Clone or download a copy of the GlCube source code.
2. Set correctly your path / LD_LIBRARY_PATH for the OpenGL and SDL2 dependencies
3. Compile with make
4. Launch app with ./glcube
5. Move the cube with your mouse and zoom in/out with the mousewheel.

ToDo
----

GlCube use glVertexPointer and glTexCoordPointer functions. We should use VBO if available on the system.

GlCube screenshots
------------------

![GlCube ScreenShot 1](http://glcube.y3h.net/glcube1.png)
![GlCube ScreenShot 1](http://glcube.y3h.net/glcube2.png)
