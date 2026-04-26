#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "Shader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Pendulum", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    unsigned int rodVAO, rodVBO;
    glGenVertexArrays(1, &rodVAO);
    glGenBuffers(1, &rodVBO);

    glBindVertexArray(rodVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rodVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int bobVAO, bobVBO;
    glGenVertexArrays(1, &bobVAO);
    glGenBuffers(1, &bobVBO);

    glBindVertexArray(bobVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bobVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 100 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float theta = 0.8f;
    float thetaDot = 0.0f;

    float gravity = 9.81f;
    float length  = 0.5f;
    float damping = 0.03f;
    float timeScale = 1.0f;

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        double now = glfwGetTime();
        float dt = float(now - lastTime);
        lastTime = now;
        dt *= timeScale;
        if (dt > 0.05f) dt = 0.05f;

        float thetaAcc = -(gravity / length) * sinf(theta) - damping * thetaDot;
        thetaDot += thetaAcc * dt;
        theta += thetaDot * dt;

        float pivotX = 0.0f, pivotY = 0.8f;
        float endX = pivotX + length * sinf(theta);
        float endY = pivotY - length * cosf(theta);

        float rodVertices[6] = {
            pivotX, pivotY, 0.0f,
            endX,   endY,   0.0f
        };

        const int segments = 40;
        const float radius = 0.04f;
        float bobVertices[(segments + 2) * 3];

        bobVertices[0] = endX;
        bobVertices[1] = endY;
        bobVertices[2] = 0.0f;

        for (int i = 0; i <= segments; i++) {
            float a = (float)i / segments * 2.0f * 3.1415926f;
            bobVertices[(i + 1) * 3 + 0] = endX + radius * cosf(a);
            bobVertices[(i + 1) * 3 + 1] = endY + radius * sinf(a);
            bobVertices[(i + 1) * 3 + 2] = 0.0f;
        }

    
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Pendulum Controls");
        ImGui::SliderFloat("Gravity", &gravity, 0.0f, 30.0f);
        ImGui::SliderFloat("Length", &length, 0.1f, 1.0f);
        ImGui::SliderFloat("Damping", &damping, 0.0f, 1.0f);
        ImGui::SliderFloat("Time Scale", &timeScale, 0.1f, 2.0f);

        if (ImGui::Button("Reset Position")) {
            theta = 0.8f;
            thetaDot = 0.0f;
        }
        if(ImGui::Button("Reset Variables")){
            gravity = 9.81f;
            length  = 0.5f;
            damping = 0.03f;
            timeScale = 1.0f;
        }
        ImGui::End();

       
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindBuffer(GL_ARRAY_BUFFER, rodVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rodVertices), rodVertices);
        glBindVertexArray(rodVAO);
        glDrawArrays(GL_LINES, 0, 2);

        glBindBuffer(GL_ARRAY_BUFFER, bobVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bobVertices), bobVertices);
        glBindVertexArray(bobVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
