#ifndef CUBEMAP_TEXTURE_H
#define CUBEMAP_TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <FreeImage.h>
#include <vector>

class CubemapTexture {
public:
    void initCubemap(const std::vector<const char*>& textureFiles);
    unsigned int getCubemapTexture();
    // Método para enlazar el cubemap
    void bind() const;

    // Método para desenlazar el cubemap
    void unbind() const;
    virtual ~CubemapTexture();

private:
    unsigned int cubemapTexture;
    unsigned char* loadCubemap(const std::vector<const char*>& textureFiles, unsigned int& w, unsigned int& h);
    unsigned char* loadTexture(const char* textureFile, unsigned int& w, unsigned int& h);
};

#endif /* CUBEMAP_TEXTURE_H */
