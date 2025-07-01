#include "stdafx.h"
#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_utils.hpp"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Globals.h"
#include "SkyBox.h"
#include "Fire.h"
#include "NormalObject.h"
#include "Light.h"

template <typename T>
T parseValue(const std::string& value) {
    std::istringstream ss(value);
    T result;
    ss >> result;
    return result;
}

SceneManager::SceneManager() {};

SceneManager* SceneManager::sm = NULL;

SceneManager* SceneManager::getInstance()
{
	if (!sm)
	{
		sm = new SceneManager();
	}
	return sm;
}

void SceneManager::Init(const char*& xmlFilePath)
{
    int N = 200, D=110;
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (int row = 0; row <= N; row++) {
        for (int col = 0; col <= N; col++) {
            Vertex v;
            v.pos.x = (col - N / 2) * D;
            v.pos.y = 0.0f;
            v.pos.z = (row - N / 2) * D;
            v.color = Vector3(0.0f, 0.0f, 1.0f);
            v.uv.x = static_cast<float>(col);
            v.uv.y = static_cast<float>(row);
            vertices.push_back(v);
        }
    }

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int topLeft = row * (N + 1) + col;
            int topRight = topLeft + 1;
            int bottomLeft = (row + 1) * (N + 1) + col;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }


    rapidxml::file<> file(xmlFilePath);
    char* buffer = file.data();
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root;
    doc.parse<0>(buffer);
    root = doc.first_node("sceneManager");

    rapidxml::xml_node<>* controlsNode = root->first_node("controls");
    for (rapidxml::xml_node<>* controlNode = controlsNode->first_node("control"); controlNode; controlNode = controlNode->next_sibling("control")) {
        std::string key = controlNode->first_node("key")->value();
        std::string action = controlNode->first_node("action")->value();
        
        int keyCode;
        if (key.length() == 1) {
            keyCode = key[0];  
        }
        else {
            if (key == "VK_LEFT") keyCode = VK_LEFT;
            else if (key == "VK_RIGHT") keyCode = VK_RIGHT;
            else if (key == "VK_UP") keyCode = VK_UP;
            else if (key == "VK_DOWN") keyCode = VK_DOWN;
            else if (key == "VK_ESCAPE") keyCode = VK_ESCAPE;
            else continue; 
        }

        keyMappings[keyCode] = action;
    }

    
    rapidxml::xml_node<>* camerasNode = root->first_node("cameras");
    for (rapidxml::xml_node<>* cameraNode = camerasNode->first_node("camera"); cameraNode; cameraNode = cameraNode->next_sibling("camera")) {
        CameraData cameraData;
        int id = parseValue<int>(cameraNode->first_attribute("id")->value());
        cameraData.type = cameraNode->first_node("type")->value();

        
        rapidxml::xml_node<>* positionNode = cameraNode->first_node("position");
        cameraData.position[0] = parseValue<float>(positionNode->first_node("x")->value());
        cameraData.position[1] = parseValue<float>(positionNode->first_node("y")->value());
        cameraData.position[2] = parseValue<float>(positionNode->first_node("z")->value());

        
        rapidxml::xml_node<>* targetNode = cameraNode->first_node("target");
        cameraData.target[0] = parseValue<float>(targetNode->first_node("x")->value());
        cameraData.target[1] = parseValue<float>(targetNode->first_node("y")->value());
        cameraData.target[2] = parseValue<float>(targetNode->first_node("z")->value());

        
        rapidxml::xml_node<>* upNode = cameraNode->first_node("up");
        cameraData.up[0] = parseValue<float>(upNode->first_node("x")->value());
        cameraData.up[1] = parseValue<float>(upNode->first_node("y")->value());
        cameraData.up[2] = parseValue<float>(upNode->first_node("z")->value());

        cameraData.translationSpeed = parseValue<float>(cameraNode->first_node("translationSpeed")->value());
        cameraData.rotationSpeed = parseValue<float>(cameraNode->first_node("rotationSpeed")->value());
        cameraData.fov = radians(parseValue<float>(cameraNode->first_node("fov")->value()));
        cameraData.nearPlane = parseValue<float>(cameraNode->first_node("near")->value());
        cameraData.farPlane = parseValue<float>(cameraNode->first_node("far")->value());

        cameras.insert({ id,cameraData });
        

    }

    
    activeCamera = parseValue<int>(root->first_node("activeCamera")->value());

    // Fog
    rapidxml::xml_node<>* fogNode = root->first_node("fog");
    if (!fogNode) {
        std::cerr << "Error: No <fog> node found in XML!" << std::endl;
        return;
    }

    rapidxml::xml_node<>* fogNodeColor = fogNode->first_node("color");
    if (fogNodeColor) {
        rapidxml::xml_node<>* rNode = fogNodeColor->first_node("r");
        rapidxml::xml_node<>* gNode = fogNodeColor->first_node("g");
        rapidxml::xml_node<>* bNode = fogNodeColor->first_node("b");

        if (rNode && gNode && bNode) {
            fogColor.x = parseValue<float>(rNode->value());
            fogColor.y = parseValue<float>(gNode->value());
            fogColor.z = parseValue<float>(bNode->value());
        }
        else {
            std::cerr << "Error: Missing color components in <fog>!" << std::endl;
        }
    }
    else {
        std::cerr << "Error: No <color> node found inside <fog>!" << std::endl;
    }

    rapidxml::xml_node<>* smallRadiusNode = fogNode->first_node("smallRadius");
    if (smallRadiusNode) {
        smallRadius = parseValue<float>(smallRadiusNode->value());
    }
    else {
        std::cerr << "Error: No <smallRadius> node found in <fog>!" << std::endl;
    }

 
    rapidxml::xml_node<>* hugeRadiusNode = fogNode->first_node("hugeRadius");
    if (hugeRadiusNode) {
        hugeRadius = parseValue<float>(hugeRadiusNode->value());
    }
    else {
        std::cerr << "Error: No <hugeRadius> node found in <fog>!" << std::endl;
    }

    //Read Objects
    rapidxml::xml_node<>* objectsNode = root->first_node("objects");
    for (rapidxml::xml_node<>* objectNode = objectsNode->first_node("object"); objectNode; objectNode = objectNode->next_sibling("object")) {
        
        std::string type = objectNode->first_node("type")->value();
        
        //Fire
        if (type == "fire") {
            Fire* fire = new Fire();

            fire->model = ResourceManager::getInstance()->getModel(std::stoi(objectNode->first_node("model")->value()));
            fire->shader = ResourceManager::getInstance()->getShader(std::stoi(objectNode->first_node("shader")->value()));


            if (rapidxml::xml_node<>* nameNode = objectNode->first_node("name")) {
                fire->name = nameNode->value();
            }


            if (rapidxml::xml_node<>* colorNode = objectNode->first_node("color")) {
                fire->color.x = parseValue<float>(colorNode->first_node("r")->value());
                fire->color.y = parseValue<float>(colorNode->first_node("g")->value());
                fire->color.z = parseValue<float>(colorNode->first_node("b")->value());
            }


            rapidxml::xml_node<>* positionNode = objectNode->first_node("position");
            fire->position[0] = parseValue<float>(positionNode->first_node("x")->value());
            fire->position[1] = parseValue<float>(positionNode->first_node("y")->value());
            fire->position[2] = parseValue<float>(positionNode->first_node("z")->value());


            rapidxml::xml_node<>* rotationNode = objectNode->first_node("rotation");
            fire->rotation[0] = parseValue<float>(rotationNode->first_node("x")->value());
            fire->rotation[1] = parseValue<float>(rotationNode->first_node("y")->value());
            fire->rotation[2] = parseValue<float>(rotationNode->first_node("z")->value());


            rapidxml::xml_node<>* scaleNode = objectNode->first_node("scale");
            fire->scale[0] = parseValue<float>(scaleNode->first_node("x")->value());
            fire->scale[1] = parseValue<float>(scaleNode->first_node("y")->value());
            fire->scale[2] = parseValue<float>(scaleNode->first_node("z")->value());

            if (rapidxml::xml_node<>* texturesNode = objectNode->first_node("textures")) {
                for (rapidxml::xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                    int textureId = parseValue<int>(textureNode->first_attribute("id")->value());
                    fire->textures[textureId] = ResourceManager::getInstance()->getTexture(textureId);
                   
                }
            }

            objects.insert({ std::stoi(objectNode->first_attribute("id")->value()), fire });
        }

        //Skybox
        if (type == "skybox") {
            Skybox* skybox = new Skybox();

            skybox->model = ResourceManager::getInstance()->getModel(std::stoi(objectNode->first_node("model")->value()));
            skybox->shader = ResourceManager::getInstance()->getShader(std::stoi(objectNode->first_node("shader")->value()));
            

            Vector3 followAxes(1, 0, 1); 
            skybox->SetFollowingCamera(followAxes);
            skybox->SetCamera(&camera);

            if (rapidxml::xml_node<>* nameNode = objectNode->first_node("name")) {
                skybox->name = nameNode->value();
            }


            if (rapidxml::xml_node<>* colorNode = objectNode->first_node("color")) {
                skybox->color.x = parseValue<float>(colorNode->first_node("r")->value());
                skybox->color.y = parseValue<float>(colorNode->first_node("g")->value());
                skybox->color.z = parseValue<float>(colorNode->first_node("b")->value());
            }


            rapidxml::xml_node<>* positionNode = objectNode->first_node("position");
            skybox->position[0] = parseValue<float>(positionNode->first_node("x")->value());
            skybox->position[1] = parseValue<float>(positionNode->first_node("y")->value());
            skybox->position[2] = parseValue<float>(positionNode->first_node("z")->value());


            rapidxml::xml_node<>* rotationNode = objectNode->first_node("rotation");
            skybox->rotation[0] = parseValue<float>(rotationNode->first_node("x")->value());
            skybox->rotation[1] = parseValue<float>(rotationNode->first_node("y")->value());
            skybox->rotation[2] = parseValue<float>(rotationNode->first_node("z")->value());


            rapidxml::xml_node<>* scaleNode = objectNode->first_node("scale");
            skybox->scale[0] = parseValue<float>(scaleNode->first_node("x")->value());
            skybox->scale[1] = parseValue<float>(scaleNode->first_node("y")->value());
            skybox->scale[2] = parseValue<float>(scaleNode->first_node("z")->value());
            
            if (rapidxml::xml_node<>* texturesNode = objectNode->first_node("textures")) {
                int textureId = parseValue<int>(texturesNode->first_node("texture")->first_attribute("id")->value());
                Texture* unwrappedTexture = ResourceManager::getInstance()->getTexture(textureId);

                if (unwrappedTexture) {
                    std::cout << "S-a incarcat";
                    skybox->textures[textureId] = unwrappedTexture;
                }
                else {
                    std::cerr << "Failed to load cube map texture!" << std::endl;
                }
            }

            objects.insert({ std::stoi(objectNode->first_attribute("id")->value()), skybox });
        }
        else if(type == "normal" || type == "terrain")
        {

            NormalObject* object = new NormalObject();
            std::string idString = objectNode->first_attribute("id")->value();
            int id = std::stoi(idString);

            std::string idmodelString = objectNode->first_node("model")->value();

            if (idmodelString == "generated")
            {
                object->isGenerated = true;
                object->model = ResourceManager::getInstance()->addModel(1000,vertices,indices);
            }
            else
            {
                object->isGenerated = false;
                int idmodel = std::stoi(idmodelString);
                object->model = ResourceManager::getInstance()->getModel(idmodel);
            }

            int idshader = std::stoi(objectNode->first_node("shader")->value());
            object->shader = ResourceManager::getInstance()->getShader(idshader);
            object->type = objectNode->first_node("type")->value();

            if (rapidxml::xml_node<>* nameNode = objectNode->first_node("name")) {
                object->name = nameNode->value();
            }


            if (rapidxml::xml_node<>* colorNode = objectNode->first_node("color")) {
                object->color.x = parseValue<float>(colorNode->first_node("r")->value());
                object->color.y = parseValue<float>(colorNode->first_node("g")->value());
                object->color.z = parseValue<float>(colorNode->first_node("b")->value());
                object->hasColor = 1;
            }

          

            rapidxml::xml_node<>* positionNode = objectNode->first_node("position");
            object->position[0] = parseValue<float>(positionNode->first_node("x")->value());
            object->position[1] = parseValue<float>(positionNode->first_node("y")->value());
            object->position[2] = parseValue<float>(positionNode->first_node("z")->value());


            rapidxml::xml_node<>* rotationNode = objectNode->first_node("rotation");
            object->rotation[0] = parseValue<float>(rotationNode->first_node("x")->value());
            object->rotation[1] = parseValue<float>(rotationNode->first_node("y")->value());
            object->rotation[2] = parseValue<float>(rotationNode->first_node("z")->value());


            rapidxml::xml_node<>* scaleNode = objectNode->first_node("scale");
            object->scale[0] = parseValue<float>(scaleNode->first_node("x")->value());
            object->scale[1] = parseValue<float>(scaleNode->first_node("y")->value());
            object->scale[2] = parseValue<float>(scaleNode->first_node("z")->value());

            if (rapidxml::xml_node<>* texturesNode = objectNode->first_node("textures")) {
                for (rapidxml::xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                    int textureId = parseValue<int>(textureNode->first_attribute("id")->value());
                    object->textures[textureId] = ResourceManager::getInstance()->getTexture(textureId);
                }
            }

            if (rapidxml::xml_node<>* lightsNode = objectNode->first_node("lights")) {
                for (rapidxml::xml_node<>* lightNode = lightsNode->first_node("light"); lightNode; lightNode = lightNode->next_sibling("light")) {
                    int lightId = parseValue<int>(lightNode->value());
                    object->idLights.push_back(lightId);
                }
            }

            if (idmodelString == "generated")
            {
                objects.insert({ 1000,object });
            }
            else
            {
                objects.insert({ id,object });
            }

        }
    }

    // --------------- Parse Ambient Light ---------------
    rapidxml::xml_node<>* ambientLightNode = root->first_node("ambientalLight");
    if (ambientLightNode) {
        // Parse Color
        rapidxml::xml_node<>* colorNode = ambientLightNode->first_node("color");
        if (colorNode) {
            ambientLight.color.x = parseValue<float>(colorNode->first_node("r")->value());
            ambientLight.color.y = parseValue<float>(colorNode->first_node("g")->value());
            ambientLight.color.z = parseValue<float>(colorNode->first_node("b")->value());
        }

        // Parse Ratio
        rapidxml::xml_node<>* ratioNode = ambientLightNode->first_node("ratio");
        if (ratioNode) {
            ambientLight.ratio = parseValue<float>(ratioNode->value());
        }
    }

    // --------------- Parse Lights ---------------
    rapidxml::xml_node<>* lightsNode = root->first_node("lights");
    if (lightsNode) {
        for (rapidxml::xml_node<>* lightNode = lightsNode->first_node("light"); lightNode; lightNode = lightNode->next_sibling("light")) {
            Light *newLight = new Light();

            int id = std::stoi(lightNode->first_attribute("id")->value());

            newLight->setId(id);

            rapidxml::xml_node<>* posNode = lightNode->first_node("position");
            std::vector<float> lightPosition;
            float positionX = parseValue<float>(posNode->first_node("x")->value());
            float positionY = parseValue<float>(posNode->first_node("y")->value());
            float positionZ = parseValue<float>(posNode->first_node("z")->value());
            lightPosition.push_back(positionX);
            lightPosition.push_back(positionY);
            lightPosition.push_back(positionZ);
            newLight->setLightPos(lightPosition);

            // Parse Diffuse Color
            rapidxml::xml_node<>* diffuseNode = lightNode->first_node("diffuseColor");
            std::vector<float> diffuseColor;
            float diffuseColorR = parseValue<float>(diffuseNode->first_node("r")->value());
            float diffuseColorG = parseValue<float>(diffuseNode->first_node("g")->value());
            float diffuseColorB = parseValue<float>(diffuseNode->first_node("b")->value());
            diffuseColor.push_back(diffuseColorR);
            diffuseColor.push_back(diffuseColorG);
            diffuseColor.push_back(diffuseColorB);
            newLight->setDiffuseColor(diffuseColor);
            

            // Parse Specular Color
            rapidxml::xml_node<>* specularNode = lightNode->first_node("specularColor");
            std::vector<float> specularColor;
            float specularColorR = parseValue<float>(specularNode->first_node("r")->value());
            float specularColorG = parseValue<float>(specularNode->first_node("g")->value());
            float specularColorB = parseValue<float>(specularNode->first_node("b")->value());
            specularColor.push_back(specularColorR);
            specularColor.push_back(specularColorG);
            specularColor.push_back(specularColorB);
            newLight->setSpecularColor(specularColor);

            // Parse Type
            newLight->setType(lightNode->first_node("type")->value());

            lights.insert({id,newLight});
        }
    }
   


    /*rapidxml::xml_node<>* debugSettingsNode = root->first_node("debugSettings");

    rapidxml::xml_node<>* objectAxesNode = debugSettingsNode->first_node("objectAxes");
    objectAxesColors.OXColor.r = parseValue<float>(objectAxesNode->first_node("OXColor")->first_node("r")->value());
    objectAxesColors.OXColor.g = parseValue<float>(objectAxesNode->first_node("OXColor")->first_node("g")->value());
    objectAxesColors.OXColor.b = parseValue<float>(objectAxesNode->first_node("OXColor")->first_node("b")->value());
    objectAxesColors.OYColor.r = parseValue<float>(objectAxesNode->first_node("OYColor")->first_node("r")->value());
    objectAxesColors.OYColor.g = parseValue<float>(objectAxesNode->first_node("OYColor")->first_node("g")->value());
    objectAxesColors.OYColor.b = parseValue<float>(objectAxesNode->first_node("OYColor")->first_node("b")->value());
    objectAxesColors.OZColor.r = parseValue<float>(objectAxesNode->first_node("OZColor")->first_node("r")->value());
    objectAxesColors.OZColor.g = parseValue<float>(objectAxesNode->first_node("OZColor")->first_node("g")->value());
    objectAxesColors.OZColor.b = parseValue<float>(objectAxesNode->first_node("OZColor")->first_node("b")->value());

    
    rapidxml::xml_node<>* camAxesNode = debugSettingsNode->first_node("camAxes");
    camAxesColors.OXColor.r = parseValue<float>(camAxesNode->first_node("OXColor")->first_node("r")->value());
    camAxesColors.OXColor.g = parseValue<float>(camAxesNode->first_node("OXColor")->first_node("g")->value());
    camAxesColors.OXColor.b = parseValue<float>(camAxesNode->first_node("OXColor")->first_node("b")->value());
    camAxesColors.OYColor.r = parseValue<float>(camAxesNode->first_node("OYColor")->first_node("r")->value());
    camAxesColors.OYColor.g = parseValue<float>(camAxesNode->first_node("OYColor")->first_node("g")->value());
    camAxesColors.OYColor.b = parseValue<float>(camAxesNode->first_node("OYColor")->first_node("b")->value());
    camAxesColors.OZColor.r = parseValue<float>(camAxesNode->first_node("OZColor")->first_node("r")->value());
    camAxesColors.OZColor.g = parseValue<float>(camAxesNode->first_node("OZColor")->first_node("g")->value());
    camAxesColors.OZColor.b = parseValue<float>(camAxesNode->first_node("OZColor")->first_node("b")->value());*/
}

