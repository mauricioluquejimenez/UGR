package civitas;

class GestorEstados {
    EstadosJuego estadoInicial() {
        return (EstadosJuego.INICIO_TURNO);
    }
  
    OperacionJuego siguienteOperacion(Jugador jugador, EstadosJuego estado) {
        OperacionJuego op = null;

    switch (estado) {
        case INICIO_TURNO :
            op = OperacionJuego.AVANZAR;
        break;

        case DESPUES_AVANZAR :
            if (jugador.PuedeComprar())
                op = OperacionJuego.COMPRAR;
            else if (jugador.tieneAlgoQueGestionar())
                op = OperacionJuego.GESTIONAR;
            else
                op = OperacionJuego.PASAR_TURNO;
            break;

        case DESPUES_COMPRAR :
            if (jugador.tieneAlgoQueGestionar())
                op = OperacionJuego.GESTIONAR;
            else
                op = OperacionJuego.PASAR_TURNO;
            break;

        case DESPUES_GESTIONAR :
            op = OperacionJuego.PASAR_TURNO;
            break;        
    }
    return op;
  }
  
    EstadosJuego siguienteEstado (Jugador jugador, EstadosJuego estado, OperacionJuego operacion) {
        EstadosJuego siguiente = null;

    switch (estado) {
        case INICIO_TURNO :
            if (operacion==OperacionJuego.AVANZAR)
                siguiente = EstadosJuego.DESPUES_AVANZAR;
            break;

        case DESPUES_AVANZAR :
            switch (operacion) {
                case PASAR_TURNO :
                    siguiente = EstadosJuego.INICIO_TURNO;
                    break;
                case COMPRAR :
                    siguiente = EstadosJuego.DESPUES_COMPRAR;
                    break;
                case GESTIONAR :
                    siguiente = EstadosJuego.DESPUES_GESTIONAR;
                    break;
            }
            break;

        case DESPUES_COMPRAR :
            if (operacion==OperacionJuego.GESTIONAR)
                siguiente = EstadosJuego.DESPUES_GESTIONAR;
            else if (operacion==OperacionJuego.PASAR_TURNO)
                siguiente = EstadosJuego.INICIO_TURNO;
            break;

        case DESPUES_GESTIONAR :
            if (operacion==OperacionJuego.PASAR_TURNO)
                siguiente = EstadosJuego.INICIO_TURNO;
            break;
    }

    Diario.getInstance().ocurreEvento("De: "+estado.toString()+ " con "+operacion.toString()+ " sale: "+siguiente.toString());

    return siguiente;
  }

}