#ifndef SCAN_LINE_ALGO_H
#define SCAN_LINE_ALGO_H

#include "Component.h"

#include <vector>
#include <list>

class Point {

    public:
    int x,y;
    Point();
    Point(int xCord, int yCord);
    ~Point();
};

class Edge {
    public:
    int ymax;
    float x;
    float dx;
    Edge* next;

    Edge();
    Edge(int ym, float xpos, float xstep, Edge* nxt);
    ~Edge();
};



class ScanLineAlgo : public Component {
    protected:
    std::vector<Point> resaults;

    std::vector<Point> points;
    Edge **edges;

    unsigned int canvasLen, pixelLen;
    bool locked;

    int bot, top;

    public:
    using ResultIter = std::vector<Point>::iterator;


    ScanLineAlgo(unsigned int cLen,unsigned int pLen);
    ~ScanLineAlgo();

    virtual void update() override;

    virtual std::string componentType() const override;

    void createEdge();

    void runScanLineAlgo();

    bool lockStat();
    void unlock();
    void clear();
    
    ResultIter getResIter();
    ResultIter end();
};

#endif