void SceneManager::InitWindow(const char*& xmlFilePath)
{
    rapidxml::file<> file(xmlFilePath);
    char* buffer = file.data();
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root;
    doc.parse<0>(buffer);
    root = doc.first_node("sceneManager");
    gameName = root->first_node("gameName")->value();

    rapidxml::xml_node<>* screenSizeNode = root->first_node("defaultScreenSize");
    screenSize.width = parseValue<int>(screenSizeNode->first_node("width")->value());
    screenSize.height = parseValue<int>(screenSizeNode->first_node("height")->value());


    rapidxml::xml_node<>* backgroundColorNode = root->first_node("backgroundColor");
    backgroundColor.r = parseValue<float>(backgroundColorNode->first_node("r")->value());
    backgroundColor.g = parseValue<float>(backgroundColorNode->first_node("g")->value());
    backgroundColor.b = parseValue<float>(backgroundColorNode->first_node("b")->value());
};

void SceneManager::Draw()
{   
   
    SceneObject* skybox = nullptr;

    for (auto& objPair : objects) {
        if (objPair.second->type == "skybox") {
            skybox = objPair.second;
        }
        else {
            objPair.second->setCamera(camera);
            objPair.second->Draw();
        }
    }

    if (skybox != nullptr) {
        skybox->setCamera(camera);
        skybox->Draw();
    }

 
};

