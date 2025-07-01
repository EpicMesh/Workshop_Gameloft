#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <string>
#include <vector>

class Light
{
private:
    int id{};
    std::vector<float> diffuseColor;   // Ensure vector has 3 elements
    std::vector<float> specularColor;  // Ensure vector has 3 elements
    std::vector<float> lightPos;
    std::string type;

public:
    Light();

    Light(int id,std::vector<float>lightPos, std::vector<float> diffuseColor, std::vector<float> specularColor, std::string type);

    void setId(int id) { this->id = id; }

    void setDiffuseColor(std::vector<float> diffuseColorL) {
        this->diffuseColor.push_back(diffuseColorL[0]);
        this->diffuseColor.push_back(diffuseColorL[1]);
        this->diffuseColor.push_back(diffuseColorL[2]);
    }

    void setSpecularColor(std::vector<float> specularColorL) {
        this->specularColor.push_back(specularColorL[0]);
        this->specularColor.push_back(specularColorL[1]);
        this->specularColor.push_back(specularColorL[2]);
        
    }

    void setLightPos(std::vector<float> lightPos) {
        this->lightPos.push_back(lightPos[0]);
        this->lightPos.push_back(lightPos[1]);
        this->lightPos.push_back(lightPos[2]);

    }

    void setType(const std::string& typeL) {
        this->type = typeL;
    }

    int getId() const { return id; }
    std::vector<float> getDiffuseColor() const { return diffuseColor; }
    std::vector<float> getSpecularColor() const { return specularColor; }
    std::vector<float> getLightPos() const { return lightPos; }
    std::string getType() const { return type; }

    ~Light();
};
