/**
 * Implementación del diagrama DC - Villanos
 */

package villanos;

public class ObjetivoMaligno {
    
    //Atributos básicos
    private String descripcion;
    
    //Atributos de referencia -> No tiene
    
    //Métodos -> Hemos supuesto que son un consultor y modificador básicos
    //Si no lo fuesen (por ejemplo si el modificador tuviese que comprobar
    //alguna condición), debería indicarse cómo implementarlos en un diagrama
    //de interacción (los estudiaremos más adelante)
    
    public String getDescripcion(){
        return descripcion;
    }
    
    public void setDescripcion(String descripcion){
        this.descripcion = descripcion;
    }
}
