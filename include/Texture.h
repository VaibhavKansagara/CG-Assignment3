#ifndef TEXTURE_H_
#define TEXTURE_H_

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include <iostream>

using namespace std;

class Texture{
public:
    Texture();

    Texture(string name);

    ~Texture();

    unsigned int get_id() const;

    string get_filename() const;

    void set_filename(string name);

    void bind_texture(unsigned int id) const;

    void load_create_texture() const;

private:
    unsigned int id;
    string filename;
};

#endif