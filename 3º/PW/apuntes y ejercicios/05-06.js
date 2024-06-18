/*
function vectorize()

Recibe un valor de tipo number
Devuelve un valor de tipo number
Aplica la función pasada como argumento a cada elemento de la lista
	1: elevar al cuadrado
	2: cambiar los números pares a 0
Devuelve la lista con los valores cambiados
*/

function vectorize(lista, operacion)
{
    for(let i = 0; i < lista.length; i++)
        lista[i] = operacion(lista[i])

    return lista
}

let lista = [1, 2, 3, 4, 5]
let cuadrado = vectorize(lista, (x) => x * x)
let par = vectorize(lista, (x) => { if (x % 2 == 0) return 0 })

alert(cuadrado)
alert(par)

/* Vector de 10 alturas de persona e informa de cuántos tienen una altura mayor o igual que 1.80m y cuántos menor. */
let alturas = [1.80, 1.55, 1.69, 1.90, 2.05, 1.75, 1.82, 1.68, 1.73, 1.88];
function medir_altura(alturas)
{
    let altos = [];
    let bajos = [];

    for(let i = 0; i < alturas.length(); i++)
        if(alturas[i] * 100 >= 180) altos.push(altos[i]);
        else bajos.push(bajos[i]);

    for(let a = 0; a < altos.length(); a++) document.write(altos[i]);
    for(let b = 0; b < bajos.length(); b++) document.write(bajos[i]);
}

medir_altura(alturas);