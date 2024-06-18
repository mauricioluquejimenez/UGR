package civitas;

import java.util.Random;

public class Dado {
    static final private Dado instance = new Dado();
    static final private int VALORDEBUG=1;
    static final private int VALORESDADO=6;
    private int ultimoResultado;
    private boolean debug;
    private Random random;
 
    private Dado(){
        ultimoResultado=0;
        debug=false;
        random = new Random();
    }
    static Dado getInstance() {
        return instance;
    }
    int tirar(){
        if(debug){
            ultimoResultado=VALORDEBUG;
            return 1;
        }
        else{
            int tirada = random.nextInt(VALORESDADO)+VALORDEBUG;
            return tirada;
        }
                    
    }
    int quienEmpieza(int n){
        int jugador=random.nextInt(n);
        return jugador;
    }
    void setDebug(boolean d){
        debug=d;
        if(debug)
            Diario.getInstance().ocurreEvento("Las tiradas son de una unidad ");
        else
            Diario.getInstance().ocurreEvento("Las tiradas pueden ser de más de una unidad");
    }
    int getUltimoResultado(){
        return ultimoResultado;
    }
}
