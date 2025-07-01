#pragma once
#include "SceneObject.h"
#include "Light.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_map>


class SceneManager
{
private:
	static SceneManager* sm;
    SceneManager();
    

public:

    struct AmbientLight {
        Vector3 color;
        float ratio;
    } ambientLight;


    struct ScreenSize {
        int width;
        int height;
    };

    struct Color {
        float r, g, b;
    };

    struct Control {
        std::string key;
        std::string action;
    };

    struct CameraData {
        std::string type;
        float position[3];
        float target[3];
        float up[3];
        float translationSpeed;
        float rotationSpeed;
        float fov;
        float nearPlane;
        float farPlane;
    };

    
    
    struct DebugColor {
        Color OXColor;
        Color OYColor;
        Color OZColor;
    };
    


    std::string gameName;
    ScreenSize screenSize;
    Color backgroundColor;

    std::unordered_map<int, std::string> keyMappings;
    std::map<int, CameraData> cameras;
    int activeCamera;
    Camera camera;

    std::map<int, SceneObject*> objects;

    DebugColor objectAxesColors;
    DebugColor camAxesColors;

    float totalTime{ 0 };

	void InitWindow(const char*& xmlFilePath);
	void Init(const char*& xmlFilePath);
	static SceneManager* getInstance();
	void Draw();
	void Update(float deltaTime);

    void setUpCamera();
    void handleKeyInput(unsigned char key, bool isPressed);
    void handleMouseInput(MouseNameSpace::MouseButtons btn, MouseNameSpace::MouseEvents event, int x, int y);

    CameraData getActiveCamera()
    {
        return cameras[activeCamera];
    }

    float radians(float degree)
    {
        return (degree * (3.14159 / 180));
    };

    //Fog
    Vector3 fogColor;
    float smallRadius;
    float hugeRadius;

    std::map<int, Light*> lights;

	~SceneManager();

};


