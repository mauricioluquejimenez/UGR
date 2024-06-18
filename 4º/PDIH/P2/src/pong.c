#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Variables globales para determinar el ancho y el alto de la ventana disponible
 */
int max_x, max_y;

/**
 * @brief Variables globales para determinar la puntuación de cada jugador
 */
int P1 = 0, P2 = 0;

/**
 * @brief Struct que almacena coordenadas X e Y
 * Utilizado tanto para la pelota como para las paletas
 */
struct object { int x, y; };
typedef struct object Object;

/**
* @brief Función auxiliar que determina si la pelota ha llegado a un límite de la ventana
* @param o Coordenada X o Y de la pelota
* @param dir Sentido de la pelota (si es 1 se está moviendo hacia la derecha, si es -1 se está moviendo hacia la izquierda)
* @param max Valor máximo de la pantalla (puede ser en el eje X o en el eje Y)
* @return true La pelota está en algún límite de la pantalla
* @return false La pelota no está en ningún límite de la pantalla
*/
bool edge(int o, int dir, int max) { return (o + dir < 0 || o + dir >= max); }

/**
* @brief Función auxiliar que determina si la pelota coordenada Y está en el rango de posiciones Y de la paleta. Utilizada en la función paddle() para minimizar código
* @param o Pelota
* @param paddle Paleta contra la que va a colisionar la pelota. Como es una línea de longitud 7, la coordenada Y de la pelota tiene que estar entre los 7 valores distintos de la coordenada Y de la paleta
* @return true La coordenada Y está en el rango de posiciones Y de la paleta
* @return false La coordenada Y no coincide con ninguna de las posiciones Y de la paleta
*/
bool range(Object o, Object paddle) { return (o.y >= paddle.y && o.y <= paddle.y + 6); }

/**
 * @brief Función auxiliar que determina si una pelota colisiona con una paleta
 * @param dir Sentido de la pelota
 * @param direction Valor con el que se compara la variable dir (si es 1 se está moviendo hacia la derecha, si es -1 se está moviendo hacia la izquierda)
 * @param o Pelota
 * @param paddle Paleta
 * @return true La pelota colisiona con la paleta
 * @return false La pelota no colisiona con la paleta
 */
bool paddle(int dir, int direction, Object o, Object paddle) { return (dir == direction && o.x == paddle.x - direction && range(o, paddle)); }

/**
 * @brief Función que inicia el programa
 * Inicia un par de colores para texto y fondo, una pantalla con los valores de las dimensiones globales y muestra un menú sencillo para iniciar el juego
 */
void inicio()
{
  clear();
  refresh();

  WINDOW *window = newwin(max_y, max_x, 0, 0);
  wbkgd(window, COLOR_PAIR(1));
  box(window, '|', '-');

  mvwprintw(window, 5, 52, "¡Hola! Bienvenido al Pong realizado por Mauricio Luque Jiménez.");
  mvwprintw(window, 12, 50, "Este juego se ha desarrollado en C utilizando la librería Ncurses.");
  mvwprintw(window, 19, 40, "Pulsa W y S para mover hacia arriba y abajo respectivamente la paleta de la izquierda. ");
  mvwprintw(window, 20, 40, "Pulsa O y L para mover hacia arriba y abajo respectivamente la paleta de la derecha. ");
  mvwprintw(window, 30, 32, "Pulsa cualquier tecla para empezar a jugar. El primer jugador que llegue a 10 puntos gana. ¡Buena suerte!");
  wrefresh(window);

  getch();
}

/**
 * @brief Juego clásico Pong
 */
void pong()
{
  struct object o, ol, ws;

  int c = 0;
  int dir_y = 1, dir_x = 1;

  P1 = 0; P2 = 0;

  nodelay(stdscr, TRUE);

  clear();
  refresh();

  o.x = max_x / 2;
  o.y = rand() % (max_y + 1);

  ol.x = max_x - 5;
  ol.y = max_y / 2;

  ws.x = 5;
  ws.y = max_y / 2;

  while(P1 != 10 && P2 != 10)
  {
    clear();

    mvprintw(2, max_x / 2 - 2, "%i | %i", P1, P2);
    mvvline(0, max_x / 2, ACS_VLINE, max_y);

    mvprintw(o.y, o.x, "o");

    mvvline(ws.y, ws.x, ACS_VLINE, 7);
    mvvline(ol.y, ol.x, ACS_VLINE, 7);

    if (edge(o.y, dir_y, max_y)) dir_y *= -1;
    if (edge(o.x, dir_x, max_x))
    {

      if (o.x < max_x / 2) P2++;
      else P1++;

      o.x = max_x / 2;
      o.y = rand() % (max_y + 1);
      dir_x *= -1;
    }
    else if (paddle(dir_x, -1, o, ws) || paddle(dir_x, 1, o, ol)) dir_x *= -1;

    o.x += dir_x;
    o.y += dir_y;

    c = getch();
    switch(c)
    {
      case('o'): if (ol.y > 1)            ol.y--;  break;
      case('l'): if (ol.y < max_y - 5)  ol.y++;  break;
      case('w'): if (ws.y > 1)           ws.y--;  break;
      case('s'): if (ws.y < max_y - 5) ws.y++;  break;
    }

    usleep(50000);

    refresh();
  }
}

/**
 * @brief Función que muestra una pantalla de final de partida
 * Muestra la puntuación de cada jugador y permite volver a jugar o terminar el programa
 * @return char Pulsación de tecla del usuario para empezar una nueva partida o salir del programa
 */
char fin()
{
  char restart;

  nodelay(stdscr, FALSE);

  clear();
  refresh();

  WINDOW *window = newwin(max_y, max_x, 0, 0);
  wbkgd(window, COLOR_PAIR(1));
  box(window, '|', '-');

  mvwprintw(window, 7, 65, "¡Buena partida! Aquí están los resultados: ");
  mvwprintw(window, 14, 75, "Jugador 1: %i punto(s)", P1);
  mvwprintw(window, 15, 75, "Jugador 2: %i punto(s)", P2);

  if (P1 > P2) mvwprintw(window, 20, 59, "¡Ha ganado el Jugador 1!¿Querrá revancha el Jugador 2?");
  if (P2 > P1) mvwprintw(window, 20, 59, "¡Ha ganado el Jugador 2!¿Querrá revancha el Jugador 1?");

  mvwprintw(window, 27, 58, "Pulsa R para reiniciar el juego, o pulsa Q para salir.");

  wrefresh(window);

  restart = getch();

  return restart;
}

/**
 * @brief Función principal que inicializa la ventana en modo ncurses y llama a las funciones auxiliares para correr el juego
 * @return int 
 */
int main()
{
  char restart;

  initscr();
  curs_set(FALSE);
  noecho();

  getmaxyx(stdscr, max_y, max_x);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  inicio();
  pong();

  while (true)
  {
    restart = fin();
    if (restart == 'r' || restart == 'R') pong();
    if (restart == 'q' || restart == 'Q') break;
  }

  endwin();

  return 0;
}
