
En esta práctica se implementa del juego "Cifras y Letras". Para ello, se crean diversas estructuras que almacenan información como letras, palabras, puntuaciones, etc. basándose en las estructuras de datos vistos a lo largo de la asignatura.
La dinámica de ambas partes del juego es idéntica a la del juego original. Para la parte de cifras, hay que tratar de obtener un resultado objetivo realizando operaciones simples (suma, resta, multiplicación y división) sobre un conjunto de números dado, ganando el jugador que se acerque más al resulatdo objetivo.
Para la parte de letras, hay que tratar de formar palabras existentes en el diccionario con un conjunto de letras concreto. Hay dos maneras de decidir al ganador: por la longitud de la palabra formada o por la suma total de sus puntuaciones, ya que en este caso cada letra tiene una puntuación asignada.
Mientras que para la parte de cifras no se han creado nuevas estructuras, para la parte de letras se han creado TDAs tales como Dictionary (representa el diccionario del que se extraen las posibles palabras a formar), LetterSet (contiene todas las letras del abecedario, el número máximo de veces que pueden aparecer en el conjunto de letras disponible y su puntuación correspondiente para el modo de juego por puntuación), y LetterBag (contiene las letras disponibles para formar palabras).
El motor de esta segunda parte para calcular las puntuaciones de las palabrases el TDA Solver, que se encarga de calcular la puntuación de las palabras formadas con las letras disponibles.


#Cifras

Implementa el juego “Las Cifras”, donde se combinan seis números mediante operaciones aritméticas básicas para aproximarse a un número objetivo de 3 cifras.
	
Para representar los distintos estados posibles durante la resolución del juego se utiliza la estructura Estado, que almacena tanto el valor obtenido hasta ese momento como el historial de operaciones realizadas. Esto permite mostrar paso a paso cómo se llega a un resultado.

Se basa en un algoritmo de búsqueda que genera todas las combinaciones posibles de números y operaciones. La función GeneraOperaciones crea nuevas posibilidades aplicando las cuatro operaciones, respetando condiciones como la no-negatividad de la resta o que la división sea exacta. La función Cifras explora todas estas combinaciones y va actualizando constantemente el mejor estado encontrado, de manera que se garantiza obtener la solución más cercana al objetivo, incluso si no se puede alcanzar exactamente.

Para seleccionar los números aleatorios, se usa un multiset en lugar de un set para permitir que los mismos números puedan repetirse en el conjunto de seis números.

En la función principal, main(), se inicializa el conjunto de números disponibles y el objetivo de manera aleatoria, se solicita al jugador que introduzca su intento, y finalmente se ejecuta la búsqueda. Tras completar la exploración, se muestra la mejor solución encontrada por el programa, la diferencia con el objetivo y un registro completo de las operaciones realizadas.


#Dictionary

El diccionario se representa internamente mediante un set de C++, lo que garantiza que las palabras se almacenen de forma ordenada y sin duplicados. La clase proporciona métodos para limpiar el diccionario (clear), obtener el tamaño (size), comprobar la existencia de una palabra (exists) y eliminar palabras (erase). También incluye operadores de entrada y salida para leer o escribir diccionarios completos desde flujos de datos.
GetOcurrences permite conocer cuántas veces aparece un carácter determinado en todo el diccionario, mientras que getTotalLetters devuelve el número total de letras almacenadas. Además, el método getWordsLength permite obtener todas las palabras que tengan una longitud específica.

La implementación se basa en estructuras estándar de C++ como set, vector y string. Esto permite que todas las operaciones de búsqueda, inserción y eliminación sean eficientes, mientras que los iteradores garantizan una navegación segura y cómoda por el diccionario.


#Letters_set

Sirve para almacenar y manejar un conjunto de letras usadas en el juego de “Las Letras”. Cada letra tiene asociada información sobre cuántas veces se puede usar (repetitions) y cuántos puntos vale (score).

La clase permite insertar y eliminar letras, consultar si el conjunto está vacío, limpiar todas las letras o saber cuántas letras hay. También tiene funciones para calcular la puntuación de una palabra sumando los puntos de cada letra, y operadores para acceder a las letras y sus datos de manera sencilla.

Internamente, las letras se almacenan en un map, que permite acceder rápidamente a la información de cada letra. Esto hace que las operaciones sean rápidas y seguras. Por ejemplo, al calcular la puntuación de una palabra, el programa recorre cada letra, la convierte a minúscula y suma los puntos según lo que esté almacenado en el LettersSet.	



