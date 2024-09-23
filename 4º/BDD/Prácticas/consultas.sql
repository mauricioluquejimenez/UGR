--- 1. Listar los clientes (nombre y direcci�n) de Andalucia o Castilla-La Mancha y
--- las  sucursales (nombre y ciudad), a los que se le ha suministrado vino de marca
--- Tablas  de Daimiel entre el 1 de Enero de 2023 y el 1 de septiembre de 2023.
    
SELECT c.nombre_cli, c.direccion_cli, s.nombre_suc, s.ciudad_suc
FROM clientes c, pedidos p, vinos v, sucursales s
WHERE (c.cod_cli = p.cod_cli_ped)
    AND (p.cod_suc_ped = s.cod_suc)
    AND (p.cod_vino_ped = v.cod_vino)
    AND (c.ca_cli = 'Andaluc�a' OR c.ca_cli = 'Castilla-La Mancha')
    AND (v.marca_vino = 'Tablas de Daimiel')
    AND (p.fecha_ped < TO_DATE('01-01-2023', 'DD-MM-YYYY') AND p.fecha_ped < TO_DATE('01-07-2023', 'DD-MM-YYYY'));
    
--- 2. Dado por teclado el código de un productor: “Listar la marca
--- el año de cosecha de cada uno de los vinos producidos por él y
--- la cantidad total suministrada de cada uno  de ellos a clientes de
--- las comunidades autónomas de Baleares, Extremadura o País  Valenciano*/

ACCEPT p_cod_pro NUMBER PROMPT 'Introduce el c�digo del productor: '

SELECT v.marca_vino, v.cosecha_vino, sum(p.can_ped) AS "CANTIDAD TOTAL"
FROM pedidos p, clientes c, vinos v
WHERE (c.cod_cli = p.cod_cli_ped)
    AND v.cod_vino = p.cod_vino_ped
    AND c.ca_cli IN ('Baleares', 'Extremadura', 'Pa�s Valenciano')
    AND v.cod_pro_vino = &p_cod_pro
GROUP BY v.marca_vino, v.cosecha_vino;

--- 3. Dado por teclado el código de una sucursal listar el nombre de cada uno de
--- sus cliente,su tipo y la cantidad total de vino de Rioja o Albariño que se
--- le ha suministrado a cada uno de ellos (solamente deberán aparecer aquellos
--- clientes a los que se les ha suministrado vinos con esta denominación de origen)

ACCEPT s_cod_suc NUMBER PROMPT 'Introduce el c�digo de la sucursal: '

SELECT c.nombre_cli, c.tipo_cli, sum(p.can_ped) AS "CANTIDAD TOTAL"
FROM clientes c, pedidos p, vinos v
WHERE (c.cod_cli = p.cod_cli_ped)
    AND (p.cod_vino_ped = v.cod_vino)
    AND (v.origen_vino IN ('Rioja','Albari�o'))
    AND (p.cod_suc_ped = &s_cod_suc)
GROUP BY c.nombre_cli, c.tipo_cli;
