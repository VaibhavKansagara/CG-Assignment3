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

    ~Texture();

    unsigned int get_id() const;

    vector<float> get_spherical() const;

    vector<float> get_cylindrical() const;

    vector<float> get_plane() const;

    void set_spherical(const vector<float>& sph);

    void set_cylindrical(const vector<float>& cyl);

    void set_plane(const vector<float>& pl);

    void bind_texture(unsigned int id);

    void load_create_texture(string filename);

private:
    unsigned int id;
    vector<float> spherical,cylindrical,plane;
};

#endif