#include "../include/Texture.h"

Texture::Texture(){
    glGenTextures(1, &id);
}

unsigned int Texture::get_id() const {
    return id;
}

void Texture::bind_texture(unsigned int id) {
    glBindTexture(GL_TEXTURE_2D, id);
}

vector<float> Texture::get_spherical() const {
    return spherical;
}

vector<float> Texture::get_cylindrical() const {
    return cylindrical;
}

vector<float> Texture::get_plane() const {
    return plane;
}

void Texture::set_spherical(const vector<float>& sph) {
    spherical = sph;
}

void Texture::set_cylindrical(const vector<float>& cyl) {
    cylindrical = cyl;
}

void Texture::set_plane(const vector<float>& pl) {
    plane = pl;
}

void Texture::load_create_texture(string filename){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(),&width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture(){}
