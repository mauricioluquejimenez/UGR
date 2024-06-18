package civitas;

import java.util.ArrayList;

public class Jugador implements Comparable<Jugador> {
    
    static protected int CasasMax = 4;
    static protected int CasasPorHotel = 4;
    private int casillaActual;
    static protected int HotelesMax=4;
    private String nombre;
    static protected float PasoPorSalida = 1000;
    private boolean puedeComprar;
    private float saldo;
    private static float SaldoInicial = 7500;
    private ArrayList<CasillaCalle> propiedades;
    private Jugador jugador,propietario;
    
    Jugador(){
        this.nombre="";
    }
    Jugador(String nombre){
        this.nombre=nombre;
    }
    int cantidadCasasHoteles(){
        return CasasPorHotel;
    }
    public int compareTo(Jugador otro){
        return 0;
    }
    boolean construirCasa(int ip){
        CasillaCalle propiedad;
        boolean result=false;
        boolean existe=existeLaPropiedad(ip);
        if(existe){
            propiedad=propiedades.get(ip);
            boolean puedoEdificar=puedoEdificarCasa(propiedad);
            float precioEdificar=propiedad.GetPrecioEdificar();
            if(puedoGastar(precioEdificar)&&propiedad.getNumCasas()<getCasasMax())
                puedoEdificar=true;
            if(puedoEdificar){
                propiedad.ConstruirCasa(jugador);
                jugador.paga(precioEdificar);
                propiedad.setNumCasas(propiedad.getNumCasas()+1);
                result=true;
            }
        }
        return result;
    }
    boolean construirHotel(int ip){
        boolean result=false;
        if(existeLaPropiedad(ip)){
            CasillaCalle propiedad=propiedades.get(ip);
            boolean puedoEdificarHotel=puedoEdificarHotel(propiedad);
            puedoEdificarHotel=false;
            float precio=propiedad.GetPrecioEdificar();
            if(puedoGastar(precio)){
                if(propiedad.getNumHoteles()<getHotelesMax()){
                    if(propiedad.getNumCasas()>=getCasasPorHotel())
                        puedoEdificarHotel=true;
                }
            }
            if(puedoEdificarHotel){
                result=propiedad.ConstruirHotel(jugador);
                jugador.paga(propiedad.GetPrecioEdificar());
                result=true;
                propiedad.derruirCasas(CasasPorHotel,jugador);
                Diario.getInstance().ocurreEvento("El jugador " + nombre + " construye hotel en la propiedad " + ip);
            }
        }
        return result;
    }
    boolean enBancarrota(){
        return(saldo<=0);
    }
    boolean existeLaPropiedad(int ip){
        boolean existe=false;
        for(int i=0;i<propiedades.size();i++)
            if(propiedades.get(i).tienePropietario())
                existe=true;
        return existe;
    }
    int getCasasMax(){
        return CasasMax;
    }
    int getCasasPorHotel(){
        return CasasPorHotel;
    }
    int getCasillaActual(){
        return casillaActual;
    }
    int getHotelesMax(){
        return HotelesMax;
    }
    public String getNombre(){
        return nombre;
    }
    float getPremioPasoSalida(){
        return PasoPorSalida;
    }
    protected ArrayList<CasillaCalle> getPropiedades(){
        return propiedades;
    }
    boolean PuedeComprar(){
        return puedeComprar=true;
    }
    public float getSaldo(){
        return saldo;
    }
    protected Jugador(Jugador otro){
        
    }
    boolean modificarSaldo(float cantidad){
        saldo+=cantidad;
        Diario.getInstance().ocurreEvento("El nuevo saldo es " + saldo + " euros");
        return true;
    }
    boolean moverACasilla(int numCasilla){
        casillaActual=numCasilla;
        puedeComprar=false;
        Diario.getInstance().ocurreEvento("La casilla actual del jugador es " + casillaActual);
        return true;
    }
    boolean paga(float cantidad){
        return modificarSaldo(-1*cantidad);
    }
    boolean pagaAlquiler(float cantidad){
        return paga(cantidad);
    }
    boolean pasaPorSalida(){
        return false;  
    }
    boolean puedeComprarCasilla(){
        return puedeComprar=true;
    }
    boolean puedoEdificarCasa(CasillaCalle TituloPropiedad){
        return (PuedeComprar()&&TituloPropiedad.getNumCasas()<CasasMax);
    }
    boolean puedoEdificarHotel(CasillaCalle TituloPropiedad){
        return(PuedeComprar()&&TituloPropiedad.getNumHoteles()<HotelesMax);
    }
    boolean puedoGastar(float precio){
        return(saldo>=precio);
    }
    boolean recibe(float cantidad){
        return modificarSaldo(cantidad);
    }
    boolean tieneAlgoQueGestionar(){
        return(propiedades.size()!=0);
    }
    boolean comprar(CasillaCalle titulo){
        boolean result=false;
        if(PuedeComprar()){
            float precio=titulo.GetPrecioCompra();
            if(puedoGastar(precio)){
                result=titulo.comprar(jugador);
                propietario=jugador;
                propietario.paga(titulo.GetPrecioCompra());
                propiedades.add(titulo);
                Diario.getInstance().ocurreEvento("El jugador " + jugador + "compra la propiedad " + titulo);
                puedeComprar=false;
            }
            Diario.getInstance().ocurreEvento("El jugador " + jugador + "no tiene saldo para comprar la propiedad " + titulo);
        }
        return result;
    }
}
