#pragma once
#include "Material.hpp"
#include "World.hpp"
#include <glm/glm.hpp>

#include <iostream>

class Chunk;

class Cell final {
public:
    World* world;
    Chunk* chunk;
    const Material* material;
    int x, y;
    float preciseX, preciseY;
    float velocityX, velocityY;

    Cell(World* world, Chunk* chunk, int x, int y, unsigned int material = 0) :
        world(world), chunk(chunk), x(x), y(y) {

        preciseX = x;
        preciseY = y;

        SetMaterial(material);

        velocityX = 0.f, velocityY = 0.f;
    }

    void Update(double deltaTime) {
        // TODO: proper viscosity and not this random nonsense
        float randFallDistance = (rand() % 3) + 1;

        // TODO: separate this logic somehow, C++ isn't convenient for it lmao
        if (material->state == State::SOLID) {
            Fall(randFallDistance + 1, deltaTime);
        }

        else if (material->state == State::LIQUID) {
            // random chance to just do absolutely fuck-all
            // for now this helps simulate air resistance on liquids
            if (!(rand() % 10))
                return;
            // if no falling occurred, disperse instead
            if (!Fall(randFallDistance, deltaTime)) {
                int dispersal = (rand() % 4) + 1;
                Cell* neighbor;
                // either use the cell's velocity or a random direction
                int xDir = (int)velocityX;
                if (xDir == 0)
                    xDir = rand() % 2 ? 1 : -1;
                xDir /= abs(xDir);

                // pog
                Cell* furthest = this;
                int maxDist = 0;
                bool left = true, right = true;

                // another "do fuck-all" check - we don't ALWAYS want to disperse immediately
                // this can be replaced with proper viscosity later
                if (!(rand() % 10))
                    return;

                // now attempt to disperse
                for (int disp = 1; disp <= dispersal; disp++) {
                    for (int i = -1; i <= 1; i += 2) {
                        if ((i * xDir < 0 && left) || (i * xDir > 0 && right)) {
                            // grab neighbors on the same y level
                            neighbor = world->GetCell(x + (i * xDir * disp), y);

                            // if we can move through this neighbor and it's further than the
                            // current max dispersal
                            // yes, this will lock the cell out of moving in one direction even
                            // if it can move an equal distance in both, but that's what
                            // the random numbers are for
                            if (neighbor && CanPassThrough(neighbor) && disp > maxDist) {
                                furthest = neighbor;
                                velocityX = i * xDir;
                                maxDist = disp;
                            }
                            else {
                                // lock them out of moving in this direction
                                if (i * xDir < 0) {
                                    left = false;
                                }
                                else if (i * xDir > 0) {
                                    right = false;
                                }
                            }

                            // break processing early if it's clear that this cell isn't moving
                            if (!left && !right)
                                return;
                        }
                    }
                }

                // then swap with the furthest cell found
                if (furthest) {
                    Swap(furthest);
                }
                else {
                    velocityX = 0.f;
                }
            }
        }
    }

    bool Fall(int maxDist, double deltaTime) {
        Cell* neighbor = nullptr;
        int x = this->x, y = this->y;
        int dx = 0, dy = 0, xDir = velocityX != 0.f ? (int)velocityX : (rand() % 2 ? 1 : -1);
        // normalize this
        xDir /= abs(xDir);

        for (dy = 1; dy <= maxDist; dy++) {
            dx = 0;
            // check center, right, left in that order
            while (dx <= 1) {
                neighbor = world->GetCell(x + dx * xDir, y + dy);

                if (neighbor && CanPassThrough(neighbor)) {
                    Swap(neighbor);
                    break;
                }
                // if we've gotten this far and can't move,
                // break processing early
                // will return false if we're still on the first level (no falling occurred)
                // true otherwise (a little falling occurred)
                else if (dx == 1)
                    return dy > 1;

                if (dx == 0)
                    dx = -1;
                else
                    dx += 2;
            }
        }
        return true;
    }

    void SetMaterial(unsigned int material) {
        this->material = Materials::Instance().Get(material);
    }

    bool CanPassThrough(const Cell* other) {
        return material->state < other->material->state ||
            (material->state == other->material->state && material->density > other->material->density);
    }

    void Swap(Cell* other) {
        int id = other->material->id;
        float vX = other->velocityX, vY = other->velocityY;

        other->SetMaterial(material->id);
        other->velocityX = velocityX, other->velocityY = velocityY;

        SetMaterial(id);
        velocityX = vX, velocityY = vY;
    }
};