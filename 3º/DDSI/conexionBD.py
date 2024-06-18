
#Variables Globales
opcionSeleccionada = 0
inicioMenu = "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Inicio del programa >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n\n" + " 1 -> Borrado y nueva creacion de 10 tuplas en la base de datos \n " + "2 -> Dar de alta un nuevo pedido \n" +" 3 -> Mostrar contenido de las Tablas \n" + " 4 -> Cerrar programa y cerrar conexion a la Base de datos \n\n"
menuDarAltaPedido = "\n\n<<<<<<<<<<< Menu creacion pedido >>>>>>>>>\n\n" + " 1 -> Añadir detalle de producto \n " + "2 -> Eliminar todos los detalles del producto \n" +" 3 -> Cancelar pedido \n" + " 4 -> Finalizar pedido \n\n"
contador = 1

#implementacion de las funciones del caso 2 del menu principal
def switchCaseMenuDarAltaPedido( opcion ):
	if opcion == 1 :	# Añadir detalle de pedido
		cpro = input( "Código del producto:" )
		cant = input( "Cantidad del producto:" )
		ccli = input( "Código del cliente:" )
		global contador
		cped = 30+contador
		
		crsr.execute("SELECT Cantidad FROM Stock WHERE Cproducto='" + cpro + "'")
		ncant = crsr.fetchone()
		value = [x for x in ncant][0]

		int(value)
		if (value > int(cant)):
			crsr.execute("INSERT INTO Pedido(Cpedido, Ccliente, FechaPedido) values (" + str(cped) + "," + str(ccli) +", ? "+")", datetime.date.today())
			contador+=1
			crsr.execute("SAVEPOINT pedido")
			crsr.execute("INSERT INTO DetallePedido (Cpedido, Cproducto, Cantidad) VALUES (" +str(cped) + "," +str(cpro) + "," + str(cant) + ")")

			n = int(cpro)-int(cant)
			crsr.execute("UPDATE Stock SET Cantidad = " + str(n) + "WHERE Cproducto="+cpro)
			mostrarTablas()
		else:
			print("No hay stock")

	elif opcion == 2 :	# Eliminar detalles del producto
		crsr.execute("ROLLBACK TO pedido")
		mostrarTablas()

	elif opcion == 3 :	# Cancelar pedido
		print( "Cancelando pedido..." )
		cnxn.rollback()
		mostrarTablas()

	elif opcion == 4 :	# Finalizar pedido
		print( "Finalizando pedido..." )
		cnxn.commit()


#Implementacion de las funciones 
def switchCaseMenuPrincipal( opcion ):
	if opcion == 1 :
		crearTuplas()

	elif opcion == 2 :
		print( opcion )
		darAltaPedido()

	elif opcion == 3 :
		print( opcion )
		mostrarTablas()

	elif opcion == 4 :
		#Cerramos la conexión con la BD 
		cnxn.close()



def darAltaPedido():
	opcionAltaPedido = 0

	while ( opcionAltaPedido != 4 ) and  (opcionAltaPedido != 3 ):
		print( menuDarAltaPedido )
		opcionAltaPedido = int(input( "Selecciona una opcion de 1 al 4 : " ))

		while (opcionAltaPedido >= 5 or opcionAltaPedido  <= 0 ):
			print( "error, opcion no contemplada." )
			opcionAltaPedido = int(input( "Selecciona una opcion de 1 al 4 :" ))
	
		switchCaseMenuDarAltaPedido( opcionAltaPedido )
		print( opcionAltaPedido )


def crearTablas():
	sql = '''create table Stock(
		Cproducto int, 
		Cantidad int, 
		primary key (Cproducto)
	)'''
	
	#if crsr.tables(table='Stock', tableType='TABLE').fetchone():
	crsr.execute(sql)

	sql = '''create table Pedido(
		Cpedido int, 
		Ccliente int, 
		FechaPedido Date,
		primary key (Cpedido)
	)'''

	#if crsr.tables(table='Pedido', tableType='TABLE').fetchone():
	crsr.execute(sql)

	sql = '''create table DetallePedido(
		Cpedido int, 
		Cproducto int, 
		Cantidad int,
		primary key (Cpedido, Cproducto),
		foreign key (Cpedido) references Pedido(Cpedido),
		foreign key (Cproducto) references Stock(Cproducto)
	)'''

	#if crsr.tables(table='DetallePedido', tableType='TABLE').fetchone():
	crsr.execute(sql)

	crsr.commit()


def crearTuplas():
	borrarTablas()
	crearTablas()
	for i in range(10):
		crsr.execute('insert into Stock values (' + str(i+1) + ','+ str(i+1) + ')')
		crsr.execute('insert into Pedido values (' + str(i+21) + ',' + str(i+1) + ', ? )', datetime.date.today())
		crsr.execute('insert into DetallePedido(Cproducto,Cpedido,Cantidad) values (' + str(i+1) + ','+ str(i+21)  + ',' + str(i + 31 ) + ')')
		crsr.commit()



def mostrarTablas():
	print("Stock:")
	crsr.execute('select * from Stock')

	cont = 1
	while True:
		row = crsr.fetchone()
		if not row:
			break
		print('\tProducto ', cont)
		print('\t\tCodigo del producto:', row[0])
		print('\t\tCantidad:', row[1])
		cont+=1

	print("Pedido:")
	crsr.execute("select * from Pedido")

	cont = 1
	while True:
		row = crsr.fetchone()
		if not row:
			break
		print('\tPedido ', cont)
		print('\t\tCodigo del pedido: ', row[0])
		print('\t\tCodigo del cliente: ', row[1])
		print('\t\tFecha del pedido: ', row[2])
		cont+=1

	print("Detalle-Pedido:")
	crsr.execute("select * from DetallePedido")

	cont = 1
	while True:
		row = crsr.fetchone()
		if not row:
			break
		print('\tDetalle Pedido ', cont)
		print('\t\tCodigo del pedido: ', row[0])
		print('\t\tCodigo del producto: ', row[1])
		print('\t\tCantidad: ', row[2])
		cont+=1

def borrarTablas():
	crsr.execute('drop table DetallePedido')
	crsr.execute('drop table Stock')
	crsr.execute('drop table Pedido')
	crsr.commit()

# Main de la clase

import pyodbc	#Interfaz con la BD
import datetime 

user = 'x0000000'
password = 'x0000000'

user = input("Introduzca el usuario: ")
password = input("Introduzca la contraseña: ")

#Nos conectamos a la BD
#autocommit = false para hacer los rollbacks, savepoints y commits manualmente
cnxn = pyodbc.connect('DRIVER={Devart ODBC Driver for Oracle};Direct=True;Host=oracle0.ugr.es;Service Name=practbd.oracle0.ugr.es;User ID=' + user + ';Password=' + password + '', autocommit=False)

crsr = cnxn.cursor()

try:
	crearTablas()
finally:
	while opcionSeleccionada != 4:
		print( inicioMenu )
		opcionSeleccionada = int( input( "Selecciona una opcion de 1 al 4 :" ))

		while ( opcionSeleccionada >= 5 or opcionSeleccionada  <= 0 ):
			print( "error, opcion no contemplada." )
			opcionSeleccionada = int( input( "Selecciona una opcion de 1 al 4 :" ))
		
		switchCaseMenuPrincipal( opcionSeleccionada )



