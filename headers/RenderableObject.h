#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "GlobalStat.h"

#include <functional>
#include <vector>
#include <map>
#include <string>

class Component;

class RenderableObject {
    
protected:
    unsigned int VAO, VBO, EBO;
    GLenum drawType, rendType;
    int shaderProgram;
    std::map<std::string, Component*> components;
    GlobalStat* bindedStat;

public:

    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    glm::mat4 transfromMatrix;

    RenderableObject(GLenum drawT, GLenum rendT, int shader);
    ~RenderableObject();

    std::vector<float> vertices;
    std::vector<int> indexes;

    /* 
        Three method to initialize object
        By using a customized callable object
        By using a refactable init function
        By using a file to initialize data
    */
    virtual void init(const std::function<void(RenderableObject*)>& initFunc);
    virtual void init();
    virtual void init(const std::string& fromFile);

    void setComponent(Component* comp);

    void bindStat(GlobalStat* stat);

    void bindData();

    virtual void processComponents();

    virtual void Start();

    virtual void doTransform(const glm::mat4& project);

    virtual void rendering();

    virtual void renderPipline(const glm::mat4& project);

    GlobalStat* getGlobStat();
};

#endif