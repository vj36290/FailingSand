#pragma once

#include "MaterialState.hpp"

#include <string>
#include <vector>
#include <map>

extern "C" {
    struct Material final {
        std::string name;
        std::vector<unsigned char> texture;
        unsigned long textureWidth, textureHeight;
        unsigned int id;
        float density;
        State state;
    };
}

class Materials final {
public:
    static Materials& Instance() {
        static Materials instance;
        return instance;
    }

    const Material* Get(unsigned int id);
    const Material& Get(std::string name);

    const Material& AddMaterial(const Material& material);

    const int NumMaterials() const;

    // remove these
    Materials(Materials const&) = delete;
    void operator=(Materials const&) = delete;
private:
    Materials();

    std::map<unsigned int, Material> materials;
    std::map<std::string, Material*> byName;

    unsigned int maxId = 0;
};