package GUI;

import controladorCivitas.Respuesta;
import civitas.OperacionInmobiliaria;
import civitas.OperacionJuego;

public interface Vista {
    void actualiza();
    public Respuesta comprar();
    public OperacionInmobiliaria elegirOperacion();
    public int elegirPropiedad();
    public void mostrarSiguienteOperacion(OperacionJuego operacion);
    public void mostrarEventos();
    public void pausa();  
   
}
