#pragma once

#include "Animated.h"

Animated::Animated(glm::vec3 Position, glm::vec3 Rotation, bool white, VertexArray *VAO, ElementBuffer *EBO,
                   ShaderProgram *SP)
        : Drawable(VAO, EBO, SP) {
    is_animation_in_progress = false;
    this->is_white = white;
    SetPosition(Position);
    SetRotation(Rotation);
}

Animated::~Animated() {
    Drawable::~Drawable();
}

bool Animated::isAnimationInProgress() {
    return is_animation_in_progress;
}

bool Animated::isWhite() {
    return is_white;
}

void Animated::startAnimation() {
    is_animation_in_progress = true;
}

void Animated::endAnimation() {
    is_animation_in_progress = false;
}

int Animated::Move(glm::vec3 destination, int phase, float height) {
    startAnimation();
    if (phase <= 1) {
        if (MoveUp(height)) {
            return 2;
        } else {
            return 1;
        }
    } else if (phase == 2) {
        if (MoveHorizontally(destination)) {
            return 3;
        } else {
            return 2;
        }
    } else {
        if (MoveDown()) {
            endAnimation();
            return 0;
        } else {
            return 3;
        }
    }
}
