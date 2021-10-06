#include "Window.h"
#include "Logger.h"
#include "InputHandler.h"
#include "SimpleShaderCompiler.h"
#include "RepeatedPixel.h"
#include "GizmosLine.h"

#include "glm/gtc/matrix_transform.hpp"

Window::Window(unsigned int wd, unsigned int ht, const std::string& name) {
    windowWidth = wd;
    windowHeight = ht;
    window = glfwCreateWindow(wd, ht, name.c_str(), NULL, NULL);

    if(window == nullptr) {
        Logger::ERROR.log("Fail to create window");
        exit(-1);
    }

    stat = new GlobalStat();
    initGLAD(window);
    projection = glm::ortho(-450.0f,450.0f,-450.0f,450.0f,-10.0f,100.0f);
}

Window::~Window() {
    for(auto* obj : objects) {
        delete obj;
    }
    delete stat;
    glfwTerminate();
}

void Window::initWindow() {
    InputHandler::init(window);

    auto defaultSizeCallback = [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    };

    glfwSetFramebufferSizeCallback(window, defaultSizeCallback);
}

void Window::renderProcess() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1);
    for(int i=0; i<objects.size(); ++i) {
        objects[i]->renderPipline(projection);
    }
    glfwSwapBuffers(window);
}

void Window::mainLoop() {
    for(int i=0; i<objects.size(); ++i) {
        objects[i]->Start();
    }
    while(!glfwWindowShouldClose(window)) {
        renderProcess();
        if(InputHandler::getKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window,true);
        }
        InputHandler::clearStatus();
        glfwPollEvents();
    }
}

void Window::setUpRendProp() {
    int vert, frag, shaderProgram,vertTrans,dVert,dFrag,shaderProgram3,pFrag,shaderProgram4;
    try {
        vert = SimpleShaderCompiler::compile("../shaders/vert.vert",GL_VERTEX_SHADER);
        frag = SimpleShaderCompiler::compile("../shaders/frag.frag",GL_FRAGMENT_SHADER);
        vertTrans = SimpleShaderCompiler::compile("../shaders/reposVert.vert",GL_VERTEX_SHADER);
        dVert = SimpleShaderCompiler::compile("../shaders/defaultVert.vert",GL_VERTEX_SHADER);
        dFrag = SimpleShaderCompiler::compile("../shaders/defaultFrag.frag",GL_FRAGMENT_SHADER);
        pFrag = SimpleShaderCompiler::compile("../shaders/pixel.frag",GL_FRAGMENT_SHADER);

        shaderProgram = SimpleShaderCompiler::link(vert,frag);
        shaderProgram3 = SimpleShaderCompiler::link(dVert,dFrag);
        shaderProgram4 = SimpleShaderCompiler::link(vert,pFrag);

        glDeleteShader(vert);
        glDeleteShader(vertTrans);
        glDeleteShader(frag);
        glDeleteShader(dVert);
        glDeleteShader(dFrag);
        glDeleteShader(pFrag);

    }
    catch(Throwable& e) {
        Logger::ERROR.log(e.type());
        Logger::ERROR.log(e.what());
        throw e;
    }

    RenderableObject* grid = new RenderableObject(GL_STATIC_DRAW,GL_LINES,shaderProgram);
    grid->init("../mesh/GridData.mdat");
    grid->bindData();

    RenderableObject* edge = new RenderableObject(GL_STATIC_DRAW,GL_LINE_LOOP,shaderProgram);
    edge->init("../mesh/Edge.mdat");
    edge->bindData();

    RenderableObject* pixset = new RepeatedPixel(GL_STATIC_DRAW,GL_TRIANGLES,shaderProgram4,900,10);
    pixset->init();
    pixset->bindData();
    pixset->bindStat(stat);
    pixset->setComponent(new ScanLineAlgo(900,10));

    RenderableObject* gizmosLine = new RenderableObject(GL_DYNAMIC_DRAW,GL_LINE_STRIP,shaderProgram);
    gizmosLine->bindStat(stat);
    gizmosLine->setComponent(new GizmosLine(900,10));
    
    objects.push_back(grid);
    objects.push_back(edge);
    objects.push_back(pixset);
    objects.push_back(gizmosLine);
}