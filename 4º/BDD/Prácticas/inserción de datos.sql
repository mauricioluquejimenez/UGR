EXEC ALTA_PRODUCTOR(1, 'Ramón y Cajal 9, Valladolid', '35353535A', 'Justiniano Briñón');
EXEC ALTA_PRODUCTOR(2, 'San Francisco 7, Pamplona', '36363636B', 'Marcelino Peña');
EXEC ALTA_PRODUCTOR(3, 'Antonio Gaudí 23, Barcelona', '37373737C', 'Paloma Riquelme');
EXEC ALTA_PRODUCTOR(4, 'Juan Ramón Jiménez 17, Córdoba', '38383838D', 'Amador Laguna');
EXEC ALTA_PRODUCTOR(5, 'Gran Vía de Colón 121, Madrid', '39393939E', 'Ramón Esteban');
EXEC ALTA_PRODUCTOR(6, 'Cruz de los Ángeles 29, Oviedo', '40404040F', 'Carlota Fuentes');

INSERT INTO sucursal VALUES(1,'Sevilla','Andalucía',1,'Santa Cruz');
INSERT INTO sucursal VALUES(2,'Granada','Andalucía',3,'Palacios Nazaríes');
INSERT INTO sucursal VALUES(3,'Cádiz','Andalucía',5,'Tacita de Plata');
INSERT INTO sucursal VALUES(4,'Madrid','Madrid',7,'Almudena');
INSERT INTO sucursal VALUES(5,'Burgos','Castilla-León',9,'El Cid ');
INSERT INTO sucursal VALUES(6,'Logroño','La Rioja',11,'Catedral del Mar');
INSERT INTO sucursal VALUES(7,'Barcelona','Cataluña',13,'Dama de Elche');
INSERT INTO sucursal VALUES(8,'Alicante','País Valenciano',15,'La Cartuja');
INSERT INTO sucursal VALUES(9,'Palma de Mallorca','Baleares',17,'Meigas');
INSERT INTO sucursal VALUES(10,'La Coruña','Galicia',19,'La Concha');
INSERT INTO sucursal VALUES(11,'San Sebastián','País Vasco',21,'Santa Cruz');
INSERT INTO sucursal VALUES(12,'Oviedo','Asturias',23,'Don Pelayo');

