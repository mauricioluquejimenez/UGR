
#include <cassert>
#include "ficha.h"

/* ***************************************** */

std::ostream& operator<<(std::ostream &salida, const Ficha &fic)
{
  assert((fic==BLANCO)||(fic==CIRCULO)||(fic==CRUZ));
  if (fic==BLANCO)
    salida << "   ";
  else if (fic==CIRCULO)
    salida << " O ";
  else   // fic==CRUZ
    salida << " X ";
  return salida;
}

/* ***************************************** */
