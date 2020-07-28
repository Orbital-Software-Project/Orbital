#include "SceneRenderer.h"

#include <cstdlib>
#include <cstdio>

namespace Orb {

SceneRenderer::SceneRenderer()  {
    // Generate buffers and shaders for the viewport texture
    glGenFramebuffers(1, &this->fbo);
    glGenTextures(1, &this->mapTexture);
    glGenRenderbuffers(1, &this->rbo);
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    this->setFBOSize(800, 600);
}

SceneRenderer::~SceneRenderer() {}

GLuint SceneRenderer::Render(int width, int height) {

    // Update framebuffer size when size has changed
    if(this->width != width || this->height != height) {
        this->setFBOSize(width, height);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    {
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);

        glEnable(GL_DEPTH_TEST);

        // TODO:
        // - Render from active camera perspective
        // - Render all meshes
        // - Only one Shader instance can be used


        for(std::shared_ptr<Mesh> mesh : this->meshes) {
            mesh->Draw();
        }

        for(std::shared_ptr<Camera> camera : this->cameras) {
            camera->Draw();
        }

    }

    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    return this->mapTexture;
}

void SceneRenderer::AddMesh(std::shared_ptr<Mesh> mesh) {
    this->meshes.push_back(mesh);
}

void SceneRenderer::AddCamera(std::shared_ptr<Camera> camera) {
    this->cameras.push_back(camera);
}

void SceneRenderer::setFBOSize(int width, int height) {

    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    glBindTexture(GL_TEXTURE_2D, this->mapTexture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width, height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mapTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);


    GLenum status;
    status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE:
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        /* choose different formats */
        break;

    default:
        /* programming error; will fail on all hardware */
        fputs("Framebuffer Error\n", stderr);
        exit(-1);
    }
}



}