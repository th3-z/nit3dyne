#include "screen.h"

Screen::Screen(std::pair<int, int> &viewPort, std::pair<int, int> &viewPortVirtual, const std::string &title)
: viewPort(viewPort)
, viewPortVirtual(viewPortVirtual) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#ifdef NDEBUG
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
#endif

    this->window = glfwCreateWindow(this->viewPort.first, this->viewPort.second, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(this->window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, this->viewPort.first, this->viewPort.second);

    glGenFramebuffers(2, this->fbo);
    glGenTextures(2, this->fboTexHandle);
    glGenRenderbuffers(2, this->rbo);

    for (size_t i = 0; i < sizeof(this->fbo) / sizeof(unsigned int); i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[i]);

        glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[i]);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     this->viewPortVirtual.first,
                     this->viewPortVirtual.second,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fboTexHandle[i], 0);

        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo[i]);
        glRenderbufferStorage(
            GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->viewPortVirtual.first, this->viewPortVirtual.second);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo[i]);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
#ifdef NDEBUG
    glEnable(GL_CULL_FACE);
#endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int quadVbo;
    glGenVertexArrays(1, &this->fboQuadVao);
    glGenBuffers(1, &quadVbo);

    glBindVertexArray(this->fboQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), &QUAD_VERTICES, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    this->copyShader = new Shader("shaders/copy.vert", "shaders/copy.frag");
    this->copyShader->use();
    this->copyShader->setUniform("tex", 0);
}

Screen::~Screen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(2, this->fbo);
    glDeleteRenderbuffers(2, this->rbo);
    glDeleteTextures(2, this->fboTexHandle);
    glDeleteVertexArrays(1, &this->fboQuadVao);

    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Screen::flip(Shader &postShader, int ditherHandle) {
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[1]);
    glClear(GL_COLOR_BUFFER_BIT);
    postShader.use();
    glBindVertexArray(this->fboQuadVao); // shared vao
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[0]); // bind intermediate tex
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->viewPort.first, this->viewPort.second);

    glClear(GL_COLOR_BUFFER_BIT);
    this->copyShader->use(); // Use copy shader

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[1]); // bind intermediate tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ditherHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(this->window);
    glfwPollEvents();

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[0]);
    glViewport(0, 0, this->viewPortVirtual.first, this->viewPortVirtual.second);
    glEnable(GL_DEPTH_TEST);
}
