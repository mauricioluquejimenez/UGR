package controladorCivitas;
import civitas.CivitasJuego;
import civitas.OperacionJuego;
import GUI.Vista;
import GUI.CivitasView;
import civitas.OperacionInmobiliaria;
import civitas.GestionInmobiliaria;


public class Controlador {
    CivitasJuego juego;
    Vista vista;
    
    public Controlador(CivitasJuego juego,CivitasView vista){
        this.juego=juego;
        this.vista=vista;
    }
    public void juega(){
        boolean fin=false;
        while(!fin){
            vista.actualiza();
            vista.pausa();
            OperacionJuego operacion=juego.siguientePaso();
            vista.mostrarSiguienteOperacion(operacion);
            if(operacion!=OperacionJuego.PASAR_TURNO)
                vista.mostrarEventos();
            fin=juego.finalDelJuego();
            if(!fin){
                if(operacion==OperacionJuego.COMPRAR){
                    if(vista.comprar()==Respuesta.SI)
                        juego.comprar();
                    juego.siguientePasoCompletado(operacion);
                }
                if(operacion==OperacionJuego.GESTIONAR){
                    OperacionInmobiliaria gestion=vista.elegirOperacion();
                    int ip=-1;
                    if(OperacionInmobiliaria.TERMINAR!=gestion)
                        ip=vista.elegirPropiedad();
                    GestionInmobiliaria operacionInmobiliaria=new GestionInmobiliaria(gestion,ip);
                    switch(gestion){
                        case TERMINAR:
                            juego.siguientePasoCompletado(operacion);
                            break;
                        case CONSTRUIR_CASA:
                            juego.ConstruirCasa(ip);
                            break;
                        case CONSTRUIR_HOTEL:
                            juego.ConstruirHotel(ip);
                            break;
                    }
                }
            } 
        }
        vista.actualiza();
    }
}