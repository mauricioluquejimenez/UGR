def show_grid(state):
    """Muestra el estado del 8-puzzle como una cuadrícula."""
    print("-" * 13)
    for i in range(0, 9, 3):
        print("|", state[i], "|", state[i + 1], "|", state[i + 2], "|")
        print("-" * 13)

def valid_movements(state):
    """Encuentra los movimientos válidos para un estado dado."""
    pos_cero = state.index('0')
    movimientos = []
    if pos_cero not in [0, 1, 2]:  # Arriba
        movimientos.append('Arriba')
    if pos_cero not in [6, 7, 8]:  # Abajo
        movimientos.append('Abajo')
    if pos_cero not in [0, 3, 6]:  # Izquierda
        movimientos.append('Izquierda')
    if pos_cero not in [2, 5, 8]:  # Derecha
        movimientos.append('Derecha')
    return movimientos

def successor_states(state, movimientos):
    """Genera los estados sucesores."""
    sucesores = {}
    for movimiento in movimientos:
        nuevo_estado = list(state)  # Copia el estado para no modificarlo
        pos_cero = state.index('0')
        if movimiento == 'Arriba':
            nueva_pos = pos_cero - 3
        elif movimiento == 'Abajo':
            nueva_pos = pos_cero + 3
        elif movimiento == 'Izquierda':
            nueva_pos = pos_cero - 1
        else:  # Derecha
            nueva_pos = pos_cero + 1
        # Intercambia el 0 con la nueva posición
        nuevo_estado[pos_cero], nuevo_estado[nueva_pos] = nuevo_estado[nueva_pos], nuevo_estado[pos_cero]
        sucesores[movimiento] = ''.join(nuevo_estado)
    return sucesores

# Entrada del usuario
initial_state = input("Introduce la configuración inicial del 8-puzzle (ej. 130425786): ")

# Mostrar la cuadrícula inicial
print("\nEstado inicial:")
show_grid(initial_state)

# Encontrar los movimientos válidos
movimientos_validos = valid_movements(initial_state)
print("\nMovimientos válidos:", movimientos_validos)

# Generar los estados sucesores
estados_sucesores = successor_states(initial_state, movimientos_validos)
print("\nEstados sucesores:")
for movimiento, estado in estados_sucesores.items():
    print(f"\nMovimiento: {movimiento}")
    show_grid(estado)