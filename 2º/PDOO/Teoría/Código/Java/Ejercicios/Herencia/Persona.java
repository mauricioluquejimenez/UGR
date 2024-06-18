class Persona{
    public String andar(){
        return ("Ando como una persona");
    }
    
    public String hablar(){
        return ("Hablo como una persona");
    }
}

class Profesor extends Persona{
    public String hablar(){
        return ("Hablo como un profesor");
    }
}