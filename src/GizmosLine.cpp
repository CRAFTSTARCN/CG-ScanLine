#include "GizmosLine.h"

#include "RenderableObject.h"

#include "Exception.hpp"
#include "InputHandler.h"

GizmosLine::GizmosLine(unsigned int srcsz, unsigned int pixSize) :
    canvasSize(srcsz), pixelSize(pixSize) {}

GizmosLine::~GizmosLine() {}

void GizmosLine::onActivate() {
    stat = attachedObject->getGlobStat();
    if(stat == nullptr) {
        throw Throwable("Cannot find stat of father");
    }
    vertices = &attachedObject->vertices;
    indexes = &attachedObject->indexes;
    vertices->push_back(0);
    vertices->push_back(0);
    vertices->push_back(0);
    indexes->push_back(0);
}


void GizmosLine::update() {
    if(stat->InputStat()) {
        if(last_stat == 0) {
            vertices->clear();
            indexes->clear();
            vertices->push_back(0);
            vertices->push_back(0);
            vertices->push_back(0);
            indexes->push_back(0);
        }
        glLineWidth(2);
        int size = attachedObject->vertices.size();
        vertices->at(size-3) = centralize(InputHandler::getMouseX());
        vertices->at(size-2) = centralize(canvasSize - InputHandler::getMouseY());

        if(InputHandler::getMouseDownL()) {
            vertices->push_back(vertices->at(size-3));
            vertices->push_back(vertices->at(size-2));
            vertices->push_back(0);
            indexes->push_back(indexes->size());
        }
        last_stat = 1;
        
     } else {
        if(last_stat) {
            vertices->pop_back();vertices->pop_back();vertices->pop_back();
            indexes->pop_back();
        }
        last_stat = 0;
     }
}

float GizmosLine::centralize(float point) {
    float hp = (float)pixelSize / 2;
    float pointAt = floorf(point / (float)pixelSize);
    return pointAt * (float) pixelSize + hp - (float)canvasSize / 2; 
}

std::string GizmosLine::componentType() const {
    return "GizmosLine";
}