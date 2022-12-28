#include <simple2d.h>

S2D_Window *window;
double x=225, y=225, r=10;

// void update() {
//   y+=1;
//   if(r>=100 || x>575 || y>575){
//       x=225, y=225, r=10;
//   }
// }


void render() {
  S2D_DrawCircle(x, y, r, 100, 1.0, 0.2, 0.2,   0.8);
  S2D_DrawCircle(100, 225, 100,  100, 0.2, 1.0, 0.2,   1);
//   S2D_DrawCircle(575, 225, 17, 16,   1,   1,   1, 0.6);
  S2D_DrawLine(x, y, 100, 225,
              2,
              1.0, 1.0, 1.0, 1.0,
              1.0, 1.0, 1.0, 1.0,
              1.0, 1.0, 1.0, 1.0,
              1.0, 1.0, 1.0, 1.0);
}

int main() {

  S2D_Diagnostics(true);

  window = S2D_CreateWindow("Simple 2D — Test Card", 600, 500, NULL, render, S2D_RESIZABLE);

  puts("Press `R` key to rotate textures.");

  S2D_Show(window);

  S2D_FreeWindow(window);

  return 0;
}