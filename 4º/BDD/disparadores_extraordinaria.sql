--------------------------------------------------------
-- Archivo creado  - viernes-febrero-02-2024   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Trigger ALMACEN_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."ALMACEN_EXISTENTE" BEFORE INSERT ON ALMACEN FOR EACH ROW
DECLARE
	cont_alm NUMBER(2);
BEGIN
  IF INSERTING THEN
    SELECT COUNT(*) INTO cont_alm FROM ALMACENES
      WHERE cod_vino_alm = :NEW.cod_vino_alm
        AND cod_suc_alm = :NEW.cod_suc_alm;
    IF (cont_alm>0) THEN 
      RAISE_APPLICATION_ERROR(-30014,'Error hay un vino almacenado con ese codigo y e una sucursal ya existente.');
    END IF;
  END IF;
  IF (UPDATING('cod_vino_alm') OR UPDATING('cod_suc_alm'))THEN
    RAISE_APPLICATION_ERROR(-30015,'Error no se puede modificar el cogigo de un vino almacenado en una sucursal.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."ALMACEN_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger CANTIDAD_DISTRIBUCION_18
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."CANTIDAD_DISTRIBUCION_18" 
BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW

DECLARE
    cantidad_pedidos NUMBER;
BEGIN
    SELECT SUM(can_ped) INTO cantidad_pedidos FROM pedidos WHERE cod_vino_ped = :NEW.cod_vino_dis AND cod_suc_ped = :NEW.cod_suc_compra_dis GROUP BY cod_vino_ped, cod_suc_ped;
    
    IF :NEW.can_dis IS NULL OR :NEW.can_dis < 0 OR :NEW.can_dis > cantidad_pedidos THEN
        RAISE_APPLICATION_ERROR(-20018, 'La cantidad total distribuida entre sucursales no puede exceder la solicitada por los clientes');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."CANTIDAD_DISTRIBUCION_18" ENABLE;
--------------------------------------------------------
--  DDL for Trigger CLIENTE_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."CLIENTE_EXISTENTE" BEFORE INSERT OR UPDATE ON CLIENTE FOR EACH ROW
DECLARE
	cont_cli NUMBER;
BEGIN
  IF (INSERTING) THEN 
    SELECT COUNT(cod_cli) INTO cont_cli
      FROM clientes
      WHERE cod_cli = :NEW.cod_cli;
    IF (cont_cli>0) THEN 
      RAISE_APPLICATION_ERROR(-30022,'Error el cliente ya existe.');
    END IF;
  END IF;
  IF (UPDATING('cod_cli')) THEN
    RAISE_APPLICATION_ERROR(-30023, 'Error no se puede modificar el codigo de un cliente');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."CLIENTE_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger CLIENTE_REALIZA_PEDIDO
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."CLIENTE_REALIZA_PEDIDO" BEFORE INSERT OR UPDATE ON Pedido FOR EACH ROW
DECLARE
  cont_cli NUMBER;
BEGIN
  SELECT COUNT(*) INTO cont_cli
    FROM clientes
    WHERE cod_cli = :NEW.cod_cli_ped;
  IF (cont_cli = 0) THEN 
    RAISE_APPLICATION_ERROR(-30290, 'No existe el cliente que ha reliazado el pedido.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."CLIENTE_REALIZA_PEDIDO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DELEGACION_DISTRIBUCION_17
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DELEGACION_DISTRIBUCION_17" 
BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW

DECLARE
    ca_compra VARCHAR(50);
    ca_vende VARCHAR(50);
    delegacion_compra NUMBER;
    delegacion_vende NUMBER;
BEGIN
    SELECT ca_suc INTO ca_compra FROM sucursales WHERE cod_suc = :NEW.cod_suc_compra_dis;
    SELECT ca_suc INTO ca_vende  FROM sucursales WHERE cod_suc = :NEW.cod_suc_vende_dis;
    
    delegacion_compra := DELEGACION(ca_compra);
    delegacion_vende  := DELEGACION(ca_vende);
    
    IF delegacion_compra = delegacion_vende THEN
        RAISE_APPLICATION_ERROR(-20017, 'Una sucursal no puede realizar pedidos a sucursales de su misma delegación');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."DELEGACION_DISTRIBUCION_17" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DIRECTOR_DE_UNA_SUCURSAL_4
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DIRECTOR_DE_UNA_SUCURSAL_4" BEFORE INSERT OR UPDATE ON sucursal FOR EACH ROW
DECLARE
	cont_emp NUMBER;
BEGIN
	SELECT COUNT(*) INTO cont_emp FROM EMPLEADOS
		WHERE cod_emp = :NEW.director_suc;
	IF (cont_emp>1) THEN  
		RAISE_APPLICATION_ERROR(-3035,'El empleado ya es director de otra sucursal.');
	END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."DIRECTOR_DE_UNA_SUCURSAL_4" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DIRECTOR_ES_EMPLEADO_3
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DIRECTOR_ES_EMPLEADO_3" BEFORE INSERT OR UPDATE ON sucursal FOR EACH ROW
DECLARE
	cont_emp NUMBER;
BEGIN
    IF (INSERTING OR UPDATING('director_suc')) THEN  
      IF (:NEW.director_suc IS NOT NULL) THEN 
        SELECT COUNT(*) INTO cont_emp FROM empleados       
          WHERE cod_emp = :NEW.director_suc;
        IF (cont_emp = 0) THEN 
          RAISE_APPLICATION_ERROR(-3034,'El director no es un empleado.');
        END IF;
      END IF;
    END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."DIRECTOR_ES_EMPLEADO_3" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DISMINUIR_SALARIO_6
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DISMINUIR_SALARIO_6" BEFORE UPDATE ON empleado FOR EACH ROW
DECLARE

BEGIN
  IF UPDATING('salario_emp') THEN 
    IF (:OLD.salario_emp > :NEW.salario_emp ) THEN 
      RAISE_APPLICATION_ERROR(-20001,'NO se puede disminuir el salario de un empleado');
    END IF;
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."DISMINUIR_SALARIO_6" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DISTRIBUCION_DIRECTA_19
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DISTRIBUCION_DIRECTA_19" 
BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW

DECLARE
    delegacion_suc_vende NUMBER;
    delegacion_vino NUMBER;
    ca_sucursal VARCHAR(50);
    ca_vino VARCHAR(50);
BEGIN
    SELECT ca_vino INTO ca_vino FROM vinos WHERE cod_vino = :NEW.cod_vino_dis;
    SELECT ca_suc INTO ca_sucursal FROM sucursales WHERE cod_suc = :NEW.cod_suc_vende_dis;
    
    delegacion_vino := DELEGACION(ca_vino);
    delegacion_suc_vende := DELEGACION(ca_sucursal);
    
    IF delegacion_vino IS NULL OR delegacion_suc_vende IS NULL THEN
        RAISE_APPLICATION_ERROR(-20019, 'La sucursal o el vino solicitados no existen'); 
    ELSE
        IF delegacion_vino <> delegacion_suc_vende THEN
            RAISE_APPLICATION_ERROR(-20019, 'Esta sucursal no distribuye directamente este vino');
        END IF;
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."DISTRIBUCION_DIRECTA_19" ENABLE;
--------------------------------------------------------
--  DDL for Trigger DISTRIBUCION_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."DISTRIBUCION_EXISTENTE" BEFORE INSERT ON DISTRIBUCION FOR EACH ROW
DECLARE
	cont_dis NUMBER(2);
BEGIN
  IF (INSERTING) THEN 
    SELECT COUNT(*) INTO cont_dis FROM DISTRIBUCION
      WHERE cod_vino_dis = :NEW.cod_vino_dis
        AND cod_suc_compra_dis = :NEW.cod_suc_compra_dis
        AND cod_suc_vende_dis = :NEW.cod_suc_vende_dis
        AND fecha_dis= :NEW.fecha_dis;
    IF (cont_dis>0) THEN 
      RAISE_APPLICATION_ERROR(-30016,'Error solicitud a una sucursal exiteste.');
    END IF;
  END IF;
  IF (UPDATING('cod_suc_compra_dis') OR UPDATING('cod_suc_vende_dis') OR UPDATING('fecha_dis') OR UPDATING('cod_vino_dis'))THEN
    RAISE_APPLICATION_ERROR(-30017,' Error no se puede modificar el codigo de una distribución.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."DISTRIBUCION_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ELIMINAR_DIRECTOR
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."ELIMINAR_DIRECTOR" BEFORE DELETE ON EMPLEADO FOR EACH ROW
DECLARE
  cont_emp NUMBER;
BEGIN
  SELECT COUNT(*) INTO cont_emp
    FROM sucursales
    WHERE director_suc = :OLD.cod_emp;
  IF (cont_emp > 0) THEN 
    RAISE_APPLICATION_ERROR(-30280,'No se puede borrar un empleado si es el director de la sucursal.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."ELIMINAR_DIRECTOR" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ELIMINAR_PRODUCTOR_16
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."ELIMINAR_PRODUCTOR_16" 
BEFORE DELETE ON PRODUCTOR FOR EACH ROW

DECLARE
    cantidad_pedidos_vino NUMBER;
    cantidad_distribuciones_vino NUMBER;
    
    CURSOR vinos_producidos IS SELECT cod_vino FROM vinos WHERE cod_pro_vino = :OLD.cod_pro;
    cod_vino_producido NUMBER;
BEGIN
    OPEN vinos_producidos;
    
    FOR i IN vinos_producidos LOOP
        FETCH vinos_producidos INTO cod_vino_producido;
        
        SELECT COUNT (*) INTO cantidad_pedidos_vino FROM pedidos WHERE cod_vino_ped = cod_vino_producido;
        SELECT COUNT (*) INTO cantidad_distribuciones_vino FROM distribucion WHERE cod_vino_dis = cod_vino_producido;
        
    END LOOP;
    
    IF (cantidad_pedidos_vino > 0 OR cantidad_distribuciones_vino > 0) THEN
        RAISE_APPLICATION_ERROR(-20016, 'No se puede eliminar el productor: tiene vinos suministrados');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."ELIMINAR_PRODUCTOR_16" ENABLE;
--------------------------------------------------------
--  DDL for Trigger ELIMINAR_VINO_15
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."ELIMINAR_VINO_15" 
BEFORE DELETE ON vino FOR EACH ROW

DECLARE
    cantidad_pedidos_vino NUMBER;
    cantidad_distribuciones_vino NUMBER;

BEGIN
    SELECT COUNT (*) INTO cantidad_pedidos_vino         FROM pedidos        WHERE cod_vino_ped = :OLD.cod_vino;
    SELECT COUNT (*) INTO cantidad_distribuciones_vino  FROM distribucion   WHERE cod_vino_dis = :OLD.cod_vino;
    
    IF (cantidad_pedidos_vino > 0 OR cantidad_distribuciones_vino > 0) THEN
        RAISE_APPLICATION_ERROR(-20015, 'No se puede eliminar el vino: ha sido o está siendo suministrado');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."ELIMINAR_VINO_15" ENABLE;
--------------------------------------------------------
--  DDL for Trigger EMPLEADO_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."EMPLEADO_EXISTENTE" BEFORE INSERT OR UPDATE ON EMPLEADO FOR EACH ROW
DECLARE
	cont_emp NUMBER(2);
BEGIN
  IF (INSERTING) THEN 
    SELECT COUNT(cod_emp) INTO cont_emp
      FROM EMPLEADOS
      WHERE cod_emp = :NEW.cod_emp;
    IF (cont_emp>0) THEN  
      RAISE_APPLICATION_ERROR(-3024,'Error el empleado ya existe.');
    END IF;
  END IF;
  IF (UPDATING('cod_emp')) THEN
    RAISE_APPLICATION_ERROR(-3025, 'Errro no se puede modificar el codigo del empleado');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."EMPLEADO_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger EXISTE_SUC_COMPRA_DIS
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."EXISTE_SUC_COMPRA_DIS" BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW
DECLARE
  cont_suc_compra NUMBER;
BEGIN
  SELECT COUNT(*) INTO cont_suc_compra
    FROM sucursales
    WHERE cod_suc = :NEW.cod_suc_compra_dis;
  IF (cont_suc_compra = 0) THEN 
    RAISE_APPLICATION_ERROR(-3032, 'No existe la sucursal de compra.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."EXISTE_SUC_COMPRA_DIS" ENABLE;
--------------------------------------------------------
--  DDL for Trigger EXISTE_SUC_PEDIDO
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."EXISTE_SUC_PEDIDO" BEFORE INSERT OR UPDATE ON PEDIDO FOR EACH ROW
DECLARE
  cont_suc NUMBER(8);
BEGIN
  SELECT COUNT(*) INTO cont_suc
    FROM sucursales
    WHERE cod_suc = :NEW.cod_suc_ped;
  IF (cont_suc = 0) THEN  
    RAISE_APPLICATION_ERROR(-3031, 'No existe la sucursal a la que se hace el pedido.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."EXISTE_SUC_PEDIDO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger EXISTE_SUC_VENDE_DIS
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."EXISTE_SUC_VENDE_DIS" BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW
DECLARE
  cont_suc_vende_dis NUMBER;
BEGIN
  SELECT COUNT(*) INTO cont_suc_vende_dis
    FROM SUCURSALES
    WHERE cod_suc = :NEW.cod_suc_vende_dis;
  IF (cont_suc_vende_dis = 0) THEN 
    RAISE_APPLICATION_ERROR(-3033, 'No existe la sucursal de venta.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."EXISTE_SUC_VENDE_DIS" ENABLE;
--------------------------------------------------------
--  DDL for Trigger EXISTE_VINO_PEDIDO
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."EXISTE_VINO_PEDIDO" BEFORE INSERT OR UPDATE ON PEDIDO FOR EACH ROW
DECLARE
  cont_vino NUMBER;
BEGIN
  SELECT COUNT(*) INTO cont_vino
    FROM vinos
    WHERE cod_vino = :NEW.cod_vino_ped;
  IF (cont_vino = 0) THEN 
    RAISE_APPLICATION_ERROR(-3030, 'No existe el vino del pedido.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."EXISTE_VINO_PEDIDO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger FECHA_DISTRIBUCION_20
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."FECHA_DISTRIBUCION_20" 
BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW

DECLARE
    distribuciones NUMBER;
    fecha_distribucion DATE DEFAULT NULL;

BEGIN
    IF INSERTING THEN
        SELECT MAX(fecha_dis) INTO fecha_distribucion FROM distribucion WHERE cod_vino_dis = :NEW.cod_vino_dis AND cod_suc_compra_dis = :NEW.cod_suc_compra_dis AND cod_suc_vende_dis = :NEW.cod_suc_vende_dis;
        IF fecha_distribucion IS NOT NULL AND fecha_distribucion > :NEW.fecha_dis THEN
            RAISE_APPLICATION_ERROR(-20020, 'La fecha del pedido de una sucursal a otra de un determinado vino tiene que ser posterior a la fecha del último pedido entre dichas sucursales');
        END IF;
    END IF;
    
    IF UPDATING('fecha') THEN
        IF :OLD.fecha_dis > :NEW.fecha_dis THEN
            RAISE_APPLICATION_ERROR(-20020, 'La fecha del pedido de una sucursal a otra de un determinado vino tiene que ser posterior a la fecha del último pedido entre dichas sucursales');
        END IF;
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."FECHA_DISTRIBUCION_20" ENABLE;
--------------------------------------------------------
--  DDL for Trigger FECHA_NUEVO_PEDIDO
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."FECHA_NUEVO_PEDIDO" BEFORE INSERT OR UPDATE ON PEDIDO FOR EACH ROW
DECLARE
  fecha_ult_sum pedido.fecha_ped%TYPE;
BEGIN
  IF (INSERTING) THEN
    SELECT MAX(fecha_ped) INTO fecha_ult_sum
      FROM PEDIDOS
      WHERE cod_cli_ped = :NEW.cod_cli_ped;
    IF (fecha_ult_sum > :NEW.fecha_ped) THEN
      RAISE_APPLICATION_ERROR(-30021, 'La fecha del nuevo pedido debe ser igual o posterior al ultimo pedidio realizado' );
    END IF;
  END IF;

  IF (UPDATING('fecha_ped'))THEN  
    IF (:OLD.fecha_ped > :NEW.fecha_ped) THEN 
      RAISE_APPLICATION_ERROR(-30022, 'La fecha del nuevo pedido debe ser igual o posterior al ultimo pedidio realizado');
    END IF;
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."FECHA_NUEVO_PEDIDO" ENABLE;
--------------------------------------------------------
--  DDL for Trigger FECHA_PEDIDO_DISTRIBUCION_21
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."FECHA_PEDIDO_DISTRIBUCION_21" 
BEFORE INSERT OR UPDATE ON DISTRIBUCION FOR EACH ROW

DECLARE
    fecha_pedido DATE;
    
BEGIN
    CASE
        WHEN INSERTING THEN
            SELECT MAX(fecha_ped) INTO fecha_pedido FROM pedidos WHERE cod_suc_ped = :NEW.cod_suc_compra_dis AND cod_vino_ped = :NEW.cod_vino_dis;
            IF(fecha_pedido > :NEW.fecha_dis) THEN
                RAISE_APPLICATION_ERROR(-20021, 'La fecha de la distribución entre sucursales es errónea: no puede ser anterior a la fecha del pedido de ese vino a esa sucursal');
            END IF;
        WHEN UPDATING THEN
            IF :OLD.fecha_dis > :NEW.fecha_dis THEN
                RAISE_APPLICATION_ERROR(-20021, 'La modificación es errónea: no se puede asignar una fecha anterior a la previamente establecida');
            END IF;
    END CASE;
    
END;
/
ALTER TRIGGER "ESMERALDA1"."FECHA_PEDIDO_DISTRIBUCION_21" ENABLE;
--------------------------------------------------------
--  DDL for Trigger PEDIDO_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."PEDIDO_EXISTENTE" BEFORE INSERT OR UPDATE ON pedido FOR EACH ROW
  DECLARE
  	cont_ped NUMBER;
  BEGIN
    IF (INSERTING) THEN 
      SELECT COUNT(*) INTO cont_ped FROM PEDIDOS
        WHERE cod_vino_ped = :NEW.cod_vino_ped
          AND cod_suc_ped = :NEW.cod_suc_ped
          AND cod_cli_ped = :NEW.cod_cli_ped
          AND fecha_ped= :NEW.fecha_ped;
      IF (cont_ped>0) THEN 
        RAISE_APPLICATION_ERROR(-30018,'Erro el pedido ya existe.');
      END IF;
    END IF;
    IF (UPDATING('cod_vino_ped') OR UPDATING('cod_suc_ped') OR UPDATING('cod_cli_ped'))THEN
      RAISE_APPLICATION_ERROR(-30019,'Error no se puede cambiar el codigo del pedido.');
    END IF;
  END;  

/
ALTER TRIGGER "ESMERALDA1"."PEDIDO_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger PRODUCTOR_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."PRODUCTOR_EXISTENTE" BEFORE INSERT ON Productor FOR EACH ROW
DECLARE
	cont_pro NUMBER;
BEGIN
  IF INSERTING THEN 
    SELECT COUNT(*) INTO cont_pro FROM Productor
      WHERE cod_pro = :NEW.cod_pro;
    IF (cont_pro>0) THEN  
      RAISE_APPLICATION_ERROR(-30012,'Error el productor ya existe.');
    END IF;
  END IF;
  IF (UPDATING('cod_pro'))THEN
    RAISE_APPLICATION_ERROR(-30013,'No se puede modificar el codigo de un productor.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."PRODUCTOR_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger PRODUCTOR_UNICO_12
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."PRODUCTOR_UNICO_12" 
BEFORE INSERT OR UPDATE OF COD_PRO_VINO ON VINO FOR EACH ROW

DECLARE
    productor_vino NUMBER;

BEGIN
    SELECT COUNT(*) INTO productor_vino FROM vinos WHERE cod_vino = :OLD.cod_vino;
    IF (productor_vino > 1) THEN
        RAISE_APPLICATION_ERROR(-20012, 'Un vino no puede ser producido por más de un productor');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."PRODUCTOR_UNICO_12" ENABLE;
--------------------------------------------------------
--  DDL for Trigger PRODUCTOR_VINO_INEXISTENTE_13
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."PRODUCTOR_VINO_INEXISTENTE_13" 
BEFORE INSERT OR UPDATE OF COD_PRO_VINO ON VINO FOR EACH ROW

DECLARE
    productor_vino NUMBER;
    
BEGIN
    SELECT cod_pro_vino INTO productor_vino FROM vinos WHERE cod_pro_vino = :NEW.cod_pro_vino;
    
    IF (:NEW.cod_pro_vino IS NULL OR productor_vino IS NULL) THEN
        RAISE_APPLICATION_ERROR(-20013, '13. Un vino no puede existir si no existe un productor que lo produzca');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."PRODUCTOR_VINO_INEXISTENTE_13" ENABLE;
--------------------------------------------------------
--  DDL for Trigger STOCK_VINO_14
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."STOCK_VINO_14" 
BEFORE INSERT OR UPDATE OF CAN_STOCK_VINO ON VINO FOR EACH ROW
BEGIN
    IF (:NEW.can_stock_vino < 0 OR :NEW.can_stock_vino > :NEW.can_prod_vino) THEN
    RAISE_APPLICATION_ERROR(-20014, 'La cantidad de stock no es correcta: es menor que 0 o mayor que la cantidad producida');
    END IF;
END;
/
ALTER TRIGGER "ESMERALDA1"."STOCK_VINO_14" ENABLE;
--------------------------------------------------------
--  DDL for Trigger SUCURSAL_DE_EMPLEADO_EXISTE_7
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."SUCURSAL_DE_EMPLEADO_EXISTE_7" BEFORE INSERT OR UPDATE ON empleado FOR EACH ROW
DECLARE
  cont_emp NUMBER;
BEGIN
  IF (INSERTING OR UPDATING ('cod_suc')) THEN
    SELECT COUNT(cod_suc) INTO cont_emp FROM sucursales
      WHERE cod_suc = :NEW.cod_suc_emp;
        IF (cont_emp=0) THEN 
          RAISE_APPLICATION_ERROR(-30008,'La sucursal para el empleado debe existir.');
        END IF;
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."SUCURSAL_DE_EMPLEADO_EXISTE_7" ENABLE;
--------------------------------------------------------
--  DDL for Trigger SUCURSAL_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."SUCURSAL_EXISTENTE" BEFORE INSERT OR UPDATE ON sucursal FOR EACH ROW
DECLARE
	cont_suc NUMBER;
BEGIN
  IF (INSERTING) THEN 
    SELECT COUNT(cod_suc) INTO cont_suc
      FROM sucursales 
      WHERE cod_suc = :NEW.cod_suc;
    IF (cont_suc>0) THEN  
      RAISE_APPLICATION_ERROR(-30020,'Error la sucursal ya existe.');
    END IF;
  END IF;
  IF (UPDATING('cod_suc')) THEN
    RAISE_APPLICATION_ERROR(-30021, 'Error no se puede modificar el codigo de la sucursal.');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."SUCURSAL_EXISTENTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger TIPO_CLIENTE_8
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."TIPO_CLIENTE_8" BEFORE INSERT OR UPDATE ON cliente FOR EACH ROW
DECLARE
BEGIN
  IF (UPDATING ('tipo_cli') OR INSERTING) THEN
    IF (:NEW.tipo_cli NOT IN('A','B','C')) THEN 
      RAISE_APPLICATION_ERROR(-30019,'El tipo de cliente es incorrecto');
    END IF;
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."TIPO_CLIENTE_8" ENABLE;
--------------------------------------------------------
--  DDL for Trigger VINO_EXISTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."VINO_EXISTE" BEFORE INSERT OR UPDATE ON pedido FOR EACH ROW
DECLARE
  cont_vino NUMBER;
BEGIN
  IF INSERTING OR UPDATING('cod_vino_ped') THEN
    SELECT COUNT (*) INTO cont_vino
      FROM vinos
      WHERE cod_vino = :NEW.cod_vino_ped;
    IF(cont_vino = 0) THEN  
      RAISE_APPLICATION_ERROR(-32123, 'El vino no existe.');
    END IF;
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."VINO_EXISTE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger VINO_EXISTENTE
--------------------------------------------------------

  CREATE OR REPLACE EDITIONABLE TRIGGER "ESMERALDA1"."VINO_EXISTENTE" BEFORE INSERT ON VINO FOR EACH ROW
DECLARE
	cont_vino NUMBER(1);
BEGIN
  IF (INSERTING) THEN 
    SELECT COUNT(cod_vino) INTO cont_vino
      FROM VINOS
      WHERE cod_vino = :NEW.cod_vino;
    IF (cont_vino>0) THEN  

      RAISE_APPLICATION_ERROR(-3026,'Error el vino con ese código ya existe.');
    END IF;
  END IF;
  IF (UPDATING('cod_vino')) THEN
    RAISE_APPLICATION_ERROR(-3027, 'Error no se puede modificar el codigo del vino');
  END IF;
END;

/
ALTER TRIGGER "ESMERALDA1"."VINO_EXISTENTE" ENABLE;
