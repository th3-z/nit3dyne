#include "texture.h"

namespace n3d {

const std::string ext = ".png";
const std::string path = "res/texture/";

Texture::Texture(const std::string &resourceName) {
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data =
            stbi_load((path + resourceName + ext).c_str(), &this->w, &this->h, &this->channels, 0);
#ifndef NDEBUG
    if (!data) {
        std::cout << "Failed to load texture: " << resourceName << std::endl;
    }
#endif

    int mode = GL_RGB;
    if (this->channels == 4)
        mode = GL_RGBA;

    glGenTextures(1, &this->handle);
    glBindTexture(GL_TEXTURE_2D, this->handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, mode, this->w, this->h, 0, mode, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->handle);
}

}