
#include "raylib.h"

#include "pthread.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>
unsigned char stop;
#define BUFFER_LEN 10
typedef struct {
  char *org;
  char *dest;

  float progress;
  float progress_folder;

} copy_args;

void copy_file(char *org, char *dest, copy_args *arg) {
  arg->progress = 0;
  int org_size;
  unsigned long read_bytes = 0;
  char *buf = malloc((sizeof(char)) * BUFFER_LEN);
  int fd_org = open(org, O_RDONLY);
  int fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0600);

  //ver o tamanho do arquivo da origem
  org_size = (int)lseek(fd_org, 0, SEEK_END);
  lseek(fd_org, 0, SEEK_SET);
  // lseek(fd_dest, 0, SEEK_SET);

  while (!stop) {
    read_bytes = read(fd_org, buf, BUFFER_LEN);

    if (read_bytes <= 0) {
      break;
    }
    // sleep(1);
    write(fd_dest, buf, read_bytes);
    arg->progress += ((float)read_bytes / org_size) * 100;
  }
  close(fd_org);
  close(fd_dest);

  if (stop) {
    arg->progress = 0;

    int status = remove(arg->dest);

    //checar o estado de deletar do arquivo
    if (status != 0) {
      printf("Unable to delete the file\n");
      perror("Following error occurred");
    }
  }

  return;
}

void *copy(void *_arg) {
  copy_args *arg = _arg;

  struct stat st;
  char *buf_org = malloc((sizeof(char)) * 25);
  char *buf_dest = malloc((sizeof(char)) * 25);

  stat(arg->org, &st);

  if (S_ISDIR(st.st_mode)) {

    DIR *dp;
    struct dirent *ep;
    dp = opendir(arg->org);

    if (stat(arg->dest, &st) == -1) {
      mkdir(arg->dest, 0700);
    }

    if (dp != NULL) {
      while ((ep = readdir(dp)) != NULL) {

        if (strcmp(".", ep->d_name) != 0 && strcmp("..", ep->d_name) != 0) {

          sprintf(buf_org, "%s/%s", arg->org, ep->d_name);
          sprintf(buf_dest, "%s/%s", arg->dest, ep->d_name);
          printf("origem: %s\n", buf_org);
          printf("destino: %s\n", buf_dest);
          copy_file(buf_org, buf_dest, arg);
        }
      }

      closedir(dp);
    } else {
      perror("Couldn't open the directory");
    }

    return 0;

  } else {
    copy_file(arg->org, arg->dest, arg);
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
  stop = 0;

  pthread_create(&t, NULL, copy, _arg);

  int screenWidth = 800;
  int screenHeight = 450;
  int boxPositionY = screenHeight / 2;
  Rectangle textBox = {40, 295, 90, 25};

  InitWindow(screenWidth, screenHeight, "uicp");
  SetTargetFPS(60);
  //--------------------------------------------------------------------------------------
  // Main game loop
  char buf[10];
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {

    if (CheckCollisionPointRec(GetMousePosition(), textBox) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      printf("CANCELAR!\n");
      stop = 1;
    }

    ClearBackground(RAYWHITE);
    BeginDrawing();
    DrawText("Thread Lab - David Fogelman", 40, 20, 20, LIGHTGRAY);
    sprintf(buf, "%.02f%%", _arg->progress);
    DrawText(buf, 40, boxPositionY - 70, 20, GRAY);
    DrawRectangle(40, boxPositionY - 50, (int)_arg->progress * 7.2, 20, MAROON);

    sprintf(buf, "%.02f%%", _arg->progress);
    DrawText(buf, 40, boxPositionY - 20, 20, GRAY);
    DrawRectangle(40, boxPositionY, (int)_arg->progress * 7.2, 20, MAROON);

    DrawRectangleRec(textBox, ORANGE);
    DrawText("CANCELAR!", textBox.x + 5, textBox.y + 5, 15, BLACK);

    EndDrawing();
  }
  CloseWindow(); // Close window and OpenGL context
  pthread_kill(t, 9);
  // pthread_cancel(t);
  return 0;
}
