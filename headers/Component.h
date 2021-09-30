#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class RenderableObject;

class Component {
    
    protected:
    RenderableObject* attachedObject;
    bool activeStatis;

    public:
    Component();
    virtual ~Component();

    void setParent(RenderableObject* obj);

    virtual void onStart();
    virtual void onActivate();
    virtual void onDeactivate();

    void Activate();
    void Deactivate();

    bool isActive() const;

    void Update();

    virtual std::string ComponentType() const = 0;
};

#endif