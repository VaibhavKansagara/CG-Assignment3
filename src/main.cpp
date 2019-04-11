#include<bits/stdc++.h>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Parser.h"
#include "../include/Shader.h"
#include "../include/Model.h"
#include "../include/View.h"
#include "../include/Controller.h"

using namespace std;

const GLfloat screenwidth = 800.0;
const GLfloat screenheight = 800.0;
vector<Texture> texture_list;
unsigned int no_t_press = 0;
unsigned int no_m_press = 0;

int source1=1,source2=1,source3=1,source4=1;

// light souce position
glm::vec3 lightPos1(-50.0f, 50.0f, 2.0f);
glm::vec3 lightPos2(-10.0f, 50.0f, 2.0f);
glm::vec3 lightPos3(30.0f, 50.0f, 2.0f);
glm::vec3 lightPos4(70.0f, 50.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

GLFWwindow * create_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow * window = glfwCreateWindow(800,800,"LearnOpengl",NULL,NULL);
    if(window == NULL){
        std::cout<<"Error creating window";
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window); //make the current context our window.
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    return window;
}

Model addModel(string name){
    Parser parser(name);
    Model model = parser.parse_file();
    return model;
}

int main(){
    GLFWwindow * window = create_window();

    //initialise shaders.
    Shader ourshader("shaders/vs.txt","shaders/fs.txt");
    Shader lightingShader("shaders/light_shader_vs.txt", "shaders/light_shaderfs.txt");

    //Create texture objects.
    texture_list.push_back(Texture("images/wall.jpg"));
    texture_list.push_back(Texture("images/checkerboard.png"));
    texture_list.push_back(Texture("images/mars.jpg"));
    texture_list.push_back(Texture("images/selfie.jpg"));
    //

    //Create Model and add models.
    Model sphere = addModel("data/sphere.ply");
    sphere.set_texture(texture_list[0]);
    sphere.pass_info_shader();

    Model canstick = addModel("data/canstick.ply");
    canstick.set_texture(texture_list[0]);
    canstick.pass_info_shader();

    Model cow = addModel("data/cow.ply");
    cow.set_texture(texture_list[0]);
    cow.pass_info_shader();

    //

    glEnable(GL_DEPTH_TEST);

    //light part starts
    Model sphere1 = addModel("data/sphere.ply");
    sphere1.set_light_source(true);
    sphere1.set_lightpos(lightPos1);
    sphere1.set_light_no(1);
    sphere1.pass_info_shader();

    Model sphere2 = addModel("data/sphere.ply");
    sphere2.set_light_source(true);
    sphere2.set_lightpos(lightPos2);
    sphere2.set_light_no(2);
    sphere2.pass_info_shader();

    Model sphere3 = addModel("data/sphere.ply");
    sphere3.set_light_source(true);
    sphere3.set_lightpos(lightPos3);
    sphere3.set_light_no(3);
    sphere3.pass_info_shader();

    Model sphere4 = addModel("data/sphere.ply");
    sphere4.set_light_source(true);
    sphere4.set_lightpos(lightPos4);
    sphere4.set_light_no(4);
    sphere4.pass_info_shader();
    //light part ends.


    //create a view object
    View view;

    //add models to  controller object.
    Controller controller;
    controller.add(&cow);
    controller.add(&canstick);
    controller.add(&sphere);

    while(!glfwWindowShouldClose(window)){
        controller.process_input(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourshader.use();
        ourshader.setVec3("lightColor1", 1.0f, 1.0f, 1.0f);
        ourshader.setVec3("lightPos1", lightPos1.x ,lightPos1.y,lightPos1.z);
        ourshader.setInt("source1",source2);

        ourshader.setVec3("lightColor2", 0.0f, 0.0f, 1.0f);
        ourshader.setVec3("lightPos2", lightPos2.x ,lightPos2.y,lightPos2.z);
        ourshader.setInt("source2",source2);

        ourshader.setVec3("lightColor3", 0.0f, 1.0f, 0.0f);
        ourshader.setVec3("lightPos3", lightPos3.x ,lightPos3.y,lightPos3.z);
        ourshader.setInt("source3",source3);

        ourshader.setVec3("lightColor4", 1.0f, 0.0f, 0.0f);
        ourshader.setVec3("lightPos4", lightPos4.x ,lightPos4.y,lightPos4.z);
        ourshader.setInt("source4",source4);


        //display all the models.
        view.display(cow,ourshader);
        view.display(canstick,ourshader);
        view.display(sphere,ourshader);

        // use lighting shader and display the source of the light.
        lightingShader.use();
        view.display(sphere1,lightingShader);
        view.display(sphere2,lightingShader);
        view.display(sphere3,lightingShader);
        view.display(sphere4,lightingShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //keep in mind that you need to delete VAO before EBO as VAO has a pointer or 
    //something else which connects to EBO.
    glfwTerminate();
    return 0;
}