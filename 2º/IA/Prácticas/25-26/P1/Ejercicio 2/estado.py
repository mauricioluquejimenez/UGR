# estado.py

def representacion_estado(cadena):
    """
    Construye la representación interna de un estado del 8-puzzle
    a partir de una cadena de 9 dígitos, por ejemplo: "123456780".

    La representación elegida es una matriz 3x3 implementada como
    una lista de listas de enteros.
    """
    numeros = [int(caracter) for caracter in cadena]
    return [
        numeros[0:3],
        numeros[3:6],
        numeros[6:9]
    ]


def mostrar_estado(estado):
    """
    Muestra por pantalla una representación visual y legible
    de un estado del 8-puzzle. La casilla vacía (0) se representa
    con un espacio en blanco.
    """
    print("+---+---+---+")
    for fila in estado:
        fila_visual = []
        for valor in fila:
            if valor == 0:
                fila_visual.append(" ")
            else:
                fila_visual.append(str(valor))
        print(f"| {fila_visual[0]} | {fila_visual[1]} | {fila_visual[2]} |")
        print("+---+---+---+")


class Problema:
    """
    Representa un problema del 8-puzzle mediante dos estados:
    estado_inicial y estado_objetivo.
    """

    def __init__(self, estado_inicial, estado_objetivo):
        # Guarda los dos estados del problema
        self.estado_inicial = estado_inicial
        self.estado_objetivo = estado_objetivo

    def representacion_problema(self):
        """
        Muestra la representación interna del problema, es decir,
        los dos estados en forma de matrices 3x3.
        """
        print("Estado inicial:")
        print(self.estado_inicial)
        print("Estado objetivo:")
        print(self.estado_objetivo)

    def mostrar_problema(self):
        """
        Muestra por pantalla una representación visual y amigable
        del problema completo: estado inicial y estado objetivo.
        """
        print("Estado inicial:")
        mostrar_estado(self.estado_inicial)
        print()
        print("Estado objetivo:")
        mostrar_estado(self.estado_objetivo)

    def es_estado_objetivo(self, estado):
        """
        Devuelve True si el estado recibido coincide exactamente
        con el estado objetivo del problema, y False en caso contrario.
        """
        return estado == self.estado_objetivo


# Ejemplo de uso desde terminal
if __name__ == "__main__":
    # Se leen dos cadenas: estado inicial y estado objetivo
    cadena_inicial = input("Introduce el estado inicial: ")
    cadena_objetivo = input("Introduce el estado objetivo: ")

    # Se construyen las representaciones internas
    estado_inicial = representacion_estado(cadena_inicial)
    estado_objetivo = representacion_estado(cadena_objetivo)

    # Se crea el problema
    problema = Problema(estado_inicial, estado_objetivo)

    # Se muestra la representación interna
    print("\nRepresentación interna del problema:")
    problema.representacion_problema()

    # Se muestra la representación visual
    print("\nRepresentación visual del problema:")
    problema.mostrar_problema()

    # Ejemplo de comprobación
    print("\n¿El estado inicial es objetivo?")
    print(problema.es_estado_objetivo(estado_inicial))