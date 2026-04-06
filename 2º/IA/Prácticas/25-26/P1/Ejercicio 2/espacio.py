# espacio.py

from estado import mostrar_estado, representacion_estado
from acciones import AplicaOperador, localizar_casilla_vacia


def EstadosSucesores(estado_actual):
    """
    Calcula todos los estados sucesores posibles de un estado dado.

    Recibe:
    - estado_actual: estado del 8-puzzle representado como una matriz 3x3

    Devuelve:
    - Una lista de tuplas con la forma (accion, estado_sucesor),
      donde accion es el operador aplicado ("U", "D", "L", "R")
      y estado_sucesor es el nuevo estado generado.
    """
    casilla_vacia = localizar_casilla_vacia(estado_actual)
    operadores = ["U", "D", "L", "R"]
    sucesores = []

    for operador in operadores:
        nuevo_estado = AplicaOperador(estado_actual, casilla_vacia, operador)

        # Si el estado cambia, entonces el operador era aplicable
        if nuevo_estado != estado_actual:
            sucesores.append((operador, nuevo_estado))

    return sucesores


if __name__ == "__main__":
    # Estado de ejemplo
    estado = representacion_estado("123405678")

    print("Estado actual:")
    mostrar_estado(estado)

    sucesores = EstadosSucesores(estado)

    print("\nEstados sucesores posibles:")
    if len(sucesores) == 0:
        print("No hay estados sucesores.")
    else:
        for accion, sucesor in sucesores:
            print(f"\nAcción aplicada: {accion}")
            mostrar_estado(sucesor)