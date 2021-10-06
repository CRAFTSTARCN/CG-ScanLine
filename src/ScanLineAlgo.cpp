#include "ScanLineAlgo.h"

#include "InputHandler.h"
#include "Logger.h"
#include "Exception.hpp"

#include <cmath>
#include <algorithm>

Point::Point() : x(0), y(0) {}

Point::Point(int xCord, int yCord) : x(xCord), y(yCord) {}

Point::~Point() {}

Edge::Edge() : next(nullptr) {}

Edge::Edge(int ym, float xpos, float xstep, Edge* nxt) : 
    ymax(ym), x(xpos), dx(xstep), next(nxt)
{}

Edge::~Edge() {}

ScanLineAlgo::ScanLineAlgo(unsigned int cLen,unsigned int pLen) : canvasLen(cLen), pixelLen(pLen) {
    edges = new Edge*[canvasLen / pixelLen + 1]();
    locked = false;
}

ScanLineAlgo::~ScanLineAlgo() {
    delete edges;
}

void ScanLineAlgo::update() {
    if(locked) {
        return;
    }
    
    if(InputHandler::getMouseDownL()) {
        int xPix = floor(InputHandler::getMouseX() / (float) pixelLen);
        int yPix = floor( ((float)canvasLen - InputHandler::getMouseY()) / (float)pixelLen );
        Logger::LOG.log(std::to_string(xPix) + " ," + std::to_string(yPix));
        if(points.empty() || xPix != (points.end()-1)->x || yPix != (points.end()-1)->y) {
            points.push_back(Point(xPix,yPix));
        }
        return;
    }

  
    if(InputHandler::getKeyDown(GLFW_KEY_BACKSPACE)) {
        if(!points.empty()) {
            points.pop_back();
        }
        return;
    }

    if(InputHandler::getKeyDown(GLFW_KEY_S)) {
        if(points.size() >= 3) {
            locked = true;
            runScanLineAlgo();
        }
    }
}

std::string ScanLineAlgo::componentType() const {
    return "ScanLineAlgo";
}

void ScanLineAlgo::createEdge() {
    if(points.size() <= 2) throw InputException("Point number less than three");
    for(int i=0; i<(canvasLen / pixelLen + 1); ++i) {
        edges[i] = nullptr;
    }
    bot = 2147483647; top = 0;
    if(points[0].x != (points.end()-1)->x || points[0].y != (points.end()-1)->y) {
        Logger::LOG.log(std::string("Warning the polygraph may not complete"));
    }

    for(int i=1; i<points.size(); ++i) {
        if(points[i].y == points[i-1].y) continue;
        int ymin = std::min(points[i].y,points[i-1].y);
        int ymax = std::max(points[i].y,points[i-1].y);

        if(ymin < bot) bot = ymin;
        if(ymax > top) top = ymax;

        int ex = ymin == points[i].y ? points[i].x : points[i-1].x;
        float dx = (float)(ymin == points[i].y ? 
                 (points[i-1].x - points[i].x) : 
                 (points[i].x - points[i-1].x));
        dx = dx / (float)(ymax - ymin);
        edges[ymin] = new Edge(ymax,(float)ex,dx,edges[ymin]);
    }
}

void ScanLineAlgo::runScanLineAlgo() {
    createEdge();
    Edge* AT = nullptr;
    std::vector<float> xPoints;
    for(int index = bot; index <= top; ++index) {
        xPoints.clear();
        Edge* cur = AT, *pre = nullptr;
        //scan
        while(cur) {
            pre = cur;
            xPoints.push_back(cur->x);
            cur = cur->next;
        }
        if(!pre) {
            AT = edges[index];
            cur = AT;
        } else {
            pre->next = edges[index];
            cur = pre->next;
        }

        if(!pre) {
            while (cur) {
                xPoints.push_back(cur->x);
                cur = cur->next;
            }
        }
        
        std::sort(xPoints.begin(),xPoints.end());
        for(int i=1; i<xPoints.size(); i += 2) {
            int l = int(xPoints[i-1]+0.5), r = int(xPoints[i] + 0.5);
            for(int xcoord=l; xcoord<=r; ++xcoord) {
                resaults.push_back(Point(xcoord,index));
            }
        }
        

        //update
        cur = AT, pre = nullptr;
        while(cur) {
            if(cur->ymax == index) {
                if(cur == AT) {
                    cur = cur->next;
                    delete AT;
                    AT = cur;
                } else {
                    cur = cur->next;
                    delete pre->next;
                    pre->next = cur;
                }
            } else {
                cur->x += cur->dx;
                pre = cur;
                cur = cur->next;
            }
        }
    }
    return;
}

bool ScanLineAlgo::lockStat() {
    return locked;
}

void ScanLineAlgo::unlock() {
    locked = false;
}

ScanLineAlgo::ResultIter ScanLineAlgo::getResIter() {
    return resaults.begin();
}

ScanLineAlgo::ResultIter ScanLineAlgo::end() {
    return resaults.end();
}

void ScanLineAlgo::clear() {
    points.clear();
    resaults.clear();
}