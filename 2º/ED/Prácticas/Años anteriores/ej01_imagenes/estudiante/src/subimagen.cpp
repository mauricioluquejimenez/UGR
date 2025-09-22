//
// Created by salma on 11/10/22.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){

    char *fich_orig, *fich_rdo; // nombres de los ficheros
    int fila, col, filas_sub, cols_sub;

    // Comprobar validez de la llamada
    if (argc != 7){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso:  subimagen <FichImagenOriginal> <FichImagenDestino> <fila> <col> <filas_sub> <cols_sub>\n";
        exit (1);
    }

    // Obtener argumentos
    fich_orig  = argv[1];
    fich_rdo = argv[2];
    fila = atoi(argv[3]);
    col = atoi(
            argv[4]);
    filas_sub = atoi(argv[5]);
    cols_sub = atoi(argv[6]);

    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << fich_orig << endl;
    cout << "Fichero resultado: " << fich_rdo << endl;
    cout << "Fila inicial para el recorte: " << fila << endl;
    cout << "Columna inicial para el recorte: " << col << endl;
    cout << "FIlas a recortar: " << filas_sub << endl;
    cout << "COlumnas a recortar: " << cols_sub << endl;

    Image imagen, copia;

    // Leer la imagen del fichero de entrada
    if (!imagen.Load(fich_orig)){
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << fich_orig << ":" << endl;
    cout << "   Imagen   = " << imagen.get_rows()  << " filas x " << imagen.get_cols() << " columnas " << endl;

    //calcular subimagen
    copia = imagen.Crop(fila,col , filas_sub  , cols_sub);

    // Guardar la imagen resultado en el fichero
    if (copia.Save(fich_rdo))
        cout  << "La imagen se guardo en " << fich_rdo << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    return 0;
}