#Letters_bag

Representa la bolsa de letras del juego de “Las Letras”. Esta bolsa contiene todas las letras disponibles para jugar y permite sacarlas de manera aleatoria.

La clase puede construirse vacía o a partir de un LettersSet, que indica cuántas veces aparece cada letra en el juego. Al construirla desde un LettersSet, la bolsa se llena automáticamente con todas las repeticiones de cada letra.

Se pueden realizar varias operaciones con la bolsa:

	- Insertar letras de forma individual con insertLetter.

	- Extraer una letra aleatoria con extractLetter. Si la bolsa está vacía, devuelve '\0'. Para mantener eficiencia al eliminar letras en medio del vector, la letra extraída se reemplaza con la última del vector y luego se elimina la última posición.

	- Extraer varias letras con extractLetters, que devuelve un vector con las letras extraídas.

	- Limpiar la bolsa con clear.

	- Consultar cuántas letras quedan con size.

	- Se puede copiar una bolsa a otra usando el operador =.

El uso de vector<char> permite acceder y modificar las letras de forma eficiente, y la semilla aleatoria inicial (srand(time(NULL))) asegura que cada ejecución del programa produzca resultados distintos al extraer letras.	



#Solver

Se encarga de encontrar las mejores palabras posibles en el juego de “Las Letras” a partir de un conjunto de letras disponibles. Esta clase combina la información del diccionario de palabras (Dictionary) con la información de las letras y su puntuación (LettersSet) para determinar qué palabras se pueden formar y cuál tiene la puntuación más alta o la mayor longitud.

Al crear un objeto Solver, se le proporciona un diccionario y un conjunto de letras, que se almacenan internamente para ser usados en la búsqueda de soluciones.

El método principal, getSolutions, recibe las letras disponibles y un parámetro que indica si se quiere calcular la puntuación de las palabras o su longitud. Primero, se calcula la frecuencia de cada letra disponible. Luego, se recorre todo el diccionario y, para cada palabra, se comprueba si se puede formar con las letras disponibles. Si se puede formar, se calcula su puntuación o longitud según corresponda.

La función mantiene un registro de las palabras con mayor puntuación o longitud encontrada. Si se encuentra una palabra con la misma puntuación que la máxima actual, se añade a la lista de soluciones; si se encuentra una mejor, se reemplaza la lista y se actualiza el valor máximo. Al final, devuelve un par: el vector de palabras óptimas y su puntuación o longitud máxima.

La implementación usa arrays para contar frecuencias de letras y recorre el diccionario mediante iteradores constantes, garantizando eficiencia en la búsqueda.




#Cantidad_letras

Este programa calcula un peso estadístico para cada letra del juego de “Las Letras” en función de su frecuencia dentro de un diccionario de palabras.

El programa se ejecuta con tres parámetros: el fichero que contiene el diccionario de palabras, el fichero que contiene el conjunto de letras con sus puntuaciones y repeticiones, y el fichero de salida donde se escribirán los resultados. Primero, se leen ambos ficheros y se cargan en un Dictionary y un LettersSet.

A continuación, el programa calcula cuántas veces aparece cada letra del conjunto dentro del diccionario y determina su frecuencia relativa respecto al total de letras. Se utiliza el logaritmo negativo en base 10 para transformar estas frecuencias en valores de peso, de forma que las letras más raras tengan un peso mayor y las más frecuentes un peso menor.

Finalmente, se escribe en el fichero de salida cada letra junto con su número de apariciones en el diccionario y su peso calculado.




#Letras

El programa permite al jugador generar un conjunto de letras aleatorias a partir de un LettersSet y un LettersBag, y luego intentar formar la mejor palabra posible usando esas letras. El jugador puede elegir entre dos modos de juego: modo Puntuación, donde el objetivo es maximizar los puntos de la palabra según las puntuaciones de cada letra, y modo Longitud, donde el objetivo es formar la palabra más larga posible.

Para validar la palabra introducida por el jugador, el programa verifica que todas las letras usadas estén disponibles en el conjunto generado y que la palabra exista en el diccionario.

Si la palabra es válida, se calcula su puntuación o longitud según el modo de juego. A continuación, el programa utiliza la clase Solver para determinar todas las mejores soluciones posibles con las letras generadas, mostrando al jugador cómo se comparan sus resultados con las óptimas.
