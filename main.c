
#include "raylib.h"

#include "pthread.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_LEN 10
typedef struct {
  char *org;
  char *dest;
  unsigned char stop;
  float progress;
  float progress_folder;

} copy_args;

void *copy_file(void *_arg) {
  copy_args *arg = _arg;

  int org_size;
  unsigned long read_bytes = 0;
  char *buf = malloc((sizeof(char)) * BUFFER_LEN);
  int fd_org = open(arg->org, O_RDONLY);
  int fd_dest = open(arg->dest, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  //ver o tamanho do arquivo da origem
  org_size = (int)lseek(fd_org, 0, SEEK_END);
  lseek(fd_org, 0, SEEK_SET);
  // lseek(fd_dest, 0, SEEK_SET);

  while (!arg->stop) {
    read_bytes = read(fd_org, buf, BUFFER_LEN);

    if (read_bytes <= 0) {
      break;
    }
    sleep(1);
    write(fd_dest, buf, read_bytes);
    arg->progress += ((float)read_bytes / org_size) * 100;
  }
  close(fd_org);
  close(fd_dest);

  if (arg->stop) {
    arg->progress = 0;

    int status = remove(arg->dest);

    //checar o estado de deletar do arquivo
    if (status != 0) {
      printf("Unable to delete the file\n");
      perror("Following error occurred");
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  pthread_t t;

  // Initialization
  //--------------------------------------------------------------------------------------
  if (argc < 3) {
    printf("ERROR: MISSING ARGUMENTS\n");
    exit(1);
  }

  copy_args *_arg = malloc(sizeof(copy_args));
  _arg->org = argv[1];
  _arg->dest = argv[2];
  _arg->progress = 0;
  _arg->progress_folder = 0;
  _arg->stop = 0;

  pthread_create(&t, NULL, copy_file, _arg);

  int screenWidth = 800;
  int screenHeight = 450;
  int boxPositionY = screenHeight / 2;
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
      _arg->stop = 1;
    }

    ClearBackground(RAYWHITE);
    BeginDrawing();
    DrawText("Thread Lab - David Fogelman", 40, 20, 20, LIGHTGRAY);

    sprintf(buf, "%.02f%%", _arg->progress);
    DrawText(buf, 40, boxPositionY - 20, 20, GRAY);
    DrawRectangle(40, boxPositionY, (int)_arg->progress * 7.2, 20, MAROON);

    DrawRectangleRec(textBox, ORANGE);
    DrawText("CANCELAR!", textBox.x + 5, textBox.y + 5, 15, BLACK);

    EndDrawing();
  }
  CloseWindow(); // Close window and OpenGL context

  return 0;
}
