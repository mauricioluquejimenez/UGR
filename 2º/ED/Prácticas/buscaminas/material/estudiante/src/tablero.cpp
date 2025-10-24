
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
  tab = new int*[numfilas];
  cerradas = new bool*[numfilas];
  marcadas = new bool*[numfilas];
  entorno = new int *[numfilas];
  for (int i=0; i<numfilas; i++){
       tab[i]= new int[numcols];
       cerradas[i]= new bool[numcols];
       marcadas[i]= new bool[numcols];
       entorno[i] = new int [numcols];
  }   
  for (int i=0; i<numfilas; i++)
    for (int j=0; j<numcols; j++){
      tab[i][j] = orig.tab[i][j];
      cerradas[i][j]= orig.cerradas[i][j];
      marcadas[i][j]= orig.marcadas[i][j];
      entorno[i][j]= orig.entorno[i][j];
    }    
  if (orig.TG!=nullptr){
    TG =orig.TG;
    for (int f=0; f<numfilas; f++) {    //  Recorremos todas las casillas
        for (int c=0; c<numcols; c++) {  //
          if (cerradas[f][c]==false && tab[f][c]!=-1){
                  string aux = to_string(entorno[f][c]);
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
  for (int i=0; i<numfilas; i++){
     delete[]tab[i];
     delete[]cerradas[i];
     delete[]marcadas[i];
  }
  delete[]tab;
  delete[]cerradas;
  delete[]marcadas;
}  
	
/* ***************************************** */
void Tablero::contabilizaEntorno(int f,int c){

	for (int i=-1;i<=1;i++)
	 if (f+i>=0 && f+i<numfilas){
	    for (int j=-1;j<=1;j++){
	      if (c+j>=0 && c+j<numcols){
	        if (tab[f+i][j+c]==-1)
	          entorno[f][c]+=1;
	      }
	    }
	 }        
}
/* ***************************************** */
void Tablero::iniciaEntorno(){
   for (int f=0;f<numfilas;f++)
     for (int c=0;c<numcols;c++)
       entorno[f][c]=0;

   for (int f=0;f<numfilas;f++)
     for (int c=0;c<numcols;c++)
         if (tab[f][c]!=-1)
               contabilizaEntorno(f,c);
          else entorno[f][c]=-1;
}          
	
/* ***************************************** */

Tablero::Tablero()
{
   TG=nullptr; //por defecto se crea sin tablero grafico
  // El constructor pone en blanco el tablero
  numfilas=numcols=0;
  numminas =0;
  inicio_row=inicio_col=-1;
  tab=0;
  cerradas=0;
  entorno=0;
  marcadas=0;

}


Tablero::Tablero(int filas, int cols,int nminas){
	
   srand (time(NULL));
   numfilas =filas;
   numcols =cols;
   numminas = nminas;
  //reservamos
   tab = new int*[numfilas];
   cerradas = new bool*[numfilas];
   marcadas = new bool*[numfilas];
   entorno = new int*[numfilas];
   for (int i=0; i<numfilas; i++){
       tab[i]= new int[numcols];
       cerradas[i]= new bool[numcols];
       marcadas[i]= new bool[numcols];
       entorno[i]= new int[numcols];	
     for (int j=0;j<numcols; j++){
	cerradas[i][j]=true; //no esta abierta
	tab[i][j]=0 ; //no contiene mina
	entorno[i][j]=0;
	marcadas[i][j]=false;//no esta marcada
     }
  }
  //colocamos las minas
  for (int cnt=0;cnt<numminas;cnt++){
    int f,c;
    do{
    	f = rand()%numfilas;
    	c = rand()%numcols;
    
    }while (tab[f][c]==-1);
    tab[f][c]=-1;
  
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
 	
  }while (tab[inicio_row][inicio_col]==-1);
  	  
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
	cerradas[i][j]=true; //no esta abierta
	tab[i][j]=0; //no contiene mina
	entorno[i][j]=0;
	marcadas[i][j]=false;
     }
  }
  for (int cnt=0;cnt<numminas;cnt++){
    int f,c;
    do{
    	f = rand()%numfilas;
    	c = rand()%numcols;

    
    }while (tab[f][c]==-1);
    tab[f][c]=-1;
  
  }
  
  	
   
   //Iniciamos la matriz entorno
   iniciaEntorno();
   cout <<"Iniciado el entorno"<<endl;
  //escogemos una fila y columna de inicio
  do{
     inicio_row = rand()%numfilas;
     inicio_col = rand()%numcols;	
 	
  }while (tab[inicio_row][inicio_col]==-1);
	 
}

/* ***************************************** */
bool Tablero::destapaCasilla(int f, int c, bool &estalla){
	assert(f>=0 && f<numfilas && c>=0 && c<numcols);
	if (!cerradas[f][c])
		return false;
	
	if (tab[f][c]==-1) //es mina
		estalla=true;
	else if (entorno[f][c]==0){
	    cout<<"Entorno 0 "<<endl;
            cerradas[f][c]=false;
            estalla=false;
            abreEntorno(f,c);
            pintaEntorno(f,c);
           }
           else if (entorno[f][c]>0){
                 cerradas[f][c]=false;
                 estalla=false;
                 pintaEntorno(f,c);
          }  
       
       return true;
}
/* ***************************************** */  
bool Tablero::pintaEntorno(int f, int c){
	if (entorno[f][c]==0){ //todas las casillas no minadas en el entorno se descubren
	 for (int i=-1;i<=1;i++){
    	  int posf=f+i; 
    	  if (posf>=0 && posf<numfilas){
	    for (int j=-1;j<=1;j++){
	      int posc=c+j;
	      if (posc>=0 && posc<numcols && !(f==posf && c==posc)){
	        if (!cerradas[posf][posc] && tab[posf][posc]!=-1){
	         string aux = to_string(entorno[posf][posc]);
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
     else if (entorno[f][c]>0){
     		string aux =to_string(entorno[f][c]); 
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
	        if (cerradas[posf][posc] && tab[posf][posc]!=-1){
	          
	      	 cerradas[posf][posc]=false;
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
  if (cerradas[f][c]){ 

     marcadas[f][c]=true;
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
     if (cerradas[i][j]==true) cnt++;
  return cnt;

}
/* ***************************************** */
int Tablero::numOpen()const{
  int cnt=0;
  for (int i=0;i<numfilas;i++)
    for (int j=0;j<numcols;j++)
     if (cerradas[i][j]==false) cnt++;
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
		if (cerradas[i][j]==false && tab[i][j]!=-1){
		 string aux;
		 aux =to_string(entorno[i][j]); 	  
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
		if (tab[i][j]!=-1){
		 string aux;
		 aux =to_string(entorno[i][j]); 	  
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
      salida <<T.tab[i][j]  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información de los entornos "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.entorno[i][j]  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información marcadas "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.marcadas[i][j]  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  salida <<"Información abiertas "<<endl;
  for (int i=0; i<T.numfilas; i++) {
    salida<<"   |";
    for (int j=0; j<T.numcols; j++)
      salida <<T.cerradas[i][j]  << "|";
    salida << endl<<"   -------------"<<endl;
  }
  return salida;
}
/* ***************************************** */



