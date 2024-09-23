import curses
import random
import time

# Diccionario para almacenar las puntuaciones utilizando estructuras de Python
scores = {'P1':0, 'P2': 0}

# Clase que almacena coordenadas X e Y
# Útil para la pelota y las paletas
class object:
    def __init__(self, _x, _y):
        self.x = _x;
        self.y = _y;

# Función auxiliar que determina si la pelota ha llegado a un límite de la ventana
# o: Coordenada X o Y de la pelota
# dir: Sentido de la pelota (si es 1 se está moviendo hacia la derecha, si es -1 se está moviendo hacia la izquierda)
# max: Valor máximo de la pantalla (puede ser en el eje X o en el eje Y)
# true: La pelota está en algún límite de la pantalla
# false: La pelota no está en ningún límite de la pantalla
def edge(o, dir, max): return o + dir < 0 or o + dir >= max

# Función auxiliar que determina si la pelota coordenada Y está en el rango de posiciones Y de la paleta. Utilizada en la función paddle() para minimizar código
# o: Pelota
# paddle: Paleta contra la que va a colisionar la pelota. Como es una línea de longitud 7, la coordenada Y de la pelota tiene que estar entre los 7 valores distintos de la coordenada Y de la paleta
# true: La coordenada Y está en el rango de posiciones Y de la paleta
# false: La coordenada Y no coincide con ninguna de las posiciones Y de la paleta
def range(o, paddle): return o.y >= paddle.y and o.y <= paddle.y + 6

# Función auxiliar que determina si una pelota colisiona con una paleta
# dir: Sentido de la pelota
# direction: Valor con el que se compara la variable dir (si es 1 se está moviendo hacia la derecha, si es -1 se está moviendo hacia la izquierda)
# o: Pelota
# paddle: Paleta
# true: La pelota colisiona con la paleta
# false: La pelota no colisiona con la paleta
def paddle(dir, direction, o, paddle): return dir == direction and o.x == paddle.x - direction and range(o, paddle)

# Función que inicia el programa
# Inicia un par de colores para texto y fondo, una pantalla con los valores de las dimensiones globales y muestra un menú sencillo para iniciar el juego
def inicio(stdscr):
    curses.start_color()
    curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLUE)

    max_y, max_x = stdscr.getmaxyx()

    stdscr.clear()
    stdscr.refresh()

    window = curses.newwin(max_y, max_x, 0, 0)
    window.bkgd(curses.color_pair(1))
    window.box('|', '-')

    window.addstr(5, 52, "¡Hola! Bienvenido al Pong realizado por Mauricio Luque Jiménez.")
    window.addstr(12, 50, "Este juego se ha desarrollado en Python utilizando la librería Curses.")
    window.addstr(19, 40, "Pulsa W y S para mover hacia arriba y abajo respectivamente la paleta de la izquierda.")
    window.addstr(20, 40, "Pulsa O y L para mover hacia arriba y abajo respectivamente la paleta de la derecha.")
    window.addstr(30, 32, "Pulsa cualquier tecla para empezar a jugar. El primer jugador que llegue a 10 puntos gana. ¡Buena suerte!")
    window.refresh()

    stdscr.getch()

# Juego clásico Pong
def pong(stdscr):
    stdscr.nodelay(True)

    stdscr.clear()
    stdscr.refresh()

    max_y, max_x = stdscr.getmaxyx()

    dir_x = dir_y = 1
    scores['P1'] = scores['P2'] = 0

    o = object(max_x // 2, random.randint(0, max_y))
    ol = object (max_x - 5, max_y // 2)
    ws = object (5, max_y // 2)

    while scores['P1'] != 10 and scores['P2'] != 10:
        stdscr.clear()

        stdscr.addstr(2, max_x // 2 - 2, f"{scores['P1']} | {scores['P2']}")
        stdscr.vline(0, max_x // 2, curses.ACS_VLINE, max_y)

        stdscr.addstr(o.y, o.x, "o")

        stdscr.vline(ws.y, ws.x, curses.ACS_VLINE, 7)
        stdscr.vline(ol.y, ol.x, curses.ACS_VLINE, 7)

        if edge(o.y, dir_y, max_y): dir_y *= -1
        if edge(o.x, dir_x, max_x): 
            if o.x < max_x // 2: scores['P2'] += 1
            else: scores['P1'] += 1

            o.x = max_x // 2
            o.y = random.randint(0, max_y)
            dir_x *= -1

        elif paddle(dir_x, -1, o, ws) or paddle(dir_x, 1, o, ol): dir_x *= -1

        o.x += dir_x
        o.y += dir_y

        c = stdscr.getch()
        if (c == ord('o') or c == ord('O')) and ol.y > 1: ol.y -= 1
        elif (c == ord('l') or c == ord('L')) and ol.y < max_y - 5: ol.y += 1
        elif (c == ord('w') or c == ord('W')) and ws.y > 1: ws.y -= 1
        elif (c == ord('s') or c == ord('S')) and ws.y < max_y - 5: ws.y += 1

        time.sleep(0.060)

        stdscr.refresh()

# Función que muestra una pantalla de final de partida
# Muestra la puntuación de cada jugador y permite volver a jugar o terminar el programa
# Pulsación de tecla del usuario para empezar una nueva partida o salir del programa
def fin(stdscr, scores):
    stdscr.nodelay(False)

    curses.start_color()
    curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLUE)

    stdscr.clear()
    stdscr.refresh()

    max_y, max_x = stdscr.getmaxyx()

    window = curses.newwin(max_y, max_x, 0, 0)
    window.bkgd(curses.color_pair(1))
    window.box('|', '-')

    window.addstr(7, 65, "¡Buena partida! Aquí están los resultados: ")
    window.addstr(14, 75, f"Jugador 1: {scores['P1']} punto(s)")
    window.addstr(15, 75, f"Jugador 2: {scores['P2']} punto(s)")

    if scores['P1'] > scores['P2']: window.addstr(20, 59, "¡Ha ganado el Jugador 1!¿Querrá revancha el Jugador 2?")
    if scores['P2'] > scores['P1']: window.addstr(20, 59, "¡Ha ganado el Jugador 2!¿Querrá revancha el Jugador 1?")

    window.addstr(27, 58, "Pulsa R para reiniciar el juego, o pulsa Q para salir.")
    window.refresh()

    return stdscr.getch()

# Función principal que inicializa la ventana en modo ncurses y llama a las funciones auxiliares para correr el juego
def main(stdscr):
    curses.curs_set(False)
    curses.noecho()

    inicio(stdscr)
    pong(stdscr)

    while True:
        restart = fin(stdscr, scores)
        if restart == ord('r') or restart == ord('R'): pong(stdscr)
        elif restart == ord('q') or restart == ord('Q'): break

    curses.endwin()

curses.wrapper(main)
