#include "RenderableObject.h"

#include "Component.h"
#include "Exception.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

RenderableObject::RenderableObject(GLenum drawT, GLenum rendT, int shader) : 
    translate(0.0f,0.0f,0.0f), rotate(0.0f,0.0f,0.0f), scale(1.0f,1.0f,1.0f)
{
    drawType = drawT;
    rendType = rendT;
    shaderProgram = shader;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO); 
}

RenderableObject::~RenderableObject() {
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

    for(auto iter = components.begin(); iter != components.end(); ++iter) {
        delete iter->second;
    }
}

/*
WARNNING:
Asynchronous function
You cant run this function in to thread simultaneously
*/
void RenderableObject::bindData() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(),drawType);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexes.size()*sizeof(int),indexes.data(),drawType);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void RenderableObject::setComponent(Component* comp) {
    auto iter = components.find(comp->componentType());
    if( iter != components.end()) {
        Component *ori = iter->second;
        delete ori;
    }
    components.emplace(comp->componentType(),comp);
    comp->setParent(this);
    comp->onStart();
    comp->activate();
}

void RenderableObject::bindStat(GlobalStat* stat) {
    bindedStat = stat;
}

void RenderableObject::processComponents() {
    for(auto iter = components.begin(); iter != components.end(); ++iter) {
        if(iter->second->isActive()) {
            iter->second->update();
        }
    }
}

void RenderableObject::Start() {
    
}

void RenderableObject::doTransform(const glm::mat4& project) {
    transfromMatrix = glm::mat4(1.0f);

    transfromMatrix = glm::translate(transfromMatrix,translate);
    transfromMatrix = glm::scale(transfromMatrix,scale);

    if(rotate.x != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.x),glm::vec3(1.0f,0.0f,0.0f));
    if(rotate.y != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.y),glm::vec3(0.0f,1.0f,0.0f));
    if(rotate.z != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.z),glm::vec3(0.0f,0.0f,1.0f));

    transfromMatrix = project * transfromMatrix;
}

void RenderableObject::rendering() {
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"MATRIX_MVP"),1,GL_FALSE,glm::value_ptr(transfromMatrix));
    if(drawType == GL_DYNAMIC_DRAW) {
        bindData();
    }

    glBindVertexArray(VAO);
    glDrawElements(rendType,indexes.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

/*
WARNNING:
Asynchronous function
You cant run this function in to thread simultaneously
*/
void RenderableObject::renderPipline(const glm::mat4& project) {
    processComponents();
    doTransform(project);
    rendering();
}

void RenderableObject::init(const std::function<void(RenderableObject*)>& initFunc) {
    initFunc(this);
}

void RenderableObject::init() {

}

void RenderableObject::init(const std::string& fromFile) {
    FILE *file;
    file = fopen(fromFile.c_str(),"rt");
    if(file == nullptr) throw IOException("Cannot open file: " + fromFile);
    int num_of_vert_coord,num_of_index;
    fscanf(file,"%d",&num_of_vert_coord);
    float vert_coord;
    for(int i=0; i<num_of_vert_coord; ++i) {
        fscanf(file,"%f",&vert_coord);
        vertices.push_back(vert_coord);
    }
    int index;
    fscanf(file,"%d",&num_of_index);
    for(int i=0; i<num_of_index; ++i) {
        fscanf(file,"%d",&index);
        indexes.push_back(index);
    }
    fclose(file);
}

GlobalStat* RenderableObject::getGlobStat() {
    return bindedStat;
}