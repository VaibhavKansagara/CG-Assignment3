#include "../include/Texture.h"

Texture::Texture(){
    glGenTextures(1, &id);
}

Texture::Texture(string name){
    filename = name;
    glGenTextures(1, &id);
}

unsigned int Texture::get_id() const {
    return id;
}

void Texture::bind_texture(unsigned int id) const {
    glBindTexture(GL_TEXTURE_2D, id);
}

string Texture::get_filename() const{
    return filename;
}

void Texture::set_filename(string name){
    filename = name;
}

void Texture::load_create_texture() const {
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
