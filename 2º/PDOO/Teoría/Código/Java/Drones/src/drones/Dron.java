package drones;


class Dron {
    private int id;
    private float altura;
    private Controlador controlador;
    private Lugar posicion;
    
    public int getId(){
        return id;
    }
    
    public float getAltura(){
        return altura;
    }
    
    public Lugar getPosicion(){
        return null; //Lo he indicado para evitar el mensaje de error
    }
    
    public void navegar(Lugar destino){
        
    }
    
    public void aterrizar(){
        controlador = null; posicion=null;
    }
    
}