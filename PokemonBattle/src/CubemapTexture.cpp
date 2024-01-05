#include "CubemapTexture.h"

//------------------------
// Crea el cubemap
//------------------------
void CubemapTexture::initCubemap(const std::vector<const char*>& textureFiles) {
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    unsigned int w, h;
    unsigned char* pixels = loadCubemap(textureFiles, w, h);

    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels + (w * h * 4 * i));
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    float aniso;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
}

unsigned char* CubemapTexture::loadCubemap(const std::vector<const char*>& textureFiles, unsigned int& w, unsigned int& h) {
    unsigned int numTextures = textureFiles.size();
    unsigned char* pixels = nullptr;

    for (int i = 0; i < numTextures; ++i) {
        unsigned int textureW, textureH;
        unsigned char* texturePixels = loadTexture(textureFiles[i], textureW, textureH);

        if (i == 0) {
            w = textureW;
            h = textureH;
            pixels = new unsigned char[numTextures * w * h * 4];
        }

        if (textureW != w || textureH != h) {
            // Manejar el caso en que las dimensiones de las texturas no coinciden
            // Puedes agregar lógica para redimensionar o lanzar un error según tus necesidades
            delete[] pixels;
            pixels = nullptr;
            break;
        }

        std::copy(texturePixels, texturePixels + (w * h * 4), pixels + (w * h * 4 * i));

        delete[] texturePixels;
    }

    return pixels;
}

//--------------------------------------------------
// Carga una textura mediante la librería Freeimage
//--------------------------------------------------
unsigned char* CubemapTexture::loadTexture(const char* textureFile, unsigned int& w, unsigned int& h) {
    FreeImage_Initialise(TRUE);

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(textureFile, 0);
    if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(textureFile);
    if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format)) {
        std::cout << "Formato de la textura " << textureFile << " no está soportado." << std::endl;
        std::cin.get();
        exit(1);
    }
    FIBITMAP* texture = FreeImage_Load(format, textureFile);
    if (texture == NULL) {
        std::cout << "El fichero " << textureFile << " no se puede abrir." << std::endl;
        std::cin.get();
        exit(1);
    }
    FIBITMAP* temp = texture;
    texture = FreeImage_ConvertTo32Bits(texture);
    FreeImage_Unload(temp);

    w = FreeImage_GetWidth(texture);
    h = FreeImage_GetHeight(texture);
    unsigned char* pixelsBGRA = (unsigned char*)FreeImage_GetBits(texture);
    unsigned char* pixelsRGBA = new unsigned char[4 * w * h];
    for (int j = 0; j < w * h; j++) {
        pixelsRGBA[j * 4 + 0] = pixelsBGRA[j * 4 + 2];
        pixelsRGBA[j * 4 + 1] = pixelsBGRA[j * 4 + 1];
        pixelsRGBA[j * 4 + 2] = pixelsBGRA[j * 4 + 0];
        pixelsRGBA[j * 4 + 3] = pixelsBGRA[j * 4 + 3];
    }

    FreeImage_Unload(texture);
    FreeImage_DeInitialise();

    return pixelsRGBA;
}

//-----------------------------------------
// Devuelve el identificador del cubemap
//-----------------------------------------
unsigned int CubemapTexture::getCubemapTexture() {
    return cubemapTexture;
}

//-----------------------
// Destructor de la clase
//-----------------------
CubemapTexture::~CubemapTexture() {
    glDeleteTextures(1, &cubemapTexture);
}

// Método para enlazar el cubemap
void CubemapTexture::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

// Método para desenlazar el cubemap
void CubemapTexture::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
