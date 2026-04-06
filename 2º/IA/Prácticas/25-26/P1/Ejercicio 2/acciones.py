# acciones.py

from estado import representacion_estado, mostrar_estado


def localizar_casilla_vacia(estado):
    """
    Devuelve la posición (fila, columna) de la casilla vacía (0).
    """
    for i in range(3):
        for j in range(3):
            if estado[i][j] == 0:
                return (i, j)


def EsAplicableU(accion, casilla_vacia):
    """
    Comprueba si el operador U es aplicable.
    La casilla vacía puede subir si no está en la primera fila.
    """
    fila, columna = casilla_vacia
    return accion == "U" and fila > 0


def EsAplicableD(accion, casilla_vacia):
    """
    Comprueba si el operador D es aplicable.
    La casilla vacía puede bajar si no está en la última fila.
    """
    fila, columna = casilla_vacia
    return accion == "D" and fila < 2


def EsAplicableL(accion, casilla_vacia):
    """
    Comprueba si el operador L es aplicable.
    La casilla vacía puede moverse a la izquierda si no está en la primera columna.
    """
    fila, columna = casilla_vacia
    return accion == "L" and columna > 0


def EsAplicableR(accion, casilla_vacia):
    """
    Comprueba si el operador R es aplicable.
    La casilla vacía puede moverse a la derecha si no está en la última columna.
    """
    fila, columna = casilla_vacia
    return accion == "R" and columna < 2


def posicion_posterior_U(accion, casilla_vacia):
    """
    Calcula la nueva posición de la casilla vacía y la ficha desplazada
    al aplicar el operador U.
    """
    fila, columna = casilla_vacia
    nueva_vacia = (fila - 1, columna)
    ficha_desplazada = (fila - 1, columna)
    return nueva_vacia, ficha_desplazada


def posicion_posterior_D(accion, casilla_vacia):
    """
    Calcula la nueva posición de la casilla vacía y la ficha desplazada
    al aplicar el operador D.
    """
    fila, columna = casilla_vacia
    nueva_vacia = (fila + 1, columna)
    ficha_desplazada = (fila + 1, columna)
    return nueva_vacia, ficha_desplazada


def posicion_posterior_L(accion, casilla_vacia):
    """
    Calcula la nueva posición de la casilla vacía y la ficha desplazada
    al aplicar el operador L.
    """
    fila, columna = casilla_vacia
    nueva_vacia = (fila, columna - 1)
    ficha_desplazada = (fila, columna - 1)
    return nueva_vacia, ficha_desplazada


def posicion_posterior_R(accion, casilla_vacia):
    """
    Calcula la nueva posición de la casilla vacía y la ficha desplazada
    al aplicar el operador R.
    """
    fila, columna = casilla_vacia
    nueva_vacia = (fila, columna + 1)
    ficha_desplazada = (fila, columna + 1)
    return nueva_vacia, ficha_desplazada


def aplicar_U(estado, casilla_vacia):
    """
    Aplica el operador U intercambiando la casilla vacía con la ficha superior.
    """
    nueva_vacia, ficha_desplazada = posicion_posterior_U("U", casilla_vacia)

    fila_vacia, col_vacia = casilla_vacia
    fila_ficha, col_ficha = ficha_desplazada

    nuevo_estado = [fila[:] for fila in estado]
    nuevo_estado[fila_vacia][col_vacia], nuevo_estado[fila_ficha][col_ficha] = (
        nuevo_estado[fila_ficha][col_ficha],
        nuevo_estado[fila_vacia][col_vacia],
    )

    return nuevo_estado


def aplicar_D(estado, casilla_vacia):
    """
    Aplica el operador D intercambiando la casilla vacía con la ficha inferior.
    """
    nueva_vacia, ficha_desplazada = posicion_posterior_D("D", casilla_vacia)

    fila_vacia, col_vacia = casilla_vacia
    fila_ficha, col_ficha = ficha_desplazada

    nuevo_estado = [fila[:] for fila in estado]
    nuevo_estado[fila_vacia][col_vacia], nuevo_estado[fila_ficha][col_ficha] = (
        nuevo_estado[fila_ficha][col_ficha],
        nuevo_estado[fila_vacia][col_vacia],
    )

    return nuevo_estado


def aplicar_L(estado, casilla_vacia):
    """
    Aplica el operador L intercambiando la casilla vacía con la ficha de la izquierda.
    """
    nueva_vacia, ficha_desplazada = posicion_posterior_L("L", casilla_vacia)

    fila_vacia, col_vacia = casilla_vacia
    fila_ficha, col_ficha = ficha_desplazada

    nuevo_estado = [fila[:] for fila in estado]
    nuevo_estado[fila_vacia][col_vacia], nuevo_estado[fila_ficha][col_ficha] = (
        nuevo_estado[fila_ficha][col_ficha],
        nuevo_estado[fila_vacia][col_vacia],
    )

    return nuevo_estado


def aplicar_R(estado, casilla_vacia):
    """
    Aplica el operador R intercambiando la casilla vacía con la ficha de la derecha.
    """
    nueva_vacia, ficha_desplazada = posicion_posterior_R("R", casilla_vacia)

    fila_vacia, col_vacia = casilla_vacia
    fila_ficha, col_ficha = ficha_desplazada

    nuevo_estado = [fila[:] for fila in estado]
    nuevo_estado[fila_vacia][col_vacia], nuevo_estado[fila_ficha][col_ficha] = (
        nuevo_estado[fila_ficha][col_ficha],
        nuevo_estado[fila_vacia][col_vacia],
    )

    return nuevo_estado


def AplicaOperador(estado, casilla_vacia, operador):
    """
    Función de transición del mundo.
    Recibe un estado, la posición de la casilla vacía y el operador a aplicar.
    Devuelve el nuevo estado tras realizar el movimiento.
    """
    if operador == "U" and EsAplicableU(operador, casilla_vacia):
        return aplicar_U(estado, casilla_vacia)

    if operador == "D" and EsAplicableD(operador, casilla_vacia):
        return aplicar_D(estado, casilla_vacia)

    if operador == "L" and EsAplicableL(operador, casilla_vacia):
        return aplicar_L(estado, casilla_vacia)

    if operador == "R" and EsAplicableR(operador, casilla_vacia):
        return aplicar_R(estado, casilla_vacia)

    return estado


# Ejemplo de uso sencillo
if __name__ == "__main__":
    estado_inicial = representacion_estado("123405678")
    casilla_vacia = localizar_casilla_vacia(estado_inicial)

    print("Estado inicial:")
    mostrar_estado(estado_inicial)

    print("\nCasilla vacía:", casilla_vacia)
    print("¿U aplicable?:", EsAplicableU("U", casilla_vacia))
    print("¿D aplicable?:", EsAplicableD("D", casilla_vacia))
    print("¿L aplicable?:", EsAplicableL("L", casilla_vacia))
    print("¿R aplicable?:", EsAplicableR("R", casilla_vacia))

    nuevo_estado = AplicaOperador(estado_inicial, casilla_vacia, "R")

    print("\nEstado tras aplicar R:")
    mostrar_estado(nuevo_estado)