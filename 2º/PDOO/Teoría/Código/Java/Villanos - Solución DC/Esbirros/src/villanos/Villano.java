/**
 * Implementación del diagrama DC - Esbirros 
 * (Ver material complementario de la unidad 6)
 */
package villanos;

import esbirros.Mayordomo;
import java.util.ArrayList;

public class Villano {
    
    //Atributos básicos
    private String nombre;
    String esconditeSecreto; //Visibilidad por defecto de paquete
    private int fortuna;
    
    //Atributos de referencia
    ArrayList<Plan> planes = new ArrayList();
    Mayordomo mayordomo; //Ojo que para hacer esto hemos necesitado
     //hacer previamente un import de la clase Mayordomo, pues se encuentra
     //en otro paquete. Otra alternativa habría sido no hacer el import
     //y cada vez que se use "Mayordomo" indicar: "esbirros.Mayordomo"
    
    //Métodos
    public void setFortuna(int cantidad){
    }
    
    public Boolean seguirPlanPrioritario(){
        return true; //Lo he puesto únicamente para que no de error
    }
    
    public Plan primerPlanActivo(){
        return null; //Lo he puesto únicamente para que no de error
    }
    
}