void SceneManager::setUpCamera()
{
    CameraData activeCamera = getActiveCamera();

    camera.setPosition(activeCamera.position);
    camera.setTarget(activeCamera.target);
    camera.setUp(activeCamera.up);
    camera.setMoveSpeed(activeCamera.translationSpeed);
    camera.setRotateSpeed(activeCamera.rotationSpeed);
    camera.setFov(activeCamera.fov);
    camera.setNearPlane(activeCamera.nearPlane);
    camera.setFarPlane(activeCamera.farPlane);
    camera.updatePerspectiveMatrix();
    camera.updateAxes();
    camera.updateWorldView();
}


void SceneManager::handleKeyInput(unsigned char key, bool isPressed) {
    
    int keyCode = key;

    if (keyMappings.find(key) == keyMappings.end()) {
        return; 
    }

    std::string action = keyMappings[keyCode];

    if (action == "MOVE_CAMERA_POSITIVE_Z") {
        camera.moveOz(-1);
    }
    else if (action == "MOVE_CAMERA_NEGATIVE_Z") {
        camera.moveOz(1);
    }
    else if (action == "MOVE_CAMERA_POSITIVE_X") {
        camera.moveOx(1);
    }
    else if (action == "MOVE_CAMERA_NEGATIVE_X") {
        camera.moveOx(-1);
    }
    else if (action == "MOVE_CAMERA_POSITIVE_Y") {
        camera.moveOy(1);
    }
    else if (action == "MOVE_CAMERA_NEGATIVE_Y") {
        camera.moveOy(-1);
    }
    else if (action == "ROTATE_CAMERA_LEFT") {
        camera.rotateOy(1);
    }
    else if (action == "ROTATE_CAMERA_RIGHT") {
        camera.rotateOy(-1);
    }
    else if (action == "ROTATE_CAMERA_UP") {
        camera.rotateOx(1);
    }
    else if (action == "ROTATE_CAMERA_DOWN") {
        camera.rotateOx(-1);
    }
    else if (action == "ROTATE_CAMERA_AROUND_Z_P")
    {
        camera.rotateOz(1);
    }
    else if (action == "ROTATE_CAMERA_AROUND_Z_N")
    {
        camera.rotateOz(-1);
    }

}

void SceneManager::handleMouseInput(MouseNameSpace::MouseButtons btn, MouseNameSpace::MouseEvents event, int x, int y)
{
    if (btn == 0)
    {
        if (x < 465)
        {
            camera.rotateOy(1);
        }
        if (x > 487)
        {
            camera.rotateOy(-1);
        }
    }
}


void SceneManager::Update(float deltaTime) {
    
    totalTime += deltaTime;
    if (totalTime > Globals::frameTime)
    {
        totalTime = totalTime - Globals::frameTime;
        camera.setDeltaTime(Globals::frameTime);

        for (auto& objPair : objects) {
           
              
            objPair.second->Update(Globals::frameTime);
                
        }
        
    }

    
};




SceneManager::~SceneManager()
{

}