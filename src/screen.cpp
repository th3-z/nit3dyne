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

Screen::Screen(int w, int h, float fov, const char *title):
w(w), h(h), fov(fov) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    this->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            this->w,
            this->h,
            SDL_WINDOW_OPENGL
    );

    this->context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

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

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    glGenTextures(1, &this->fboTexHandle);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->w, this->h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fboTexHandle, 0);

    glGenRenderbuffers(1, &this->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->w, this->h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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
}

Screen::~Screen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &this->fbo);
    glDeleteRenderbuffers(1, &this->rbo);
    glDeleteTextures(1, &this->fboTexHandle);
    glDeleteVertexArrays(1, &this->fboQuadVao);

    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Screen::resize() {}

void Screen::flip(Shader &postShader) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    postShader.use();
    glBindVertexArray(this->fboQuadVao);
    glBindTexture(GL_TEXTURE_2D, this->fboTexHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(this->window);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glEnable(GL_DEPTH_TEST);
}
