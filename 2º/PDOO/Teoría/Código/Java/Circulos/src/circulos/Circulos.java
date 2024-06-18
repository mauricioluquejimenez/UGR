package circulos;
import java.util.Scanner;

public class Circulos {
    public static void main(String[] args){
        Circulo c1=new Circulo(2,-1,1);
        Circulo c2=new Circulo(5,3,4);
        Circulo c3=new Circulo();
        c1.pintar();
        c2.pintar();
        c3.pintar();
        
        System.out.println("Seleccione un nuevo radio: ");
        int nuevo_radio;
        Scanner entradaTeclado = new Scanner (System.in);
        nuevo_radio = entradaTeclado.nextInt ();
        
        System.out.println("Seleccione un círculo al que aplicarle el nuevo radio: ");
        int nuevo_c;
        Scanner entradaTeclado2 = new Scanner (System.in);
        nuevo_c = entradaTeclado2.nextInt ();
        
        if(nuevo_c==1)
            c1.SetMayorRadio(nuevo_radio);
            else if(nuevo_c==2)
                c2.SetMayorRadio(nuevo_radio);
                else if(nuevo_c==3)
                    c3.SetMayorRadio(nuevo_radio);
        
        
      c1.pintar();
      c2.pintar();
      c3.pintar();  
    }
}
