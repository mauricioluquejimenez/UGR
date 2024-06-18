package civitas;

public class GestionInmobiliaria {
    private int propiedad;
    private OperacionInmobiliaria operacion;
    
    public GestionInmobiliaria(OperacionInmobiliaria gest, int ip){
        this.operacion=gest;
        this.propiedad=ip;
    }
    OperacionInmobiliaria getOperacion(){
        return operacion;
    }
    int getPropiedad(){
        return propiedad;
    }
}
