#include "raylib.h"

int main() {
    InitWindow(800, 600, "Raylib on macOS");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("HOLY SHIT!", 200, 300, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}