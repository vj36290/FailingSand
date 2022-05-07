#pragma once

#include <map>

class Cell;

enum class State {
    SOLID_STATIC,
    SOLID,
    LIQUID,
    GAS
};

class MaterialState {
public:
    virtual void Step(Cell* cell, double deltaTime);
private:
    static std::map<State, MaterialState*> states;
};