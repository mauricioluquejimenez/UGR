# resolver_bfs.py

from collections import deque

from estado import representacion_estado, mostrar_estado
from acciones import localizar_casilla_vacia
from espacio import EstadosSucesores


def estado_a_tupla(estado):
    """
    Convierte un estado representado como lista de listas en una tupla de tuplas.
    Esto permite usarlo en estructuras como conjuntos y diccionarios.
    """
    return tuple(tuple(fila) for fila in estado)


def reconstruir_camino(padres, estado_objetivo_tupla):
    """
    Reconstruye la secuencia de movimientos desde el estado inicial
    hasta el estado objetivo usando el diccionario de padres.
    """
    movimientos = []
    estado_actual = estado_objetivo_tupla

    while padres[estado_actual][0] is not None:
        estado_padre, operador = padres[estado_actual]
        movimientos.append(operador)
        estado_actual = estado_padre

    movimientos.reverse()
    return movimientos


def obtener_operadores():
    """
    Devuelve la lista de operadores del problema.
    Mantener esta función separada facilita modificar el algoritmo
    o el orden de exploración.
    """
    return ["U", "D", "L", "R"]


def crear_frontera_bfs():
    """
    Crea la estructura de frontera para búsqueda en anchura.
    Esta modularidad permite cambiar fácilmente a otro tipo
    de búsqueda, como profundidad.
    """
    return deque()


def insertar_en_frontera_bfs(frontera, elemento):
    """
    Inserta un elemento en la frontera de búsqueda en anchura.
    """
    frontera.append(elemento)


def extraer_de_frontera_bfs(frontera):
    """
    Extrae el siguiente elemento de la frontera de búsqueda en anchura.
    """
    return frontera.popleft()


def frontera_vacia(frontera):
    """
    Comprueba si la frontera está vacía.
    """
    return len(frontera) == 0


def buscar_solucion_bfs(estado_inicial, estado_final):
    """
    Realiza una búsqueda en anchura desde el estado inicial
    hasta el estado final.

    Devuelve:
    - La lista de movimientos si encuentra solución
    - None si no encuentra solución
    """
    inicial_tupla = estado_a_tupla(estado_inicial)
    final_tupla = estado_a_tupla(estado_final)

    if inicial_tupla == final_tupla:
        return []

    frontera = crear_frontera_bfs()
    insertar_en_frontera_bfs(frontera, estado_inicial)

    visitados = set()
    visitados.add(inicial_tupla)

    # Cada entrada almacena:
    # clave: estado actual
    # valor: (estado padre, operador aplicado para llegar aquí)
    padres = {inicial_tupla: (None, None)}

    while not frontera_vacia(frontera):
        estado_actual = extraer_de_frontera_bfs(frontera)

        for operador, sucesor in EstadosSucesores(estado_actual):
            sucesor_tupla = estado_a_tupla(sucesor)

            if sucesor_tupla not in visitados:
                visitados.add(sucesor_tupla)
                padres[sucesor_tupla] = (estado_a_tupla(estado_actual), operador)

                if sucesor_tupla == final_tupla:
                    return reconstruir_camino(padres, sucesor_tupla)

                insertar_en_frontera_bfs(frontera, sucesor)

    return None


def imprimir_movimientos(movimientos):
    """
    Muestra por pantalla la secuencia de movimientos realizada.
    """
    for i, movimiento in enumerate(movimientos, start=1):
        print(f"{i}. {movimiento}")


def Soluciona8Puzzle(estado_inicial, estado_final):
    """
    Resuelve el problema del 8-puzzle mediante búsqueda en anchura.

    - Imprime el estado inicial
    - Imprime el estado final
    - Indica si el estado final es alcanzable
    - Si lo es, muestra la secuencia de movimientos
    """
    print("Estado inicial:")
    mostrar_estado(estado_inicial)

    print("\nEstado final:")
    mostrar_estado(estado_final)

    movimientos = buscar_solucion_bfs(estado_inicial, estado_final)

    if movimientos is None:
        print("\nEl estado final no es alcanzable desde el estado inicial.")
    else:
        print("\nEl estado final es alcanzable.")
        print("Movimientos realizados:")

        if len(movimientos) == 0:
            print("No es necesario realizar movimientos.")
        else:
            imprimir_movimientos(movimientos)


if __name__ == "__main__":
    # Lectura de estados desde terminal
    cadena_inicial = input("Introduce el estado inicial: ")
    cadena_final = input("Introduce el estado final: ")

    estado_inicial = representacion_estado(cadena_inicial)
    estado_final = representacion_estado(cadena_final)

    Soluciona8Puzzle(estado_inicial, estado_final)