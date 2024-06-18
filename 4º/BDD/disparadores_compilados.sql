--------------------------------------------------------
-- Archivo creado  - martes-diciembre-26-2023   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Trigger BUSCAR_SUCURSAL
--------------------------------------------------------

CREATE OR REPLACE TRIGGER "ESMERALDA1"."BUSCAR_SUCURSAL" 
BEFORE INSERT OR UPDATE OF COD_SUC_EMP ON EMPLEADO1 FOR EACH ROW

DECLARE
sucursal_exists number;

BEGIN
    select cod_suc_emp into sucursal_exists from empleado1 where cod_suc_emp = :new.cod_suc_emp;
    if (sucursal_exists is null) then
        raise_application_error(-20007, 'La sucursal introducida no existe');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."BUSCAR_SUCURSAL" ENABLE;
--------------------------------------------------------
--  DDL for Trigger BUSCAR_VINO
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."BUSCAR_VINO"
BEFORE INSERT OR UPDATE OF COD_VINO_SUM ON SUMINISTRO1 FOR EACH ROW

DECLARE
vino_exists number;

BEGIN
    select cod_vino_sum into vino_exists from suministro1;
    if (:new.cod_vino_sum not in (vino_exists)) then
        raise_application_error(-20007, 'El vino introducido no existe');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."BUSCAR_VINO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger CANTIDAD_VINO
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."CANTIDAD_VINO"
BEFORE INSERT ON SUMINISTRO1 FOR EACH ROW

declare
cantidad_dis number;
cantidad_ped number;

BEGIN
    select sum(can_dis) into cantidad_dis from distribucion1, suministro1 where distribucion1.cod_sum_dis = :new.cod_sum  group by cod_vino_sum;
    select sum(can_ped) into cantidad_ped from suministro1, pedido1 where pedido1.cod_sum_ped = :new.cod_sum group by cod_vino_sum;
    
    if(cantidad_dis > cantidad_ped) then
        raise_application_error(-20007, 'Cantidad de vino distribuida excedida');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."CANTIDAD_VINO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger CLIENTE_DELEGACION
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."CLIENTE_DELEGACION" 
BEFORE INSERT ON PEDIDO1 FOR EACH ROW

declare
ca_cliente varchar(50);
ca_sucursal varchar(50);

begin
    select ca_cli into ca_cliente  from cliente1, pedido1               where :new.cod_cli_ped = cliente1.cod_cli;
    select ca_suc into ca_sucursal from pedido1, sucursal1, suministro1 where :new.cod_sum_ped = suministro1.cod_sum and suministro1.cod_suc_sum = sucursal1.cod_suc;
    
    if((((ca_cliente = 'Castilla-León') or (ca_cliente = 'Castilla-La Mancha') or (ca_cliente = 'Aragón') or (ca_cliente = 'Madrid') or (ca_cliente = 'La Rioja')) and
        ((ca_sucursal  <> 'Castilla-León') and (ca_sucursal  <> 'Castilla-La Mancha') and (ca_sucursal  <> 'Aragón') and (ca_sucursal  <> 'Madrid') and (ca_sucursal  <> 'La Rioja'))) or
        (((ca_cliente = 'Cataluña') or (ca_cliente = 'Baleares') or (ca_cliente = 'País Valenciano') or (ca_cliente = 'Murcia')) and
        ((ca_sucursal   <> 'Cataluña') and (ca_sucursal  <> 'Baleares') and (ca_sucursal  <> 'País Valenciano') and (ca_sucursal  <> 'Murcia'))) and
        (((ca_cliente = 'Galicia') or (ca_cliente = 'Asturias') or (ca_cliente = 'Cantabria') or (ca_cliente = 'País Vasco') or (ca_cliente = 'Navarra')) and
        ((ca_sucursal   <> 'Galicia') and (ca_sucursal  <> 'Asturias') and (ca_sucursal  <> 'Cantabria') and (ca_sucursal  <> 'País Vasco') and (ca_sucursal  <> 'Navarra'))) or
        (((ca_cliente = 'Andalucía') or (ca_cliente = 'Extremadura') or (ca_cliente = 'Canarias') or (ca_cliente = 'Ceuta') or (ca_cliente = 'Melilla')) and
        ((ca_sucursal   <> 'andalucía') and (ca_sucursal  <> 'Extremadura') and (ca_sucursal  <> 'Canarias') and (ca_sucursal  <> 'Ceuta') and (ca_sucursal  <> 'Melilla')))) then
        raise_application_error(-20007, 'No se puede realizar pedidos a la delegación de esta sucursal');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."CLIENTE_DELEGACION" ENABLE;
--------------------------------------------------------
--  DDL for Trigger COMPROBAR_FECHA_DISTRIBUCION
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."COMPROBAR_FECHA_DISTRIBUCION" 
BEFORE INSERT ON DISTRIBUCION1 FOR EACH ROW

DECLARE
fecha date;

BEGIN
  select max(fecha_dis) into fecha from distribucion1, suministro1 group by suministro1.cod_vino_sum;
    if (fecha > :new.fecha_dis) then
        raise_application_error(-20007, 'La fecha del nuevo pedido entre sucursales es incorrecta');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."COMPROBAR_FECHA_DISTRIBUCION" ENABLE;