EXEC ALTA_EMPLEADO_1(1,'11111111A','Raúl','21-10-2005',2000,'Sierpes 37, Sevilla',1);
EXEC ALTA_EMPLEADO_1(2,'22222222B','Federico','25-08-2004',1800,'Emperatriz 25, Sevilla',1);
EXEC ALTA_EMPLEADO_1(3,'33333333C','Natalia','30-01-2007',2000,'Ronda 126, Granada',2);
EXEC ALTA_EMPLEADO_1(4,'44444444D','Amalia','13-02-2008',1800,'San Matías 23, Granada',2);
EXEC ALTA_EMPLEADO_1(5,'55555555E','Susana','01-10-2013',2000,'Zoraida 5, Cádiz',3);
EXEC ALTA_EMPLEADO_1(6,'66666666F','Gonzalo','01-01-2002',1800,'Tartesos 9, Cádiz',3);
EXEC ALTA_EMPLEADO_1(7,'77777777G','Agustín','05-05-2014',2000,'Pablo Neruda 84, Madrid',4);
EXEC ALTA_EMPLEADO_1(8,'88888888H','Eduardo','06-06-2014',1800,'Alcalá 8, Madrid',4);
EXEC ALTA_EMPLEADO_1(9,'99999999I','Alberto','05-09-2015',2000,'Las Huelgas 15, Burgos',5);
EXEC ALTA_EMPLEADO_1(10,'10101010J','Soraya','04-10-2012',1800,'Jimena 2, Burgos',5);
EXEC ALTA_EMPLEADO_1(11,'01010101K','Manuel','06-07-2011',2000,'Estrella 26, Logroño',6);
EXEC ALTA_EMPLEADO_1(12,'12121212L','Emilio','05-11-2013',1800,'Constitución 3, Logroño',6);
EXEC ALTA_EMPLEADO_1(13,'13131313M','Patricia','04-12-2014',2000,'Diagonal 132, Barcelona',7);
EXEC ALTA_EMPLEADO_1(14,'14141414N','Inés','07-03-2013',1800,'Colón 24, Barcelona',7);
EXEC ALTA_EMPLEADO_1(15,'15151515O','Carlos','16-06-2014',2000,'Palmeras 57, Alicante',8);
EXEC ALTA_EMPLEADO_1(16,'16161616P','Dolores','14-05-2013',1800,'Calatrava 9, Alicante ',8);
EXEC ALTA_EMPLEADO_1(17,'17171717Q','Elías','13-06-2014',2000,'Arenal 17, P. Mallorca',9);
EXEC ALTA_EMPLEADO_1(18,'18181818R','Concepción','18-08-2015',1800,'Campastilla 14, P. Mallorca',9);
EXEC ALTA_EMPLEADO_1(19,'19191919S','Gabriel','19-09-2010',2000,'Hércules 19, La Coruña',10);
EXEC ALTA_EMPLEADO_1(20,'20202020T','Octavio','20-10-2012',1800,'María Pita 45, La Coruña',10);
EXEC ALTA_EMPLEADO_1(21,'21212121V','Cesar','13-11-2016',2000,'Las Peñas 41, San Sebastián',11);
EXEC ALTA_EMPLEADO_1(22,'23232323W','Julia','24-03-2015',1800,'San Cristóbal 5, San Sebastián',11);
EXEC ALTA_EMPLEADO_1(23,'24242424X','Claudia','13-02-2017',2000,'Santa Cruz 97, Oviedo ',12);
EXEC ALTA_EMPLEADO_1(24,'25252525Z','Mario','23-04-2012',1800,'Naranco 21, Oviedo',12);

EXEC TRASLADAR_EMPLEADO_4(1,1,'Sierpes 37, Sevilla');
EXEC CAMBIAR_DIRECTOR_SUCURSAL_6(1,1);

INSERT INTO esmeralda4.cliente VALUES (1,'Andalucía','Virgen de la Capilla 32, Jaén','26262626A','Hipercor','A');
INSERT INTO esmeralda4.cliente VALUES (2,'Extremadura','San Marcos 41, Cáceres','27272727B','Restaurante Cacereño','C');
INSERT INTO esmeralda3.cliente VALUES (3,'Galicia','San Francisco 37, Vigo','28282828C','Continente','A');
INSERT INTO esmeralda3.cliente VALUES (4,'Asturias','Covadonga 24, Luarca','29292929D','Restaurante El Asturiano','C');
INSERT INTO esmeralda2.cliente VALUES (5,'Baleares','San Lucas 33, Mahón','30303030E','Restaurante El Payés','C');
INSERT INTO esmeralda2.cliente VALUES (6,'País Valenciano','Desamparados 29, Castellón','31313131F','Mercadona','A');
INSERT INTO esmeralda1.cliente VALUES (7,'Castilla-León','Acueducto 1, Segovia','32323232G','Restaurante Cándido','C');
INSERT INTO esmeralda1.cliente VALUES (8,'Castilla-La Mancha','Cervantes 16, Almagro','34343434H','Restaurante Las Vidrieras','C');

