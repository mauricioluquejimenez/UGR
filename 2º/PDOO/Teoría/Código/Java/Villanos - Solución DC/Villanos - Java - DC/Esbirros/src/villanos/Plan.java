
package villanos;

import java.util.ArrayList;
import java.util.Date;

/**
 * Implementación del diagrama DC - Villanos
 */

public class Plan {
   
   //Atributos básicos
   private String nombre;
   private Date fechaConsecucion;
   private int prioridad;
   private int posicionPasoSiguiente = 0;
   private Boolean terminado = false;
   
   //Atributos de referencia
   private ArrayList<Paso> pasos = new ArrayList();
   private ObjetivoMaligno objetivo;
   
   //Métodos
   public Boolean getTerminado(){
       return terminado;
   }
   
   public Boolean estaActivo(){
       return true; //Lo he puesto para que no diese error
   }
   
   public void darPasoSiguiente(){
   }
   
   public int inversionPasoSiguiente(){
       return 0; //Lo he puesto para que no de error
   }
}
