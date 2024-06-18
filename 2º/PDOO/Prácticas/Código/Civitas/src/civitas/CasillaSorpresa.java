/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package civitas;

import java.util.ArrayList;

/**
 *
 * @author mauri
 */
public class CasillaSorpresa extends Casilla{
    private MazoSorpresas mazo;
    private Sorpresa sorpresa;
    public CasillaSorpresa(String nombre,MazoSorpresas mazo){
        super(nombre);
        this.mazo=mazo;
    }
    void recibeJugador_sorpresa(int iactual,ArrayList<Jugador> todos){
        sorpresa=mazo.siguiente();
        informe(iactual,todos);
        sorpresa.aplicarAJugador(iactual, todos);
    }
}
