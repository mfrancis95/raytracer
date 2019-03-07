#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <vector>
#include "renderer.h"

#define LIGHT_SIZE 32
#define MATERIAL_SIZE 32
#define PRIMITIVE_SIZE 80

struct OpenGLRenderer : Renderer {

protected:

    const char *readFile(const char *file) const;
    GLuint setupShader(const GLint type, std::vector<const char *> files) const;

};