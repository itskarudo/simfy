#define NCURSES_WIDECHAR 69

#include <curses.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>


struct Player {
  float x;
  float y;
  float angle;
  float FOV;
};

struct Player player = {8.0, 6.0, 0.0, M_PI_4};

int map_height = 16;
int map_width = 16;

float depth = 16;
float speed = 20;

void get_keys(float elapsed_time);

char *map = "################"
            "#..............#"
            "#..........#...#"
            "#..........#...#"
            "#..............#"
            "######.........#"
            "#..............#"
            "#..............#"
            "#..............#"
            "#........#.....#"
            "####.....#.....#"
            "#........#.....#"
            "#........#######"
            "#..............#"
            "#..............#"
            "################";

int main(void) {


  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);

  int WIDTH, HEIGHT;
  getmaxyx(stdscr, HEIGHT, WIDTH);

  clock_t tp1 = clock();
  clock_t tp2 = clock();

  while (1) {

    tp2 = clock();
    float elapsed_time = (float)(tp2 - tp1) / CLOCKS_PER_SEC;
    tp1 = tp2;

    get_keys(elapsed_time);

    for (int x = 0; x < WIDTH; x++) {

      float ray_angle = player.angle - (player.FOV / 2) + ((float)x / (float)WIDTH) * player.FOV;

      float distance_to_wall = 0;
      bool hit_wall = false;

      float eyeX = sin(ray_angle);
      float eyeY = cos(ray_angle);

      while (!hit_wall && distance_to_wall < depth) {

        distance_to_wall += 0.01;

        int testX = (int)(player.x + eyeX * distance_to_wall);
        int testY = (int)(player.y + eyeY * distance_to_wall);

        if (testX < 0 || testX >= map_width || testY < 0 || testY >= map_height) {

          hit_wall = true;
          distance_to_wall = depth;

        } else if (map[testY * map_width + testX] == '#') {
            hit_wall = true;
        }

      }
    
      int ceiling = (float)(HEIGHT/2) - HEIGHT / (float)distance_to_wall;
      int floor = HEIGHT - ceiling;
      
      wchar_t shade[2] = {L' ', L'\0'};

      if (distance_to_wall <= depth/4.0) shade[0] = 0x2588;
      else if (distance_to_wall < depth/3.0) shade[0] = 0x2593;
      else if (distance_to_wall < depth/2.0) shade[0] = 0x2592;
      else if (distance_to_wall < depth) shade[0] = 0x2591;

      
      for (int y = 0; y < HEIGHT; y++) {

        if (y < ceiling)
          mvaddch(y, x, ' ');
        else if (y > floor) {

          float b = 1.0 - (((float)y - HEIGHT / 2.0) / ((float)HEIGHT/2.0));
          char floorchar;

          if (b < 0.25) floorchar = '#';
          else if (b < 0.5) floorchar = 'x';
          else if (b < 0.75) floorchar = '.';
          else if (b < 0.9) floorchar = '-';
          else floorchar = ' ';

          mvaddch(y, x, floorchar);
            

        } else
          mvaddwstr(y, x, shade);

      }

    }

    mvprintw(0,0,"FPS: %3.2f", 1.0/elapsed_time);

    for (int nx = 0; nx < map_width; nx++) {

      for (int ny = 0; ny < map_height; ny++) {

        mvaddch(ny+1, nx, map[ny * map_width + nx]);
      }

    }

    mvaddch((int)player.y+1, (int)player.x, 'P');

    refresh();



  }


  endwin();
  
  return 0;
}

void get_keys(float elapsed_time) {

    char ch = getch();
    if (ch != ERR) {

      if (ch == 'w') {

          player.x +=  sin(player.angle) * speed * elapsed_time;
          player.y +=   cos(player.angle) * speed * elapsed_time;

          if (map[(int)player.x * map_width + (int)player.y] == '#') {
            player.x -=  sin(player.angle) * speed * elapsed_time;
            player.y -=  cos(player.angle) * speed * elapsed_time;
          }

      } else if (ch == 's') {

          player.x -=  sin(player.angle) * speed * elapsed_time;
          player.y -=   cos(player.angle) * speed * elapsed_time;

          if (map[(int)player.x * map_width + (int)player.y] == '#') {
            player.x += sin(player.angle) * speed * elapsed_time;
            player.y +=  cos(player.angle) * speed * elapsed_time;
          }

      } else if (ch == 'd') {
          player.angle +=  (speed * 0.75) * elapsed_time;
      } else if (ch == 'a') {
          player.angle -=  (speed * 0.75) * elapsed_time;
      }

    }

}
