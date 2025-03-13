#include <iostream>
#include <cmath>
#include "stuff/raylib.h"
#include "stuff/raymath.h"
#include "stuff/globals.h"



// // Global variables
// float circle_x1 = 100;
// float circle_x2 = 200;
// float circle_y1 = 100;
// float circle_y2 = 200;
// float r1 = 40;
// float r2 = 100;
// float speed = 300.0f;
// float thickness = 4.0f; 

// float velocity_x1 = 0, velocity_y1 = 0;
// float velocity_x2 = 0, velocity_y2 = 0;
// float friction = 0.98f;



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

    float overlap = (r1+r2) - distance; // if +ve they are overlapping
    
    float pushX = (dx / distance) * (overlap / 2);
    float pushY = (dy / distance) * (overlap / 2);

    // Apply push while keeping circles inside the screen bounds
    // Each circle will move half the overlap distance in opposite directions
    if (circle_x1 - pushX >= r1 && circle_x1 - pushX <= GetScreenWidth() - r1) {
        circle_x1 -= pushX;
    }
    if (circle_y1 - pushY >= r1 && circle_y1 - pushY <= GetScreenHeight() - r1) {
        circle_y1 -= pushY;
    }

    if (circle_x2 + pushX >= r2 && circle_x2 + pushX <= GetScreenWidth() - r2) {
        circle_x2 += pushX;
    }
    if (circle_y2 + pushY >= r2 && circle_y2 + pushY <= GetScreenHeight() - r2) {
        circle_y2 += pushY;
    }
}



void moveBlue(float deltaTime) {
    if (IsKeyDown(KEY_D) && circle_x1 < GetScreenWidth() - r1) {
        circle_x1 += speed * deltaTime;
    }
    if (IsKeyDown(KEY_A) && circle_x1 > r1) {
        circle_x1 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_W) && circle_y1 > r1) {
        circle_y1 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_S) && circle_y1 < GetScreenHeight() - r1) {
        circle_y1 += speed * deltaTime;
    }
}

void moveRed(float deltaTime) {
    if (IsKeyDown(KEY_RIGHT) && circle_x2 < GetScreenWidth() - r2) {
        circle_x2 += speed * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT) && circle_x2 > r2) {
        circle_x2 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_UP) && circle_y2 > r2) {
        circle_y2 -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN) && circle_y2 < GetScreenHeight() - r2) {
        circle_y2 += speed * deltaTime;
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Window");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); // Time since last frame

        moveBlue(deltaTime);
        moveRed(deltaTime);
    
        if(checkCollision()){
            resolveCollision();
        }

        BeginDrawing();
        ClearBackground(VERYDARKGRAY);
        // DrawCircleLines(static_cast<int>(circle_x1), static_cast<int>(circle_y1), r1, BLUE);
        // DrawCircleLines(static_cast<int>(circle_x2), static_cast<int>(circle_y2), r2, RED);
        DrawRing({circle_x1, circle_y1}, r1 - thickness, r1, 0, 360, 50, BLUE);
        DrawRing({circle_x2, circle_y2}, r2 - thickness, r2, 0, 360, 50, RED);

        DrawText("Physics!!!", (screenWidth / 2), 50, 25, WHITE);
        DrawFPS(screenWidth/2, 80); 
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
