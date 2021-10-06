#include "RepeatedPixel.h"

#include "Exception.hpp"
#include "InputHandler.h"

RepeatedPixel::RepeatedPixel(GLenum drawT, GLenum rendT, int shader, 
                             unsigned int srcsz, unsigned int pixSize) : 
    RenderableObject(drawT,rendT, shader), canvasSize(srcsz), pixelSize(pixSize) 
{
    rendStat = 0;
}

RepeatedPixel::~RepeatedPixel() {}

void RepeatedPixel::Start() {
    scanLine = dynamic_cast<ScanLineAlgo*>(components.find("ScanLineAlgo")->second);
    if(scanLine == nullptr) {
        throw Throwable("No scanline alogrithm finded");
    }
    if(bindedStat == nullptr) {
        throw Throwable("Cannot find stat binded");
    }
}

void RepeatedPixel::init() {
    float hp = (float)pixelSize / 2.0;
    vertices = {
        -hp,-hp,0.0f,
        -hp,hp-1,0.0f,
        hp-1,hp-1,0.0f,
        hp-1,-hp,0.0f
    };

    indexes = {
        0,1,2,
        0,2,3
    };

}

void RepeatedPixel::renderPipline(const glm::mat4& project) {
    if(bindedStat->InputStat() && InputHandler::getKeyDown(GLFW_KEY_R)) {
        rendStat = 0;
        scanLine->unlock();
        scanLine->clear();

    }
    if(rendStat == 1 && InputHandler::getKeyDown(GLFW_KEY_SPACE)) {
        itNow = scanLine->end();
    }
    if(bindedStat->InputStat() && InputHandler::getMouseDownL() && rendStat == 2) {
        rendStat = 0;
        scanLine->unlock();
        scanLine->clear();
    }
    processComponents();

    if(scanLine->lockStat() && rendStat == 0) {
        itNow = scanLine->getResIter();
        if(itNow != scanLine->end()) {
            rendStat = 1;
            bindedStat->lockInput();
        } else {
            scanLine->unlock();
        }
        
    }
    
    float hp = (float)pixelSize / 2.0;
    float hs = (float)canvasSize / 2.0;
    glm::vec3 translateOld = translate;

    if(rendStat == 0) return;
    for(auto it = scanLine->getResIter(); it != itNow; ++it) {
        translate.x += pixelSize * it->x * scale.x - (hs - hp) * scale.x;
        translate.y += pixelSize * it->y * scale.y - (hs - hp) * scale.y;
        doTransform(project);
        translate = translateOld;
        rendering();
    }
    if(itNow == scanLine->end()) {
        rendStat = 2;
        bindedStat->unlockInput();
    }else {
        ++itNow;
    }



}