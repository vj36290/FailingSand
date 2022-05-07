#include "Material.hpp"

Materials::Materials() {
    materials = std::map<unsigned int, Material>();
    byName = std::map<std::string, Material*>();
    AddMaterial({
        "none",                 // it's basically air
        { 15, 15, 15, 0 },         // transparent black
        1, 1,                   // 1x1 texture
        maxId++,                // id increment   
        1.225f,                 // density in kg/m^3
        State::GAS
        });

    AddMaterial({
        "stone",
        { 92, 92, 92, 255 },
        1, 1,
        maxId++,
        2.780f,
        State::SOLID_STATIC
        });

    AddMaterial({
    "water",
    { 12, 64, 242, 255 },
    1, 1,
    maxId++,
    1.001f,
    State::LIQUID
        });

    AddMaterial({
    "sand",
    { 194, 178, 128, 255 },
    1, 1,
    maxId++,
    1.225f,
    State::SOLID
        });
}

const Material& Materials::AddMaterial(const Material& material) {
    materials.insert(std::make_pair(material.id, material));
    byName.insert(std::make_pair(material.name, &materials[material.id]));
    return material;
}

const Material* Materials::Get(unsigned int id) {
    return &materials[id];
}

const Material& Materials::Get(std::string name) {
    return *byName[name];
}

const int Materials::NumMaterials() const {
    return maxId;
}