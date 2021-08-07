#include <math.h>
#include <curses.h>
#include <time.h>

#define LENGTH 20
#define G 40
#define miu 0.05

#define AIRRES true

struct Point {
  int x;
  int y;
};

void draw_point(struct Point point, char c);
void draw_line(struct Point p1, struct Point p2, char c);

int main(void) {
  
  float angle = M_PI_2;
  float av = 0;


  int WIDTH, HEIGHT;

  initscr();
  getmaxyx(stdscr, HEIGHT, WIDTH);

  struct Point top_center = {WIDTH / 2, 0};
  struct Point center_center = {WIDTH / 2, HEIGHT / 2};

  clock_t tp1 = clock();
  clock_t tp2 = clock();


  while (1) {

    tp2 = clock();
    float elapsed_time = (float)(tp2 - tp1) / CLOCKS_PER_SEC;
    tp1 = tp2;

    erase();


    draw_line(top_center, center_center, '|');


    struct Point obj = {
      ((WIDTH/2) + LENGTH * sin(angle)),
      ((HEIGHT/2) + LENGTH * cos(angle))
    };


    draw_line(center_center, obj, '*');
    draw_point(obj, '@');

    float a = (AIRRES ? (-miu * av) : 0) + ((-G / LENGTH) * sin(angle));
    av +=  elapsed_time * a;
    angle +=  av * elapsed_time;

    mvprintw(0,0,"FPS: %3.2f | angle: %+2.3frad | angular velocity: %+2.3frad/s\n", 1.0/elapsed_time, angle, av);
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

