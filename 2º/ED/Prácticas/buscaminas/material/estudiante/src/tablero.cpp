
#include <cassert>
#include "../include/tablero.h"
#include "cstdlib" //para random
#include <ctime>
using namespace std;

/* ***************************************** */
// Método privado
void Tablero::copia_tableros(const Tablero &orig)
{
  numfilas = orig.numfilas;
  numcols = orig.numcols;
  numminas = orig.numminas;
  inicio_row = orig.inicio_row;
  inicio_col = orig.inicio_col;	
  
  //reservamos
  //tab = Matriz<int>(numfilas,numcols);
  //cerradas = Matriz<bool>(numfilas,numcols);
  //arcadas = Matriz<bool>(numfilas,numcols);
  //entorno = Matriz<int>(numfilas,numcols);

  tab.Copiar(orig.tab);
  cerradas.Copiar(orig.cerradas);
  marcadas.Copiar(orig.marcadas);
  entorno.Copiar(orig.entorno);
    
  if (orig.TG!=nullptr){
    TG =orig.TG;
    for (int f=0; f<numfilas; f++) {    //  Recorremos todas las casillas
        for (int c=0; c<numcols; c++) {  //
          if (cerradas.get(f,c)==false && tab.get(f,c)!=-1){
                  string aux = to_string(entorno.get(f,c));
	          TG->putTexto(f,c,aux);
	   }       
       }
    }
  }
  else TG=nullptr;
  
}
/* ***************************************** */
// Método privado
void Tablero::borrar_tableros(){
  tab = Matriz<int>();
  cerradas = Matriz<bool>();
  marcadas = Matriz<bool>();
  entorno = Matriz<int>();
} 
	
/* ***************************************** */
void Tablero::contabilizaEntorno(int f,int c){

	for (int i=-1;i<=1;i++)
	 if (f+i>=0 && f+i<numfilas){
	    for (int j=-1;j<=1;j++){
	      if (c+j>=0 && c+j<numcols){
	        if (tab.get(f+i, j+c)==-1)
	          entorno.set(f, c, entorno.get(f, c)+1);
	      }
	    }
	 }        
}
/* ***************************************** */
void Tablero::iniciaEntorno(){
   for (int f=0;f<numfilas;f++)
     for (int c=0;c<numcols;c++)
       entorno.set(f, c, 0);

   for (int f=0;f<numfilas;f++)
     for (int c=0;c<numcols;c++)
         if (tab.get(f, c)!=-1)
               contabilizaEntorno(f,c);
          else entorno.set(f, c, -1);
}          
	
/* ***************************************** */

Tablero::Tablero()
{
  TG=nullptr; //por defecto se crea sin tablero grafico
  // El constructor pone en blanco el tablero
  numfilas=numcols=0;
  numminas =0;
  inicio_row=inicio_col=-1;
  tab=Matriz<int>();
  cerradas=Matriz<bool>();
  entorno=Matriz<int>();
  marcadas=Matriz<bool>();
}


Tablero::Tablero(int filas, int cols,int nminas){
	
   srand (time(NULL));
   numfilas =filas;
   numcols =cols;
   numminas = nminas;

  cerradas = Matriz<bool>(numfilas,numcols);
  tab = Matriz<int>(numfilas,numcols);
  entorno = Matriz<int>(numfilas,numcols);
  marcadas = Matriz<bool>(numfilas,numcols);

  for(int i = 0; i < numfilas; i++) {
      for(int j = 0; j < numcols; j++) {
          cerradas.set(i, j, true); //no esta abierta
          tab.set(i, j, 0); //no contiene mina
          entorno.set(i, j, 0);
          marcadas.set(i, j, false);//no esta marcada
      }
  }
  //colocamos las minas
  for (int cnt=0;cnt<numminas;cnt++){
    int f,c;
    do{
    	f = rand()%numfilas;
    	c = rand()%numcols;
    
    }while (tab.get(f, c)==-1);
    tab.set(f, c, -1);
  
  }
  //colocamos los valores 0 
  /*for (int i=0;i<numfilas;i++)
    for (int j=0;j<numcols;j++){
    	if (tab[i][j]!=-1){

    		tab[i][j]=0;
    	}
   }*/ 		
   
   //Iniciamos la matriz entorno
   iniciaEntorno();
   
    		
  
  //escogemos una fila y columna de inicio
  //escogemos una fila y columna de inicio
  do{
     inicio_row = rand()%numfilas;
     inicio_col = rand()%numcols;	
 	
  }while (tab.get(inicio_row, inicio_col)==-1);
  	  
}
/* ***************************************** */

Tablero::Tablero(const Tablero &orig)
{
  copia_tableros(orig);

}
/* ***************************************** */

Tablero& Tablero::operator=(const Tablero &orig)
{
  if (this!=&orig){
    borrar_tableros();
    copia_tableros(orig);
  }  
  return *this;
}
/* ***************************************** */
void Tablero::iniciaTableros(){
   for (int i=0; i<numfilas; i++){
     for (int j=0;j<numcols; j++){
      cerradas.set(i, j, true); //no esta abierta
	    tab.set(i, j, 0); //no contiene mina
	    entorno.set(i, j, 0);
	    marcadas.set(i, j, false);//no esta marcada
     }
  }
  for (int cnt=0;cnt<numminas;cnt++){
    int f,c;
    do{
    	f = rand()%numfilas;
    	c = rand()%numcols;

    
    }while (tab.get(f, c)==-1);
    tab.set(f, c, -1);
  }
  
  	
   
   //Iniciamos la matriz entorno
   iniciaEntorno();
   cout <<"Iniciado el entorno"<<endl;
  //escogemos una fila y columna de inicio
  do{
     inicio_row = rand()%numfilas;
     inicio_col = rand()%numcols;	
  }while (tab.get(inicio_row, inicio_col)==-1);
	 
}

