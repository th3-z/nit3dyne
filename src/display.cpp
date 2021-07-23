#include "display.h"

float QUAD_VERTICES[] = {
        -1.0f,  1.0f,  0.0f,
        1.0f, -1.0f, -1.0f,
        0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  0.0f,  1.0f,
        1.0f,  1.0f,  1.0f
};

void Display::init() {
    viewPort = std::pair<int, int>(1920, 1200);
    viewPortVirtual = std::pair<int, int>(776, 485);
    title = "GlToy";
    shouldClose = false;

    frame = 0;
    timeLastFrame = 0.;
    timeThisFrame = 0.;
    timeDelta = 0.;
    target_frametime = 1. / 75.;

    initGlfw();
    initGl();
    initBuffers();
    initResources();
}

void Display::destroy() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(2, fbo);
    glDeleteRenderbuffers(2, rbo);
    glDeleteTextures(2, fboTexHandle);
    glDeleteVertexArrays(1, &fboQuadVao);

    delete copyShader;
    delete dither;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Display::initGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(viewPort.first, viewPort.second, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

void Display::initGl() {
    glViewport(0, 0, viewPort.first, viewPort.second);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display::initBuffers() {
    glGenFramebuffers(2, fbo);
    glGenTextures(2, fboTexHandle);
    glGenRenderbuffers(2, rbo);

    for (size_t i = 0; i < sizeof(fbo) / sizeof(unsigned int); i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);

        glBindTexture(GL_TEXTURE_2D, fboTexHandle[i]);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            viewPortVirtual.first,
            viewPortVirtual.second,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexHandle[i], 0);

        glBindRenderbuffer(GL_RENDERBUFFER, rbo[i]);
        glRenderbufferStorage(
            GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortVirtual.first, viewPortVirtual.second
        );
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[i]);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
    }

    unsigned int quadVbo;
    glGenVertexArrays(1, &fboQuadVao);
    glGenBuffers(1, &quadVbo);

    glBindVertexArray(fboQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), &QUAD_VERTICES, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    glBindVertexArray(0);
}

void Display::update() {
    timeLastFrame = timeThisFrame;
    timeThisFrame = glfwGetTime();
    while ((timeThisFrame - timeLastFrame) < target_frametime)
        timeThisFrame = glfwGetTime();

    timeDelta = timeThisFrame - timeLastFrame;
    ++frame;

    shouldClose = (bool) glfwWindowShouldClose(window);
}

void Display::flip(Shader &postShader) {
    // Bind intermediate fb for applying post effects
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[1]);
    glClear(GL_COLOR_BUFFER_BIT);

    postShader.use();
    postShader.setUniform("grainSeed", randFloat(0.f, 1.f));
    glBindVertexArray(fboQuadVao); // VAO is shared for both copies
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexHandle[0]); // bind intermediate tex
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Bind main fb
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, viewPort.first, viewPort.second);
    glClear(GL_COLOR_BUFFER_BIT);

    // copy intermediate fb to main fb and swap
    copyShader->use(); // Use copy shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexHandle[1]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dither->handle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);

    // switch back to virtual fb before next frame
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
    glViewport(0, 0, viewPortVirtual.first, viewPortVirtual.second);
    glEnable(GL_DEPTH_TEST);
}

void Display::initResources() {
    dither = new Texture("dith");

    copyShader = new Shader("shaders/copy.vert", "shaders/copy.frag");
    copyShader->use();
    copyShader->setUniform("tex", 0);
}


