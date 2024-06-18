package GUI;

import java.util.ArrayList;
import civitas.CivitasJuego;
import controladorCivitas.Controlador;

public class testP5 {        
    
    public static void main(String[] args){
        CivitasView cw=new CivitasView();
        CapturaNombres cn=new CapturaNombres(cw,true);
        ArrayList<String> nombres=new ArrayList<>();
        nombres=cn.getNombres();
        CivitasJuego cj=new CivitasJuego(nombres,true);
        Controlador c=new Controlador(cj,cw);
        cw.setCivitasJuego(cj);
    }
    
}
