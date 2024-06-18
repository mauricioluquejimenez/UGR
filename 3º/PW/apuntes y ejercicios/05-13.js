/*
clase vehichulo
    velocidad
    posicion
    moverse() -> posicion + velocidad

clase coche que hereda de vehiculo
    marca
    get_marca() -> "Mi marca es" + marca

vehiculo1
coche1, coche2

Llamar a todos las funciones en todos los objetos
uso de __proto__ y funciones
*/

class vehiculo
{
	constructor(velocidad, posicion)
    {
    	this.velocidad = velocidad
        this.posicion = posicion
    }

    moverse(){this.posicion+=this.velocidad}
}

class coche
{
    constructor(marca)
    {
        this.marca = marca
    }

    get_marca(){ document.write("Mi marca es" + this.marca)}

    get latest()
    {
        return "Mi marca es" + this.marca
    }

}

let vehiculo1 = new vehiculo(120, 60)
let coche1 = new coche("Mercedes")
let coche2 = new coche("Ferrari")
coche1.__proto__ = vehiculo1
coche2.__proto__ = vehiculo1

document.write(vehiculo1.velocidad)
document.write(vehiculo1.posicion)
document.write(vehiculo1.marca)

document.write(coche1.velocidad)
document.write(coche1.posicion)
document.write(coche1.marca)

document.write(coche2.velocidad)
document.write(coche2.posicion)
document.write(coche2.marca)