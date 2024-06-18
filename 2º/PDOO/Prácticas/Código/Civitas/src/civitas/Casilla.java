package civitas;

import java.util.ArrayList;

public class Casilla {
    
    private MazoSorpresas mazo;
    private Sorpresa sorpresa;
    private ArrayList<Tablero> tablero;
    private Jugador propietario;
    private String Nombre;
    
    
    public Casilla(){
        init();
    }
    public Casilla(String nombre){
        init();
        this.Nombre=nombre;
    }
    
    void init(){
        this.Nombre="";
    }
    String GetNombre(){
        return Nombre;
    }
    void informe(int actual,ArrayList<Jugador> todos){
        Diario.getInstance().ocurreEvento("El jugador " + todos.get(actual) +
                " ha caído en la casilla " + actual);
    }
    void recibeJugador(int iactual,ArrayList<Jugador> todos){
        informe(iactual,todos);
    }
    public String toString(){
        return "Casilla de descanso , con nombre " + Nombre;
    }
    
}