/* ***************************************** */
bool Tablero::destapaCasilla(int f, int c, bool &estalla){
	assert(f>=0 && f<numfilas && c>=0 && c<numcols);
	if (!cerradas.get(f, c))
		return false;
	
	if (tab.get(f, c)==-1) //es mina
		estalla=true;
	else if (entorno.get(f, c)==0){
	    cout<<"Entorno 0 "<<endl;
            cerradas.set(f, c, false);
            estalla=false;
            abreEntorno(f,c);
            pintaEntorno(f,c);
           }
           else if (entorno.get(f, c)>0){
                 cerradas.set(f, c, false);
                 estalla=false;
                 pintaEntorno(f,c);
          }  
       
       return true;
}
/* ***************************************** */  
bool Tablero::pintaEntorno(int f, int c){
	if (entorno.get(f, c)==0){ //todas las casillas no minadas en el entorno se descubren
	 for (int i=-1;i<=1;i++){
    	  int posf=f+i; 
    	  if (posf>=0 && posf<numfilas){
	    for (int j=-1;j<=1;j++){
	      int posc=c+j;
	      if (posc>=0 && posc<numcols && !(f==posf && c==posc)){
	        if (!cerradas.get(posf, posc) && tab.get(posf, posc)!=-1){
	         string aux = to_string(entorno.get(posf, posc));
	         cout<<"Pintando en "<<posf << " " <<posc <<" " <<aux<<endl;
	         
	      	 TG->putTexto(posf,posc,aux);    
	      	} 
	      
	      }
    	   }
    	 }
      }	  
      string aux ="0";
      TG->putTexto(f,c,aux);
      return true;
     } 
     else if (entorno.get(f, c)>0){
     		string aux =to_string(entorno.get(f, c)); 
     		TG->putTexto(f,c,aux);    
     		return true;
     }		 
     else return false;
} 

/* ***************************************** */  
void Tablero::abreEntorno(int f, int c){
    	for (int i=-1;i<=1;i++){
    	  int posf=f+i; 
    	  if (posf>=0 && posf<numfilas){
	    for (int j=-1;j<=1;j++){
	      int posc=c+j;
	      if (posc>=0 &&posc<numcols){
	        if (cerradas.get(posf, posc) && tab.get(posf, posc)!=-1){
	          
	      	 cerradas.set(posf, posc, false);
	      	} 
	      
	      }
    	   }
    	 }
      }	  
}    
       
    
/* ***************************************** */    
bool Tablero::ponerBandera(int f, int c)
{
  assert((f>=0) && (f<numfilas) && (c>=0) && (c<numcols));
  if (cerradas.get(f, c)){ 

     marcadas.set(f, c, true);
     string aux ="F";
     TG->putTexto(f,c,aux);    
     return true;
  } 
  else  
   return false;
}
/* ***************************************** */
int Tablero::numClose()const{
  int cnt=0;
  for (int i=0;i<numfilas;i++)
    for (int j=0;j<numcols;j++)
     if (cerradas.get(i, j)==true) cnt++;
  return cnt;

}
/* ***************************************** */
int Tablero::numOpen()const{
  int cnt=0;
  for (int i=0;i<numfilas;i++)
    for (int j=0;j<numcols;j++)
     if (cerradas.get(i, j)==false) cnt++;
  return cnt;

}
/* ***************************************** */

bool Tablero::terminado()const
{
  return (numClose()==numminas && numOpen()+numClose()==numfilas*numcols);
}


/**********************************************/
void Tablero::Redraw(){

    if (TG!=nullptr){
        
        for (int i=0; i<numfilas; i++)
            for (int j=0; j<numcols; j++){
		if (cerradas.get(i, j)==false && tab.get(i, j)!=-1){
		 string aux;
		 aux =to_string(entorno.get(i, j)); 	  
                 TG->putTexto(i,j,aux);
               } 
            }
    }
}
/* ***************************************** */

void Tablero::RedrawInicio(){

    if (TG!=nullptr){
        
        for (int i=0; i<numfilas; i++)
            for (int j=0; j<numcols; j++){
                string aux = "  ";  
 		TG->putTexto(i,j,aux);
                TG->putImagen(i,j,"images/B.png");
               } 
   }
    
}
/* ***************************************** */

void Tablero::drawAll()const{
	
    if (TG!=nullptr){
        
        for (int i=0; i<numfilas; i++)
            for (int j=0; j<numcols; j++){
		if (tab.get(i, j)!=-1){
		 string aux;
		 aux =to_string(entorno.get(i, j)); 	  
                TG->putTexto(i,j,aux);
               } 
               else {
                TG->putImagen(i,j,"images/b1.png");
               } 
               
            }
    }
}

/* ***************************************** */

void Tablero::pintaBomba(int f, int c){
	TG->putImagen(f,c,"images/b1.png");
}


/* ***************************************** */

ostream& operator<<(ostream &salida, const Tablero &T)
{
  salida << "   -------------"<<endl;
  salida <<"Donde estan las minas (valor -1) "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.tab.get(i, j)  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información de los entornos "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.entorno.get(i, j)  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información marcadas "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.marcadas.get(i, j)  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información abiertas "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.cerradas.get(i, j)  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  return salida;
}
/* ***************************************** */



