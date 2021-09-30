#include "InputHandler.h"

GLFWwindow* InputHandler::bandedWindow = nullptr;

double InputHandler::mousePosX = 0.0;
double InputHandler::mousePosY = 0.0;
bool InputHandler::lClick = false;
bool InputHandler::rClick = false;
bool InputHandler::mClick = false;
bool InputHandler::lRelease = false;
bool InputHandler::rRelease = false;
bool InputHandler::mRelease = false;
bool InputHandler::lStatus = 0;
bool InputHandler::rStatus = 0;
bool InputHandler::mStatus = 0;
double InputHandler::scrollOffsetX = 0.0;
double InputHandler::scrollOffsetY = 0.0;

void InputHandler::cursorPositionCallback(GLFWwindow* window, double x, double y) {
    mousePosX = x;
    mousePosY = y;
}

void InputHandler::mouseDownCallBack(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
                lClick = true;
                lStatus = 1;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
                mClick = true;
                mStatus = 1;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				rClick = true;
                rStatus = 1;
                break;
			default:
				return;
			}
    }

    if(action == GLFW_RELEASE) {
        switch(button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
                lRelease = true;
                lStatus = 0;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
                mRelease = true;
                mStatus = 0;
                break;
			case GLFW_MOUSE_BUTTON_RIGHT:
                rRelease = true;
                rStatus = 0;
				break;
			default:
				return;
			}
    }
}

void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollOffsetX = xoffset;
    scrollOffsetY = yoffset;
}

void InputHandler::init(GLFWwindow* window) {
    bandedWindow = window;
    glfwSetCursorPosCallback(bandedWindow,cursorPositionCallback);
    glfwSetMouseButtonCallback(bandedWindow,mouseDownCallBack);
    glfwSetScrollCallback(window,scrollCallback);
}

bool InputHandler::getMouseDownL(){
    return lClick;
}

bool InputHandler::getMouseDownR(){
    return rClick;
}

bool InputHandler::getMouseDownM(){
    return mClick;
}

bool InputHandler::getMouseUpL(){
    return lRelease;
}

bool InputHandler::getMouseUpR(){
    return rRelease;
}

bool InputHandler::getMouseUpM(){
    return mRelease;
}

bool InputHandler::getMouseL(){
    return lStatus;
}

bool InputHandler::getMouseR(){
    return rStatus;
}

bool InputHandler::getMouseM(){
    return mStatus;
}

float InputHandler::getMouseX() {
    return (float)mousePosX;
}

float InputHandler::getMouseY() {
    return (float)mousePosY;
}


bool InputHandler::getKeyDown(int key) {
    if(glfwGetKey(bandedWindow,key) == GLFW_PRESS) {
        return true;
    }

    return false;
}

void InputHandler::clearStatus() {
    lClick = false;
    rClick = false;
    mClick = false;

    lRelease = false;
    rRelease = false;
    mRelease = false;

    scrollOffsetX = 0.0;
    scrollOffsetY = 0.0;
}

float InputHandler::getScrollOffsetX() {
    return scrollOffsetX;
}

float InputHandler::getScrollOffsetY() {
    return scrollOffsetY;
}