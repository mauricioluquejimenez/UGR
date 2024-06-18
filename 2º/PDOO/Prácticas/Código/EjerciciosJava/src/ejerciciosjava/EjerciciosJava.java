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
public class EjerciciosJava {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Parcela P1=new Parcela("P1",1000,1000,1000,1);
        Parcela P2=new Parcela("P2",2000,2000,2000,2);
        Parcela P3=new Parcela("P3",3000,3000,3000,3);
        
        P1.ConstruirCasa();
        P1.ConstruirHotel();
        P1.ConstruirCasa();
        P2.ConstruirHotel();
        P3.ConstruirCasa();
        P3.ConstruirCasa();
        System.out.println("El precio de la primera parcela es " + P1.GetPrecioAlquilerCompleto());
    }
    
}
