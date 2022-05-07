#pragma once

#include "Renderable.hpp"

class UIBrush : public Renderable {
public:
    UIBrush(FallingSandEngine* app);
    void Render();
};