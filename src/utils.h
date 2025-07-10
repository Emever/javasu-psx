#ifndef UTILS_H  // Check if it's already included
#define UTILS_H  // Define the guard

#include "raylib.h"
#include "raymath.h"

typedef struct LevelObject {
    Vector3 position;
    Vector3 size;
} LevelObject;

inline bool CheckSphereCollision(Vector3 newPosition, float radius, LevelObject *level, int levelCount) {
    for (int i = 0; i < levelCount; i++) {
        LevelObject obj = level[i];

        BoundingBox box = {
            .min = {
                obj.position.x - obj.size.x / 2,
                obj.position.y - obj.size.y / 2,
                obj.position.z - obj.size.z / 2
            },
            .max = {
                obj.position.x + obj.size.x / 2,
                obj.position.y + obj.size.y / 2,
                obj.position.z + obj.size.z / 2
            }
        };

        if (CheckCollisionBoxSphere(box, newPosition, radius)) {
            return true; // Collision detected!
        }
    }

    return false; // No collision
};

#endif