/**
 * Implementación del diagrama DC - Villanos
 */

package villanos;

import java.util.Date;

class Paso {
    private String descripcion;
    private Date fechaLimite;
    private Boolean terminado = false;
    private int inversion;
    private int ganancia;
    
    //Métodos -> Hemos supuesto que son consultores y modificadores básicos
    //Si no lo fuesen (por ejemplo si el modificador tuviese que comprobar
    //alguna condición), debería indicarse cómo implementarlos en un diagrama
    //de interacción (los estudiaremos más adelante)
    
    public int getInversion(){
        return inversion;
    }
    
    public int getGanancia(){
        return ganancia;
    }
    
    public void setTerminado(Boolean estado){
        terminado = estado;
    }
}
