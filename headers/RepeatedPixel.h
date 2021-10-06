#ifndef REPEATED_PIXEL_H
#define REPEATED_PIXEL_H

#include "RenderableObject.h"
#include "ScanLineAlgo.h"


class RepeatedPixel :public RenderableObject {
    protected:
    ScanLineAlgo* scanLine;
    unsigned int canvasSize;
    unsigned int pixelSize;
    ScanLineAlgo::ResultIter itNow;
    int rendStat;

    public:
    RepeatedPixel(GLenum drawT, GLenum rendT, int shader, unsigned int srcsz, unsigned int pixSize);
    ~RepeatedPixel();

    virtual void Start() override;
    virtual void renderPipline(const glm::mat4& project) override;

    virtual void init() override;
};

#endif 