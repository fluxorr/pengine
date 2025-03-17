#include <iostream>
#include <cmath>
#include "stuff/raylib.h"
#include "stuff/raymath.h"
#include "stuff/globals.h"


bool checkCollision() {
    float dx = circle_x2 - circle_x1;
    float dy = circle_y2 - circle_y1;
    float distance = sqrt(dx * dx + dy * dy);
    
    return distance <= (r1 + r2); // if distance is less (radius of one circle + radius of another) it is considered collided!
}
void resolveCollision() {
    float dx = circle_x2 - circle_x1;
    float dy = circle_y2 - circle_y1;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance == 0) return; // Prevent division by zero

    float overlap = (r1 + r2) - distance; 
    float normX = dx / distance;
    float normY = dy / distance;

    // Separate the balls slightly to avoid overlap
    circle_x1 -= normX * (overlap / 2);
    circle_y1 -= normY * (overlap / 2);
    circle_x2 += normX * (overlap / 2);
    circle_y2 += normY * (overlap / 2);

    // Velocity reflection (Elastic collision)
    float relativeVelocityX = velocity_x2 - velocity_x1;
    float relativeVelocityY = velocity_y2 - velocity_y1;
    float dotProduct = (relativeVelocityX * normX + relativeVelocityY * normY);

    // If already moving away, no need to resolve
    if (dotProduct > 0) return;

    float bounceFactor = 0.9f; // Adjust for elasticity (1.0 = fully elastic)

    float impulse = (2 * dotProduct) / 2; // Since both masses are equal, we use (m1+m2)=2*m

    velocity_x1 += bounceFactor * impulse * normX;
    velocity_y1 += bounceFactor * impulse * normY;
    velocity_x2 -= bounceFactor * impulse * normX;
    velocity_y2 -= bounceFactor * impulse * normY;
}
void resolveBoundaries() {
    // Handle circle 1 boundaries
    if (circle_x1 < r1) {
        circle_x1 = r1;
        velocity_x1 = fabs(velocity_x1) * 0.9f; // Ensure velocity is positive after bounce
    } else if (circle_x1 > GetScreenWidth() - r1) {
        circle_x1 = GetScreenWidth() - r1;
        velocity_x1 = -fabs(velocity_x1) * 0.9f; // Ensure velocity is negative after bounce
    }
    if (circle_y1 < r1) {
        circle_y1 = r1;
        velocity_y1 = fabs(velocity_y1) * 0.9f;
    } else if (circle_y1 > GetScreenHeight() - r1) {
        circle_y1 = GetScreenHeight() - r1;
        velocity_y1 = -fabs(velocity_y1) * 0.9f;
    }

    // Handle circle 2 boundaries
    if (circle_x2 < r2) {
        circle_x2 = r2;
        velocity_x2 = fabs(velocity_x2) * 0.9f;
    } else if (circle_x2 > GetScreenWidth() - r2) {
        circle_x2 = GetScreenWidth() - r2;
        velocity_x2 = -fabs(velocity_x2) * 0.9f;
    }
    if (circle_y2 < r2) {
        circle_y2 = r2;
        velocity_y2 = fabs(velocity_y2) * 0.9f;
    } else if (circle_y2 > GetScreenHeight() - r2) {
        circle_y2 = GetScreenHeight() - r2;
        velocity_y2 = -fabs(velocity_y2) * 0.9f;
    }
}


// void moveBlue(float deltaTime) {
//     if (IsKeyDown(KEY_D) && circle_x1 < GetScreenWidth() - r1) {
//         circle_x1 += speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_A) && circle_x1 > r1) {
//         circle_x1 -= speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_W) && circle_y1 > r1) {
//         circle_y1 -= speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_S) && circle_y1 < GetScreenHeight() - r1) {
//         circle_y1 += speed * deltaTime;
//     }
// }

// void moveRed(float deltaTime) {
//     if (IsKeyDown(KEY_RIGHT) && circle_x2 < GetScreenWidth() - r2) {
//         circle_x2 += speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_LEFT) && circle_x2 > r2) {
//         circle_x2 -= speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_UP) && circle_y2 > r2) {
//         circle_y2 -= speed * deltaTime;
//     }
//     if (IsKeyDown(KEY_DOWN) && circle_y2 < GetScreenHeight() - r2) {
//         circle_y2 += speed * deltaTime;
//     }
// }

void moveBlue(float deltaTime) {
    if (IsKeyDown(KEY_D)) {
        circle_x1 += speed * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        circle_x1 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_W)) {
        circle_y1 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        circle_y1 += speed * deltaTime;
    }
}

void moveRed(float deltaTime) {
    if (IsKeyDown(KEY_RIGHT)) {
        circle_x2 += speed * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT)) {
        circle_x2 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_UP)) {
        circle_y2 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN)) {
        circle_y2 += speed * deltaTime;
    }
}
int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Pengine");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); // Time since last frame

        moveBlue(deltaTime);
        moveRed(deltaTime);
        resolveBoundaries();
    
        // Apply velocity updates
        circle_x1 += velocity_x1 * deltaTime;
        circle_y1 += velocity_y1 * deltaTime;
        circle_x2 += velocity_x2 * deltaTime;
        circle_y2 += velocity_y2 * deltaTime;

        if(checkCollision()){
            resolveCollision();
        }
        
        resolveBoundaries();

        // increases frcition over time so that it does not go moving forever...
        velocity_x1 *= friction;
        velocity_y1 *= friction;
        velocity_x2 *= friction;
        velocity_y2 *= friction;

        BeginDrawing();
        ClearBackground(VERYDARKGRAY);
        DrawRing({circle_x1, circle_y1}, r1 - thickness, r1, 0, 360, 50, BLUE);
        DrawRing({circle_x2, circle_y2}, r2 - thickness, r2, 0, 360, 50, RED);

        DrawText("Physics!!!", (screenWidth / 2), 50, 25, WHITE);
        DrawFPS(screenWidth/2, 80); 
        EndDrawing();
    }

    CloseWindow();
    return 0;
}