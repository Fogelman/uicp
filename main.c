
#include "raylib.h"
#include <stdio.h>

int copy_file() int main() {
  // Initialization
  //--------------------------------------------------------------------------------------
  int screenWidth = 800;
  int screenHeight = 450;
  int boxPositionY = screenHeight / 2;
  int i = 0;
  Rectangle textBox = {40, 295, 90, 25};

  // if (CheckCollisionPointRec(GetMousePosition(), textBox))
  // {

  // }
  InitWindow(screenWidth, screenHeight, "");
  SetTargetFPS(60);
  //--------------------------------------------------------------------------------------
  // Main game loop
  char buf[10];
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {

    if (CheckCollisionPointRec(GetMousePosition(), textBox) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      printf("CANCELAR!\n");
      i = 0;
    }

    ClearBackground(RAYWHITE);
    BeginDrawing();
    DrawText("Thread Lab - David Fogelman", 40, 20, 20, LIGHTGRAY);

    sprintf(buf, "%d%%", i);
    DrawText(buf, 40, boxPositionY - 20, 20, GRAY);
    DrawRectangle(40, boxPositionY, (int)i * 7.2, 20, MAROON);
    // DrawRectangle

    //----------------------------------------------------------------------------------
    if (i < 100) {
      i++;
    }
    DrawRectangleRec(textBox, ORANGE);
    DrawText("CANCELAR!", textBox.x + 5, textBox.y + 5, 15, BLACK);

    EndDrawing();
  }
  CloseWindow(); // Close window and OpenGL context

  // De-Initialization
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------

  return 0;
}
