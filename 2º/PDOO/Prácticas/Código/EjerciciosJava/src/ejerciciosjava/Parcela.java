/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ejerciciosjava;

/**
 *
 * @author mauri
 */
public class Parcela {
    private String nombre;
    private float precioCompra,
                  precioEdificar,
                  precioBaseAlquiler;
    private int MaxCasas;
    private int numCasas,
                numHoteles;
    private float FACTORALQUILERCALLE=1.0f;
    private float FACTORALQUILERCASA=1.0f;
    private float FACTORALQUILERHOTEL=4.0f;
                  

    public Parcela(String nombre, float precio_compra, float precio_edificar, float precio_alquiler, int MaxCasas){
        this.nombre=nombre;
        this.precioCompra=precio_compra;
        this.precioEdificar=precio_edificar;
        this.precioBaseAlquiler=precio_alquiler;
        this.MaxCasas=MaxCasas;
        this.numCasas=0;
        this.numHoteles=0;
    }
    String GetNombre(){
        return nombre;
    }
    float GetPrecioCompra(){
        return precioCompra;
    }
    float GetPrecioEdificar(){
        return precioEdificar;
    }
    float GetPrecioAlquilerCompleto(){
        return precioBaseAlquiler*(1+FACTORALQUILERCASA+FACTORALQUILERHOTEL);
    }
    boolean ConstruirCasa(){
        if(numCasas<MaxCasas){
            numCasas+=1;
            return true;
        }
        else
            return false;
    }
    boolean ConstruirHotel(){
        numHoteles+=1;
        return true;
    }
    boolean igualdadIdentidad(Parcela otraParcela){
        if (this==otraParcela);
            return true;
    }
    boolean igualdadEstado(Parcela otraParcela){
        if(this.equals(otraParcela));
            return true;
    }
}