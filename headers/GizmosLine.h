#ifndef GIZMOSLINE_H
#define GIZMOSLINE_H

#include "Component.h"
#include "GlobalStat.h"

#include <vector>

class GizmosLine : public Component {

    unsigned int canvasSize;
    unsigned int pixelSize;
    GlobalStat* stat;
    bool last_stat;

    std::vector<float>* vertices;
    std::vector<int>* indexes;

    public:
    GizmosLine(unsigned int srcsz, unsigned int pixSize);
    ~GizmosLine();

    float centralize(float point);

    virtual void onActivate() override;
    virtual void update() override;

    virtual std::string componentType() const override;

};

#endif