INSERT INTO esmeralda1.vino VALUES (1,200,200,1,'Castilla-León',2003,12.5,'Vega Sicilia','Ribera del Duero','Castillo Blanco');
INSERT INTO esmeralda1.vino VALUES (2,100,100,1,'Castilla-León',2010,12.5,'Vega Sicilia','Ribera del Duero','Castillo Blanco');
INSERT INTO esmeralda1.vino VALUES (3,200,200,2,'La Rioja',2014,11,'Marqués de Cáceres','Rioja','Santo Domingo');
INSERT INTO esmeralda1.vino VALUES (4,250,250,2,'La Rioja',2017,11.5,'Marqués de Cáceres','Rioja','Santo Domingo');
INSERT INTO esmeralda2.vino VALUES (5,200,200,3,'Cataluña',2018,11.5,'René Barbier','Penedés','Virgen de Estrella ');
INSERT INTO esmeralda2.vino VALUES (6,250,250,3,'Cataluña',2015,11,'René Barbier','Penedés','Virgen de Estrella ');
INSERT INTO esmeralda3.vino VALUES (7,150,150,4,'Galicia',2019,9.5,'Rias Baixas','Albariño','Santa Compaña');
INSERT INTO esmeralda3.vino VALUES (8,100,100,4,'Galicia',2018,9,'Rias Baixas','Albariño','Santa Compaña');
INSERT INTO esmeralda4.vino VALUES (9,200,200,4,'Andalucía',2013,12,'Córdoba Bella ','Montilla','Mezquita Roja');
INSERT INTO esmeralda4.vino VALUES (10,200,200,4,'Andalucía',2015,12.5,'Tío Pepe','Jerez','Campo Verde');
INSERT INTO esmeralda2.vino VALUES (13,250,250,5,'Murcia',2018,11.5,'Vega Murciana','Jumilla','Vega Verde');
INSERT INTO esmeralda1.vino VALUES (14,300,300,5,'Castilla-La Mancha',2013,11.5,'Tablas de Daimiel','Valdepeñas','Laguna Azul');
INSERT INTO esmeralda3.vino VALUES (15,200,200,6,'Asturias',2018,10,'Santa María','Tierra de Cangas','Monte Astur');
INSERT INTO esmeralda2.vino VALUES (16,250,250,6,'Cataluña',2019,7.5,'Freixenet','Cava','Valle Dorado');
INSERT INTO esmeralda1.vino VALUES (17,200,200,3,'Aragón',2017,10.5,'Estela','Cariñena','San Millán');
INSERT INTO esmeralda4.vino VALUES (18,200,200,5,'Andalucía',2018,13,'Uva dorada','Málaga','Axarquía');
INSERT INTO esmeralda3.vino VALUES (19,250,250,6,'Galicia',2019,8.5,'Meigas Bellas','Ribeiro','Mayor Santiago');
INSERT INTO esmeralda3.vino VALUES (20,300,300,1,'Cantabria',2019,9.5,'Altamira','Tierra de Liébana','Cuevas');
INSERT INTO esmeralda4.vino VALUES (21,300,300,3,'Canarias',2019,10.5,'Virgen negra','Islas Canarias','Guanche');

