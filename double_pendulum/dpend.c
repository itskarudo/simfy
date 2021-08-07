#include <math.h>
#include <time.h>
#include <curses.h>

#define M1 5
#define R1 12

#define M2 3
#define R2 10

#define G 15
#define miu 0.2

#define AIRRES false

struct Point {
  int x;
  int y;
};

void draw_point(struct Point point, char c);
void draw_line(struct Point p1, struct Point p2, char c);

int main(void) {

  float ANGLE1 = M_PI_2 + M_PI_4;
  float ANGLE2 = M_PI_4;
  
  initscr();

  int WIDTH, HEIGHT;
  getmaxyx(stdscr, HEIGHT, WIDTH);

  struct Point top_center = {WIDTH/2, 0};
  struct Point hanging = {WIDTH/2, HEIGHT/2.5};


  float av1 = 0;
  float av2 = 0;

  clock_t tp1 = clock();
  clock_t tp2 = clock();

  while(1) {

    tp2 = clock();
    float elapsed_time = (float)(tp2 - tp1) / CLOCKS_PER_SEC;
    tp1 = tp2;

    erase();
    draw_line(top_center, hanging, '|');

    struct Point s1 = {
      hanging.x + R1 * sin(ANGLE1),
      hanging.y + R1 * cos(ANGLE1)
    };

    struct Point s2 = {
      s1.x + R2 * sin(ANGLE2),
      s1.y + R2 * cos(ANGLE2)
    };

    draw_line(hanging, s1, '*');
    draw_line(s1, s2, '*');

    draw_point(s1, '@');
    draw_point(s2, '@');

    float num1 = -G * (2 * M1 + M2) * sin(ANGLE1);
    float num2 = -M2 * G * sin(ANGLE1 - 2*ANGLE2);
    float num3 = -2*sin(ANGLE1 - ANGLE2)*M2;
    float num4 = av2*av2*R2 + av1*av1*R1*cos(ANGLE1 - ANGLE2);
    float den = R1 * (2*M1+M2-M2*cos(2*ANGLE1 - 2*ANGLE2));

    float a1 = (AIRRES ? -miu * av1 : 0) + (num1 + num2 + num3 * num4)/den;

    num1 = 2 * sin(ANGLE1 - ANGLE2);
    num2 = (av1 * av1 * R1 * (M1+M2));
    num3 = G * (M1+M2) * cos(ANGLE1);
    num4 = av2 * av2 * R2 * M2 * cos(ANGLE1 - ANGLE2);
    den = R2 * (2 * M1 + M2 - M2 * cos(2*ANGLE1 - 2*ANGLE2));

    float a2 = (AIRRES ? -miu * av2 : 0) + (num1 * (num2 + num3 + num4)) / den;

    av1 += a1 * elapsed_time;
    av2 += a2 * elapsed_time;
    
    ANGLE1 += av1 * elapsed_time;
    ANGLE2 += av2 * elapsed_time;
    
    mvprintw(0,0,"FPS %3.2f\n", 1.0/elapsed_time);

    refresh();
  }


  endwin();
  return 0;
}

void draw_point(struct Point point, char c) {
  mvaddch(point.y, point.x, c);
}

void draw_line(struct Point p1, struct Point p2, char c) {


  if (p1.y == p2.y) {
    mvhline(p1.y, p1.x, c, p2.x - p1.x);
    return;
  }

  if(p1.x == p2.x || p1.x == p2.x + 1) {
    mvvline(p1.y, p1.x, c, p2.y - p1.y);
    return;
  }

  struct Point fp, lp;

  if (p1.x < p2.x) {
    fp = p1;
    lp = p2;
  } else {
    fp = p2;
    lp = p1;
  }

  double dy = lp.y - fp.y;
  double dx = lp.x - fp.x;
  double slope = dy/dx;

  if (dx >= dy) {
    for (int x = fp.x; x < lp.x; x++) {
      struct Point point = {x, (fp.y + (x - fp.x)*slope)};
      draw_point(point, c);
    }  
  } else {
    for (int y = fp.y; y < lp.y; y++) {
      struct Point point = {(fp.x + (y - fp.y)/slope), y};
      draw_point(point, c);
    }
  }

}

