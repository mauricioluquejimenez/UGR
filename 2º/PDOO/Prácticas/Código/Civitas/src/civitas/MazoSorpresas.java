package civitas;

import java.util.ArrayList;
import java.util.Collections;

public class MazoSorpresas {
    private ArrayList<Sorpresa> sorpresas;
    private boolean barajada;
    private boolean debug;
    private int usadas;
    
    
    private void init(){
        sorpresas = new ArrayList<>();
        barajada=false;
        usadas=0;
    }
    MazoSorpresas(){
        debug=false;
        init();
    }
    MazoSorpresas(boolean d){
        debug=d;
        init();
        if(debug)
            Diario.getInstance().ocurreEvento("Las tiradas son de una unidad");
    }
    void alMazo(Sorpresa s){
        if(!barajada)
            sorpresas.add(s);
    }
    Sorpresa siguiente(){
        Sorpresa auxiliar=new Sorpresa();
        Sorpresa sorpresa_final=new Sorpresa();
        if(!barajada||usadas==sorpresas.size()){
            if(!debug){
                Collections.shuffle(sorpresas);
                usadas=0;
                barajada=true;
                usadas++;
            }
            auxiliar=sorpresas.get(0);
            for(int i=0;i<sorpresas.size();i++)
                sorpresas.set(i,sorpresas.get(i+1));
            sorpresas.set(sorpresas.size()-1,auxiliar);
            sorpresa_final=sorpresas.get(sorpresas.size()-1);    
        }
        return sorpresa_final;
    }
}
