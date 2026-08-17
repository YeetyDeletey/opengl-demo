#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>


            //variables
//timing variables
float deltatime = 0.0f;
float lastframe = 0.0f;

//camera class initialization
//relevant as a public variable so everything can see it
Camera c(0.0f,1.0f,3.0f,0.0f,0.0f,-1.0f,-90.0f,0.0f,45.0f, 0.1);


            //functions
//handle mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    c.mouse(xpos, ypos); //currently all mouse movement is thrown 
}               //straight to the camera

//change size of screen as required
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


int main()
{           //glfw/glad stuff
    #pragma region glfw/glad
    //initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Name goes here", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;}
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
#pragma endregion

    
    //mouse stuff
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//trapped and invisible
    glfwSetCursorPosCallback(window, mouse_callback);


    //set up shader
    Shader cubeshad("cubevshad.fs", "cubefshad.fs");
    cubeshad.use();
    Shader lightshad("cubevshad.fs", "lightfshad.fs");

    #pragma region vertices
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };


    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 lightpos(1.0f, 1.0f, 0.0f);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
#pragma endregion

    #pragma region properties
    //set up material properties
    cubeshad.setFloat("ambient", 0.1f);
    cubeshad.setFloat("diffuse", 1.0f);
    cubeshad.setFloat("specular", 2.0f);
    cubeshad.setFloat("shininess", 16);
    //dirlight properties
    cubeshad.setVec3("dirlight.direction", glm::vec3(-0.3f, -1.0f, -0.3f));
    cubeshad.setVec3("dirlight.color", glm::vec3(1.0f, 1.0f, 1.0f));
    cubeshad.setFloat("dirlight.str", 0.2f);
    //pointlight properties
    cubeshad.setVec3("pointlights[0].position", pointLightPositions[0]);
    cubeshad.setVec3("pointlights[0].color", glm::vec3(0.0f, 0.0f, 1.0f));
    cubeshad.setFloat("pointlights[0].linear", 0.09f);
    cubeshad.setFloat("pointlights[0].quadratic", 0.032f);
    cubeshad.setFloat("pointlights[0].str", 1.0f);

    cubeshad.setVec3("pointlights[1].position", pointLightPositions[1]);
    cubeshad.setVec3("pointlights[1].color", glm::vec3(1.0f, 0.0f, 0.0f));
    cubeshad.setFloat("pointlights[1].linear", 0.09f);
    cubeshad.setFloat("pointlights[1].quadratic", 0.032f);
    cubeshad.setFloat("pointlights[1].str", 1.0f);

    cubeshad.setVec3("pointlights[2].position", pointLightPositions[2]);
    cubeshad.setVec3("pointlights[2].color", glm::vec3(0.0f, 1.0f, 0.0f));
    cubeshad.setFloat("pointlights[2].linear", 0.09f);
    cubeshad.setFloat("pointlights[2].quadratic", 0.032f);
    cubeshad.setFloat("pointlights[2].str", 1.0f);

    cubeshad.setVec3("pointlights[3].position", pointLightPositions[3]);
    cubeshad.setVec3("pointlights[3].color", glm::vec3(1.0f, 1.0f, 1.0f));
    cubeshad.setFloat("pointlights[3].linear", 0.09f);
    cubeshad.setFloat("pointlights[3].quadratic", 0.032f);
    cubeshad.setFloat("pointlights[3].str", 1.0f);
    //spotlight properties
    cubeshad.setVec3("spotlight.color", glm::vec3(1.0f, 1.0f, 1.0f));
    cubeshad.setFloat("spotlight.linear", 0.022f);
    cubeshad.setFloat("spotlight.quadratic", 0.0019f);
    cubeshad.setFloat("spotlight.inner", glm::cos(glm::radians(5.0f)));
    cubeshad.setFloat("spotlight.outer", glm::cos(glm::radians(20.0f)));
    cubeshad.setFloat("spotlight.str", 3.0f);
#pragma endregion

    //create VBO/VAO for the cube and set them up
    unsigned int VBO, cubeVAO;   //make something to put data in
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //connect storage to array_buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //connect data to array_buffer

    //set up attributes and strides
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //VAO for the light source (VBO still holds cube data)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    //wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);


    #pragma region texture stuff
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;}
    stbi_image_free(data);
    //texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); //texture wrapping stays the same as last time
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture2" << std::endl;}
    stbi_image_free(data);
    //set uniforms in shader
    cubeshad.setInt("atexture", 0);
    cubeshad.setInt("aspectexture", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
#pragma endregion (sets diffuse and spec maps here)


    //render loop
    while (!glfwWindowShouldClose(window)) {
        //timing check logic
        float currentframe = static_cast<float>(glfwGetTime());
        deltatime = currentframe - lastframe;
        lastframe = currentframe;

        //input
        c.input(window, deltatime); //camera input (moevement)

        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int q = static_cast<int>(0.5f);


        //projection stuff
        glm::mat4 view;                             //view matrix (camera)
        view = glm::lookAt(c.campos, c.campos + c.camfront, c.camup);
        glm::mat4 projection = glm::mat4(1.0f);     //projection matrix (math to make it look 3d)
        projection = glm::perspective(glm::radians(c.fov), 800.0f / 600.0f, 0.3f, 100.0f);
        
        cubeshad.use();
        cubeshad.setVec3("viewpos", c.campos);
        cubeshad.setMat4("view", view);
        cubeshad.setMat4("projection", projection);

        cubeshad.setVec3("spotlight.position", c.campos);
        cubeshad.setVec3("spotlight.direction", c.camfront);


        glBindVertexArray(cubeVAO);
        //cubes
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);      //model matrix (moving it within the world)
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeshad.setMat4("model", model);
            glm::mat4 rot = glm::mat4(1.0f);    //rotatoin tracking
            rot = glm::rotate(rot, glm::radians(angle) + (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeshad.setMat4("rot", rot);

            cubeshad.use();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }   

        //lights
        lightshad.use();
        glBindVertexArray(lightVAO);
        lightshad.setMat4("view", view);
        lightshad.setMat4("projection", projection);
        for (int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2));
            lightshad.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
