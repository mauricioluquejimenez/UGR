import sys
from heapq import heappush, heappop

# Definición de la clase Nodo para representar cada estado del puzzle
class Nodo:
    def __init__(self, estado, padre=None, movimiento=None, costo=0):
        self.estado = estado
        self.padre = padre
        self.movimiento = movimiento
        self.costo = costo
        self.heuristica = self.calcular_heuristica()

    def __lt__(self, otro):
        return (self.costo + self.heuristica) < (otro.costo + otro.heuristica)

    def calcular_heuristica(self):
        heuristica = 0
        for i in range(9):
            if self.estado[i] == '0':
                continue
            fila_actual, col_actual = divmod(i, 3)
            fila_objetivo, col_objetivo = divmod(int(self.estado[i]) - 1, 3)
            heuristica += abs(fila_actual - fila_objetivo) + abs(col_actual - col_objetivo)
        return heuristica

    def obtener_sucesores(self):
        sucesores = []
        indice = self.estado.index('0')
        fila, col = divmod(indice, 3)

        for movimiento in self.obtener_movimientos_posibles(fila, col):
            nuevo_estado = list(self.estado)
            nuevo_indice = indice + movimiento
            nuevo_estado[indice], nuevo_estado[nuevo_indice] = nuevo_estado[nuevo_indice], nuevo_estado[indice]
            sucesores.append(Nodo(''.join(nuevo_estado), self, movimiento, self.costo + 1))
        return sucesores

    def obtener_movimientos_posibles(self, fila, col):
        movimientos = []
        if fila > 0:
            movimientos.append(-3)
        if fila < 2:
            movimientos.append(3)
        if col > 0:
            movimientos.append(-1)
        if col < 2:
            movimientos.append(1)
        return movimientos

    def obtener_camino(self):
        camino = []
        nodo_actual = self
        while nodo_actual:
            camino.append(nodo_actual)
            nodo_actual = nodo_actual.padre
        return camino[::-1]

# Función para resolver el 8-Puzzle
def resolver_8_puzzle(estado_inicial):
    estado_objetivo = "123456780"
    if estado_inicial == estado_objetivo:
        return [estado_inicial], [], [estado_inicial]

    frontera = []
    heappush(frontera, Nodo(estado_inicial))
    explorados = set()

    while frontera:
        nodo_actual = heappop(frontera)

        if nodo_actual.estado == estado_objetivo:
            camino = nodo_actual.obtener_camino()
            movimientos = [nodo.movimiento for nodo in camino[1:]]
            estados = [nodo.estado for nodo in camino]
            return estados, movimientos, camino

        explorados.add(nodo_actual.estado)

        for sucesor in nodo_actual.obtener_sucesores():
            if sucesor.estado not in explorados:
                heappush(frontera, sucesor)

    return None, None, None

# Función para imprimir el estado en forma de cuadrícula 3x3
def imprimir_cuadricula(estado):
    for i in range(0, 9, 3):
        print(estado[i:i+3])
    print()

# Función principal
def main():
    if len(sys.argv) != 2:
        print("Uso: E2.py <configuracion>")
        sys.exit(1)

    estado_inicial = sys.argv[1]
    if len(estado_inicial) != 9 or not estado_inicial.isdigit() or '0' not in estado_inicial:
        print("La configuración debe ser una cadena de 9 dígitos que incluya el 0.")
        sys.exit(1)

    estados, movimientos, camino = resolver_8_puzzle(estado_inicial)

    if estados:
        print("Solución encontrada:")
        for estado in estados:
            imprimir_cuadricula(estado)
        print("Movimientos:", movimientos)
    else:
        print("No se encontró solución.")

if __name__ == "__main__":
    main()