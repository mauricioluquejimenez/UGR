package civitas;

import java.util.ArrayList;

public class CivitasJuego {
    private int indiceJugadorActual;
    private ArrayList<Jugador> jugadores;
    private Tablero tablero;
    private MazoSorpresas mazo;
    private EstadosJuego estado;
    private GestorEstados gestor;
    
    
    void avanzaJugador(){
        Jugador jugadorActual=getJugadorActual();
        int posicionActual=jugadorActual.getCasillaActual();
        int tirada=Dado.getInstance().tirar();
        int posicionNueva=tablero.nuevaPosicion(posicionActual,tirada);
        Casilla casilla=tablero.getCasilla(posicionNueva);
        contabilizarPasosPorSalida(jugadorActual);
        jugadorActual.moverACasilla(posicionNueva);
        casilla.recibeJugador(indiceJugadorActual, jugadores);       
    }
    public CivitasJuego(ArrayList<String> nombres,boolean debug){
        gestor=new GestorEstados();
        gestor.estadoInicial();
        boolean modo=debug;
    }
    public boolean comprar(){
        Jugador jugadorActual=getJugadorActual();
        int numCasillaActual=jugadorActual.getCasillaActual();
        CasillaCalle casilla=tablero.getCasilla(numCasillaActual);
        boolean res=jugadorActual.comprar(casilla);
        return res;
    }
    public boolean ConstruirCasa(int ip){
        return getJugadorActual().construirCasa(ip);
    }
    public boolean ConstruirHotel(int ip){
        return getJugadorActual().construirHotel(ip);
    }
    void contabilizarPasosPorSalida(Jugador jugadorActual){
        jugadorActual.pasaPorSalida();
    }
    public boolean finalDelJuego(){
        boolean fin=false;
        for(int i=0;i<jugadores.size();i++)
            if(jugadores.get(i).enBancarrota());
                fin=true;
        return fin;
    }
    int getIndiceJugadorActual(){
        return indiceJugadorActual;
    }
    Jugador getJugadorActual(){
        return jugadores.get(indiceJugadorActual);
    }
    ArrayList<Jugador> getJugadores(){
        return jugadores;
    }
    void inicialziaTablero(MazoSorpresas mazo){
        
    }
    void inicializaMazoSorpresas(){
        
    }
    void pasarTurno(){
        indiceJugadorActual++;
        if(indiceJugadorActual==jugadores.size())
            indiceJugadorActual=0;
    }
    public OperacionJuego siguientePaso(){
       Jugador jugadorActual=getJugadorActual();
       OperacionJuego operacion=gestor.siguienteOperacion(jugadorActual,estado);
       if(operacion==OperacionJuego.PASAR_TURNO){
           pasarTurno();
           siguientePasoCompletado(operacion);
       }
       if(operacion==OperacionJuego.AVANZAR){
           avanzaJugador();
           siguientePasoCompletado(operacion);
       }
       return operacion;
    }
    
    public void siguientePasoCompletado(OperacionJuego operacion){
        
    }
    
    
    
}