--------------------------------------------------------
--  DDL for Trigger COMPROBAR_FECHA_PEDIDO
--------------------------------------------------------

 CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."COMPROBAR_FECHA_PEDIDO"
BEFORE INSERT ON PEDIDO1 FOR EACH ROW

declare
fecha date;

begin
    select max(fecha_ped) into fecha from pedido1;
    if (:new.fecha_ped < fecha) then
        raise_application_error(-20007, 'La fecha del nuevo pedido es incorrecta');
    end if;
end;
/
ALTER TRIGGER "ESMERALDA1"."COMPROBAR_FECHA_PEDIDO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DIRECTOR_SUCURSAL
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DIRECTOR_SUCURSAL" 
BEFORE INSERT OR UPDATE ON sucursal1 FOR EACH ROW

declare
director_exists number;

BEGIN
    select count(director_suc) into director_exists from sucursal1, empleado1 where :NEW.cod_suc = empleado1.cod_suc_emp and :NEW.director_suc = empleado1.cod_emp;
    if(director_exists > 0) then
        raise_application_error(-20007, 'El empleado de la compañía no puede ser director de la sucursal porque es empleado de la misma');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."DIRECTOR_SUCURSAL" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DISMINUCION_SALARIO
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DISMINUCION_SALARIO_4" 
BEFORE UPDATE OF SALARIO_EMP ON EMPLEADO4 FOR EACH ROW
BEGIN
  if (:new.salario_emp < :old.salario_emp) then
        raise_application_error(-20007, 'No se permite disminuir el salario del empleado');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA4"."DISMINUCION_SALARIO_4" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ELIMINAR_PRODUCTOR_1
--------------------------------------------------------

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."ELIMINAR_PRODUCTOR"
BEFORE DELETE ON PRODUCTOR1 FOR EACH ROW

DECLARE
total_sum number;

BEGIN
    select count(*) into total_sum from productor1, vino1 where vino1.cod_pro_vino = :old.cod_pro and vino1.can_sum_vino <> 0;
    if (total_sum > 0) then
        raise_application_error(-20007, 'No se puede eliminar al productor');
    end if;
END;
/
ALTER TRIGGER "ESMERALDA1"."ELIMINAR_PRODUCTOR" ENABLE;
--------------------------------------------------------
--  DDL for Trigger MISMA_DELEGACION
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."MISMA_DELEGACION" 
BEFORE INSERT ON DISTRIBUCION1 FOR EACH ROW

declare
ca_compra varchar(50);
ca_vende varchar(50);

begin
    select ca_suc into ca_compra from distribucion1, sucursal1              where :new.cod_suc_dis = sucursal1.cod_suc;
    select ca_suc into ca_vende  from distribucion1, sucursal1, suministro1 where :new.cod_sum_dis = suministro1.cod_sum and suministro1.cod_suc_sum = sucursal1.cod_suc;

    if((((ca_compra = 'Castilla-León') or (ca_compra = 'Castilla-La Mancha') or (ca_compra = 'Aragón') or (ca_compra = 'Madrid') or (ca_compra = 'La Rioja')) and
        ((ca_vende  = 'Castilla-León') and (ca_vende  = 'Castilla-La Mancha') and (ca_vende  = 'Aragón') and (ca_vende  = 'Madrid') and (ca_vende  = 'La Rioja'))) or
        (((ca_compra = 'Cataluña') or (ca_compra = 'Baleares') or (ca_compra = 'País Valenciano') or (ca_compra = 'Murcia')) and
        ((ca_vende   = 'Cataluña') and (ca_vende  = 'Baleares') and (ca_vende  = 'País Valenciano') and (ca_vende  = 'Murcia'))) and
        (((ca_compra = 'Galicia') or (ca_compra = 'Asturias') or (ca_compra = 'Cantabria') or (ca_compra = 'País Vasco') or (ca_compra = 'Navarra')) and
        ((ca_vende   = 'Galicia') and (ca_vende  = 'Asturias') and (ca_vende  = 'Cantabria') and (ca_vende  = 'País Vasco') and (ca_vende  = 'Navarra'))) or
        (((ca_compra = 'Andalucía') or (ca_compra = 'Extremadura') or (ca_compra = 'Canarias') or (ca_compra = 'Ceuta') or (ca_compra = 'Melilla')) and
        ((ca_vende   = 'Andalucía') and (ca_vende  = 'Extremadura') and (ca_vende  = 'Canarias') and (ca_vende  = 'Ceuta') and (ca_vende  = 'Melilla')))) then
        raise_application_error(-20007, 'No se puede realizar pedidos a la delegación de esta sucursal');
    end if;
end;
/

ALTER TRIGGER "ESMERALDA1"."MISMA_DELEGACION" ENABLE;

CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."FECHAS_DISTRIBUCION_PEDIDO"
BEFORE INSERT ON DISTRIBUCION1 FOR EACH ROW

DECLARE
fecha_p date;

BEGIN
    SELECT max(fecha_ped) INTO fecha_p FROM pedido1, suministro1 WHERE pedido1.cod_sum_ped = suministro1.cod_sum AND suministro1.cod_sum = :new.cod_sum_dis;
    IF(fecha_p > :new.fecha_dis) THEN
    raise_application_error(-20007, 'No se puede agregar una distribución posterior al pedido que requiere de esa distribución');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."FECHAS_DISTRIBUCION_PEDIDO" ENABLE;

commit;