EXEC ALTA_PEDIDO_8(100, 1, 1, 4, TO_DATE('12-06-2023', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(150, 1, 2, 5, TO_DATE('11-07-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(200, 1, 3, 14, TO_DATE('15-07-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(20, 2, 2, 2, TO_DATE('03-04-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(50, 2, 1, 7, TO_DATE('04-05-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(40, 2, 2, 6, TO_DATE('15-09-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(100, 2, 3, 16, TO_DATE('20-09-23', 'DD-MM-YYYY'));

EXEC ALTA_PEDIDO_8(100, 3, 10, 3, TO_DATE('21-02-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(90, 3, 10, 6, TO_DATE('02-08-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(200, 3, 11, 13, TO_DATE('03-10-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(150, 3, 12, 20, TO_DATE('04-11-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(50, 4, 12, 8, TO_DATE('01-03-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(70, 4, 12, 17, TO_DATE('03-05-23', 'DD-MM-YYYY'));

EXEC ALTA_PEDIDO_8(50, 5, 7, 16, TO_DATE('14-08-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(100, 5, 9, 18, TO_DATE('01-10-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(100, 6, 8, 15, TO_DATE('13-01-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(150, 6, 8, 9, TO_DATE('19-02-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(160, 6, 9, 19, TO_DATE('27-06-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(200, 6, 7, 21, TO_DATE('17-09-23', 'DD-MM-YYYY'));

EXEC ALTA_PEDIDO_8(80, 7, 4, 1, TO_DATE('15-02-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(50, 7, 5, 7, TO_DATE('17-04-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(70, 7, 4, 10, TO_DATE('21-06-23', 'DD-MM-YYYY'));
/*EXEC ALTA_PEDIDO_8(40, 7, 5, 12, TO_DATE('23-07-23', 'DD-MM-YYYY'));*/
EXEC ALTA_PEDIDO_8(50, 8, 6, 14, TO_DATE('11-01-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(60, 8, 6, 4, TO_DATE('14-03-23', 'DD-MM-YYYY'));
EXEC ALTA_PEDIDO_8(70, 8, 4, 6, TO_DATE('21-05-23', 'DD-MM-YYYY'));

EXEC ALTA_DISTRIBUCION_10(1, 4, 4, TO_DATE('13-10-2023', 'DD-MM-YYYY'), 100);
EXEC ALTA_DISTRIBUCION_10(1, 10, 7, TO_DATE('05-05-2023', 'DD-MM-YYYY'), 50);
EXEC ALTA_DISTRIBUCION_10(2, 7, 5, TO_DATE('12-07-2023', 'DD-MM-YYYY'), 150);
EXEC ALTA_DISTRIBUCION_10(2, 5, 2, TO_DATE('04-04-2023', 'DD-MM-YYYY'), 20);
EXEC ALTA_DISTRIBUCION_10(2, 8, 6, TO_DATE('16-09-2023', 'DD-MM-YYYY'), 40);
EXEC ALTA_DISTRIBUCION_10(3, 6, 14, TO_DATE('15-07-2023', 'DD-MM-YYYY'), 200);
EXEC ALTA_DISTRIBUCION_10(3, 9, 16, TO_DATE('21-09-2023', 'DD-MM-YYYY'), 100);
EXEC ALTA_DISTRIBUCION_10(4, 1, 10, TO_DATE('22-06-2023', 'DD-MM-YYYY'), 70);
EXEC ALTA_DISTRIBUCION_10(4, 7, 6, TO_DATE('22-05-2023', 'DD-MM-YYYY'), 70);
EXEC ALTA_DISTRIBUCION_10(5, 10, 7, TO_DATE('18-05-2023', 'DD-MM-YYYY'), 50);
EXEC ALTA_DISTRIBUCION_10(7, 2, 21, TO_DATE('18-09-2023', 'DD-MM-YYYY'), 200);
EXEC ALTA_DISTRIBUCION_10(8, 11, 15, TO_DATE('14-01-2023', 'DD-MM-YYYY'), 100);
EXEC ALTA_DISTRIBUCION_10(8, 2, 9, TO_DATE('20-02-2023', 'DD-MM-YYYY'), 150);
EXEC ALTA_DISTRIBUCION_10(9, 3, 18, TO_DATE('02-10-2023', 'DD-MM-YYYY'), 100);
EXEC ALTA_DISTRIBUCION_10(9, 12, 19, TO_DATE('28-06-2023', 'DD-MM-YYYY'), 160);
EXEC ALTA_DISTRIBUCION_10(10, 4, 3, TO_DATE('22-02-2023', 'DD-MM-YYYY'), 100);
EXEC ALTA_DISTRIBUCION_10(10, 8, 6, TO_DATE('02-08-2023', 'DD-MM-YYYY'), 90);
EXEC ALTA_DISTRIBUCION_10(11, 9, 13, TO_DATE('04-10-2023', 'DD-MM-YYYY'), 200);
/*EXEC ALTA_DISTRIBUCION_10(12, 4, 17, TO_DATE('04-05-2023', 'DD-MM-YYYY'), 70);*/

INSERT INTO esmeralda1.pedido(cod_cli_ped, cod_suc_ped) VALUES (7,1);

DELETE FROM sucursal WHERE cod_suc = 1;

COMMIT;