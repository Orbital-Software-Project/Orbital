#include "MapViewer.h"
#include "Global.h"


#include <stdint.h>
#include <iostream>

#include <GL/glew.h>

#include <string>


namespace Orb {

// https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
// https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
// https://learnopengl.com/Advanced-OpenGL/Framebuffers

MapViewer::MapViewer() {

    // Generate buffers and shaders for the viewport texture
    glGenFramebuffers(1, &this->fbo);
    glGenTextures(1, &this->mapTexture);
    glGenRenderbuffers(1, &this->rbo);
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    this->setFBOSize(800, 600);




    this->shader = std::make_unique<Shader>("Shaders/MapViewer.vs", "Shaders/MapViewer.fs");

    static std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    static std::vector<unsigned int> indices = {  // note that we start from 0!
                                                  0, 1, 3,   // first triangle
                                                  1, 2, 3    // second triangle
                                               };


    this->testMesh = std::make_unique<Mesh>(vertices, indices);


    this->initGridMesh();
}

MapViewer::~MapViewer() {}

void MapViewer::OnRender(){
    ImGui::Begin("Map viewer");

    // Get content size of the imgui window
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    // initialize with default size
    static ImVec2 vSize = ImVec2(800, 600);

    // get current size of the imgui window
    ImVec2 vNewSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

    // check if window size has changed
    if(vSize.x != vNewSize.x || vSize.y != vNewSize.y) {
        // size has changed so update new size
        vSize = vNewSize;

        // update framebuffer with new size
        this->setFBOSize(vSize.x, vSize.y);
    }

    // Draw viewport content to opengl texture
    this->drawViewportTexture(vSize.x, vSize.y);

    // Draw opengl texture (viewport) as imgui image
    ImGui::Image((void*)(intptr_t)this->mapTexture, vSize);

    ImGui::End();

    return;
}

void MapViewer::drawViewportTexture(int width, int height) {

    // Framebuffer for 3d viewport
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    {
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);

        // Use MapViewer shader
        this->shader->Use();

        // Draw square test mesh
        this->testMesh->Draw();

    }

    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void MapViewer::drawLine3D(float x0, float y0, float z0, float x1, float y1, float z1) {
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y1, z1);
}

void MapViewer::initGridMesh() {

    std::vector<float> gridVerticies;

    constexpr float interval_ratio = 0.1;
    constexpr float grid_min       = -100.0f * interval_ratio;
    constexpr float grid_max       = 100.0f  * interval_ratio;

    for (int x = -10; x <= 10; x += 1) {
        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    for (int y = -10; y <= 10; y += 1) {
        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    std::vector<unsigned int> indices;
    for(int i = 0; i < gridVerticies.size(); i++) {
        indices.push_back(i);
    }

    this->gridMesh = std::make_unique<Mesh>(gridVerticies, indices);



}

void MapViewer::drawGrid3D() {

    Eigen::Matrix4f origin;
    origin << 0, 0, 1, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1;

    glPushMatrix();
    glMultTransposeMatrixf(origin.data());

    glLineWidth(1);
    std::array<float, 3> gridColor = {{0.7f, 0.7f, 0.7f}};
    glColor3fv(gridColor.data());

    std::vector<float> gridVerticies;
    //glBegin(GL_LINES);

    glPolygonMode(GL_FRONT, GL_LINES);

    constexpr float interval_ratio = 0.1;
    constexpr float grid_min       = -100.0f * interval_ratio;
    constexpr float grid_max       = 100.0f  * interval_ratio;

    for (int x = -10; x <= 10; x += 1) {
        //this->drawLine3D(x * 10.0f * interval_ratio, grid_min, 0, x * 10.0f * interval_ratio, grid_max, 0);
        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    for (int y = -10; y <= 10; y += 1) {
        //this->drawLine3D(grid_min, y * 10.0f * interval_ratio, 0, grid_max, y * 10.0f * interval_ratio, 0);
        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    static unsigned int VBO, VAO;
    if(!VAO) {
        glGenVertexArrays(1, &VAO);
    }
    if(!VBO) {
        glGenBuffers(1, &VBO);
    }

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, gridVerticies.size() * sizeof(float), gridVerticies.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, gridVerticies.size() * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, gridVerticies.size());

    //glEnd();

    glPopMatrix();


}

void MapViewer::setFBOSize(int width, int height) {

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
