//
// Created by the_z on 11/05/2021.
//

#include "screen.h"

float quadVertices[] = {
        // Pos         // TexCoord
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
};

const float SCREEN_W_VIRTUAL = 864;
const float SCREEN_H_VIRTUAL = 486;

Screen::Screen(int w, int h, float fov, const char *title):
w(w), h(h), fov(fov) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

#ifdef NDEBUG
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
#endif

    this->window = glfwCreateWindow(this->w, this->h, title, NULL, NULL);
    glfwMakeContextCurrent(this->window);

#ifdef NDEBUG
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;

    std::cout << "GL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << "GLSL Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << (char *)glGetString(GL_RENDERER) << std::endl;

    glViewport(0, 0, this->w, this->h);

    this->perspective = glm::perspective(
        glm::radians(this->fov),
        (float) this->w / this->h,
        0.1f, 100.0f
    );

    glGenFramebuffers(2, this->fbo);
    glGenTextures(2, this->fboTexHandle);
    glGenRenderbuffers(2, this->rbo);

    for (int i = 0; i < sizeof(this->fbo) / sizeof(unsigned int); i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[i]);

        glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_W_VIRTUAL, SCREEN_H_VIRTUAL, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fboTexHandle[i], 0);

        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_W_VIRTUAL, SCREEN_H_VIRTUAL);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo[i]);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int quadVbo;
    glGenVertexArrays(1, &this->fboQuadVao);
    glGenBuffers(1, &quadVbo);

    glBindVertexArray(this->fboQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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

void Screen::resize() {}

void Screen::flip(Shader &postShader, int ditherHandle) {
    /*
     * disable depth
     * bind intermediate FBO
     * clear color buffer
     * use post shader
     *
     * bind fbo tex
     * draw quad
     */
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[1]);
    glClear(GL_COLOR_BUFFER_BIT);
    postShader.use();
    glBindVertexArray(this->fboQuadVao);  // shared vao
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[0]);  // bind intermediate tex
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->w, this->h);

    glClear(GL_COLOR_BUFFER_BIT);
    this->copyShader->use();  // Use copy shader

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle[1]);  // bind intermediate tex
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ditherHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(this->window);
    glfwPollEvents();

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[0]);
    glViewport(0, 0, SCREEN_W_VIRTUAL, SCREEN_H_VIRTUAL);
    glEnable(GL_DEPTH_TEST);
}
