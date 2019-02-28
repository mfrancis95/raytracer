#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include "renderer.h"

struct OpenGLRenderer : Renderer {

protected:

    const char *readFile(const char *file) const;
    GLuint setupShader(const GLint type, const char *file) const;

};