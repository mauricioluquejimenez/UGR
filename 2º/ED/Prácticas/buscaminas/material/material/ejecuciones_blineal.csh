#!/bin/csh
@ inicio = 100
@ fin = 100000
@ incremento = 1000
set ejecutable = busqueda_lineal
set salida = tiempos_busqueda.dat

@ i = $inicio
echo > $salida
while ( $i <= $fin )
  echo Ejecución tam = $i
  echo `./{$ejecutable} $i 1000000` >> $salida
  @ i += $incremento
end
