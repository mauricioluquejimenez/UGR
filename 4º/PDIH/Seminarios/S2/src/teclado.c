#include <stdio.h>
#include <dos.h>

/*
* @brief Leer carácter por pantalla
* return outregs.h.al: registro que contiene el carácter leído por teclado
*/
int getchar_S2(){
  union REGS inregs, outregs;

  inregs.h.ah = 1;
  int86(0x21, &inregs, &outregs);

  return outregs.h.al;
}

/*
* @brief Escribir un carácter en pantalla
* @param c: carácter a mostrar por pantalla
*/
void putchar_S2(char c){
  union REGS inregs, outregs;

  inregs.h.ah = 2;
  inregs.h.dl = c;

  int86(0x21, &inregs, &outregs);
}

int main(){
  char c;

  printf("\n Introduzca un caracter: ");
  c = getchar_S2();

  printf("\n Ha pulsado: ");
  putchar_S2(c);
}
