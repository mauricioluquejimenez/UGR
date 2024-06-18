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
public class CasillaCalle extends Casilla{
    static private float FACTORALQUILERCALLE=1.0f;
    static private float FACTORALQUILERCASA=1.0f;
    static private float FACTORALQUILERHOTEL=4.0f;    
    private ArrayList<Tablero> tablero;
    private Jugador propietario;
    private String Nombre;
    private float precioCompra;
    private float precioEdificar;
    private float precioBaseAlquiler;
    private int MaxCasas;
    private int numCasas;
    private int numHoteles;
    
    
    public CasillaCalle(){
        super();
    }
    public CasillaCalle(String nombre){
        super(nombre);
    }
    public CasillaCalle(String nombre,float unPrecioCompra,float unPrecioEdificar,float unPrecioAlquilerBase){
        super(nombre);
        this.precioCompra=unPrecioCompra;
        this.precioEdificar=unPrecioEdificar;
        this.precioBaseAlquiler=unPrecioAlquilerBase;
    }
    
    
    void init(){
        this.Nombre="";
        this.precioCompra=0;
        this.precioEdificar=0;
        this.precioBaseAlquiler=0;
        this.MaxCasas=0;
        this.numCasas=0;
        this.numHoteles=0;
    }
    String GetNombre(){
        return Nombre;
    }
    float GetPrecioCompra(){
        return precioCompra;
    }
    float GetPrecioEdificar(){
        return precioEdificar;
    }
    float GetPrecioAlquilerCompleto(){
        return precioBaseAlquiler*(1+numCasas+numHoteles*4);
    }
    void setNumCasas(int numero){
        numCasas=numero;
    }
    int getNumCasas(){
        return numCasas;
    }
    int getNumHoteles(){
        return numHoteles;
    }
    Jugador getPropietario(){
        return propietario;
    }
    boolean ConstruirCasa(Jugador jugador){
        if(numCasas<MaxCasas){
            numCasas++;
            return true;
        }
        else
            return false;
    }
    boolean ConstruirHotel(Jugador jugador){
        numHoteles++;
        return true;
    }
    void informe(int actual,ArrayList<Jugador> todos){
        Diario.getInstance().ocurreEvento("El jugador " + todos.get(actual) +
                " ha caído en la casilla " + actual);
    }
    int cantidadCasasHoteles(){
        return numCasas+numHoteles;
    }
    boolean derruirCasas(int n,Jugador jugador){
        boolean ok=false;
        if(esEsteElPropietario(jugador)&&getNumCasas()>=n){
            numCasas-=n;
            ok=true;
        }
         return ok;   
    }
    boolean tienePropietario(){
        return (propietario!=null);
    }
    boolean esEsteElPropietario(Jugador jugador){
        return(propietario==jugador);
    }
    void tramitarAlquiler(Jugador jugador){
        if(tienePropietario()&&!esEsteElPropietario(jugador)){
            jugador.pagaAlquiler(GetPrecioAlquilerCompleto());
            propietario.recibe(GetPrecioAlquilerCompleto());
        }
    }
    boolean comprar(Jugador jugador){
        return false;
    }
    void recibeJugador_calle(int iactual,ArrayList<Jugador> todos){
        informe(iactual,todos);
        Jugador jugador=todos.get(iactual);
        if(!tienePropietario()){
            jugador.puedeComprarCasilla();
            tramitarAlquiler(jugador);
        }
    }
    
     public String toString(){
        return "Casilla de tipo calle, con nombre " + Nombre + "con unos "
                + "precios de compra, edificación y alquiler de " + precioCompra + ", "
                + precioEdificar + " y " + precioBaseAlquiler + " euros respectivamente.";
    }
}
