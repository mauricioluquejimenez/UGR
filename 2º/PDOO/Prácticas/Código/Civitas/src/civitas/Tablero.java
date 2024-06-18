package civitas;
import java.util.ArrayList;

public class Tablero {
    private ArrayList<CasillaCalle> casillas;
    private boolean porSalida;
    
    Tablero(){
        casillas = new ArrayList<>();
        CasillaCalle Salida=new CasillaCalle("Salida");
        casillas.add(Salida);
        
        porSalida=false;
    }
    boolean correcto(int numCasilla){
        return(numCasilla>0);
    }
    boolean computarPasoPorSalida(){
        return porSalida=false;
    }
    void añadeCasilla(CasillaCalle casilla){
        casillas.add(casilla);
    }
    CasillaCalle getCasilla(int numCasilla){
        if(correcto(numCasilla))
            return casillas.get(numCasilla);
        else
            return null;
    }
    ArrayList<CasillaCalle> getCasillas(){
        return casillas;
    }
    int nuevaPosicion(int actual,int tirada){
        int nueva=actual+tirada;
        if(nueva==casillas.size()){
            nueva%=casillas.size();
            porSalida=true;
        }
        return nueva;
    }
}
