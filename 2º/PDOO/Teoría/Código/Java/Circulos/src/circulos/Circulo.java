package circulos;

public class Circulo {
    
    //atributos y métods de clase
    static private int NC=0;
    
    static public int cuantos_NC(){
        return Circulo.NC;
    }
    
    //atributos y métodos de instancia
    private int radio;
    private int x,
                y;
    
    public Circulo(){
        this(1,0,0);
        //this.radio=1;
        //this.x=0;
        //this.y=0;
    }
           
    public Circulo(int radio,int x,int y){
        this.radio=radio;
        this.x=x;
        this.y=y;
        
        Circulo.NC++;
    }
    public void SetMayorRadio(int r){
        if(r>radio)
            this.radio=r;
    }
    public int GetRadio(){
        return this.radio;
    }
    public void pintar(){
        System.out.println("Círculo de radio " +  radio + " y con centro (" + x + "," + y + ")");
    }
        
}
