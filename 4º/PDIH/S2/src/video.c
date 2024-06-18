#include <stdio.h>
#include <dos.h>

/*
* @brief Fijar el modo de vídeo
* @param modo: relación de aspecto de la pantalla
*/
void setvideomode_S2(char modo){
  union REGS inregs, outregs;

  inregs.h.ah = 0x00;
  inregs.h.al = modo;

  int86(0x10, &inregs, &outregs);
}

int main(){
      char modo;

      printf("\n Seleccione el modo de video: ");
	    scanf("%d", &modo);

      setvideomode_S2(modo);
      printf("\n Modo de video: %d", modo);
      printf("\n");

}
