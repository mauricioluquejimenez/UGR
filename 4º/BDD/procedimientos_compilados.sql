--------------------------------------------------------
-- Archivo creado  - lunes-enero-08-2024   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Procedure ALTA_ACTUALIZAR_PEDIDO
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."ALTA_ACTUALIZAR_PEDIDO" 
(
  NEW_COD_CLI IN NUMBER,
  NEW_COD_SUC IN NUMBER, 
  NEW_COD_VINO IN NUMBER, 
  NEW_FECHA_SOLICITUD IN DATE, 
  NEW_CAN_PED IN NUMBER 
) AS 
    suministro_exists NUMBER;
    codigo_suministro NUMBER;
    comunidad VARCHAR(50);
BEGIN
IF NEW_FECHA_SOLICITUD IS NOT NULL THEN

    SELECT COUNT(*) INTO suministro_exists FROM (
        SELECT * FROM esmeralda1.suministro1 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT * FROM esmeralda2.suministro2 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT * FROM esmeralda3.suministro3 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT * FROM esmeralda4.suministro4 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
    );

    SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1 WHERE esmeralda1.sucursal1.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2 WHERE esmeralda2.sucursal2.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3 WHERE esmeralda3.sucursal3.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4 WHERE esmeralda4.sucursal4.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
        );

    IF suministro_exists >= 1 THEN

    SELECT cod_sum INTO codigo_suministro FROM (
        SELECT cod_sum FROM esmeralda1.suministro1 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT cod_sum FROM esmeralda2.suministro2 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT cod_sum FROM esmeralda3.suministro3 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
        UNION
        SELECT cod_sum FROM esmeralda4.suministro4 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO AND fecha_sum = NEW_FECHA_SOLICITUD
    );

        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                INSERT INTO esmeralda1.pedido1 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                INSERT INTO esmeralda2.pedido2 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                INSERT INTO esmeralda3.pedido3 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta', 'Melilla')) THEN
                INSERT INTO esmeralda4.pedido4 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );
        END CASE;
        DBMS_OUTPUT.PUT_LINE('Suministro registrado o actualizado para el cliente con c骴igo ' || NEW_COD_CLI);
    ELSE
        raise_application_error(-20112, 'ERROR: No existe un pedido con esos par醡etros');
    END IF;
ELSE

    SELECT COUNT(*) INTO suministro_exists FROM (
        SELECT * FROM esmeralda1.suministro1 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT * FROM esmeralda2.suministro2 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT * FROM esmeralda3.suministro3 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT * FROM esmeralda4.suministro4 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
    );

    SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1 WHERE esmeralda1.sucursal1.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2 WHERE esmeralda2.sucursal2.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3 WHERE esmeralda3.sucursal3.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4 WHERE esmeralda4.sucursal4.cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
        );
        
    IF suministro_exists >= 1 THEN
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                INSERT INTO esmeralda1.pedido1 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                INSERT INTO esmeralda2.pedido2 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                INSERT INTO esmeralda3.pedido3 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );

            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta', 'Melilla')) THEN
                INSERT INTO esmeralda4.pedido4 (
                    can_ped,
                    cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped
                )
                VALUES (
                    NEW_CAN_PED,
                    NEW_COD_CLI,
                    codigo_suministro,
                    NEW_FECHA_SOLICITUD
                );
        END CASE;
        DBMS_OUTPUT.PUT_LINE('Suministro registrado o actualizado para el cliente con c骴igo ' || NEW_COD_CLI);

    ELSE
        raise_application_error(-20112, 'ERROR: No existe un pedido con esos par醡etros');
    END IF;
END IF;
END ALTA_ACTUALIZAR_PEDIDO;

/
--------------------------------------------------------
--  DDL for Procedure ALTA_DISTRIBUCION
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."ALTA_DISTRIBUCION" 
(
  NEW_COD_SUC_ORIGEN IN NUMBER,
  NEW_COD_SUC_DESTINO IN NUMBER,
  NEW_COD_VINO IN NUMBER,
  NEW_FECHA_DIS IN DATE,
  NEW_CAN_DIS IN NUMBER 
) AS
    sucursal_exists NUMBER;
    codigo_suministro NUMBER;
    comunidad VARCHAR(50);
BEGIN
    SELECT ca_suc INTO comunidad FROM(
        SELECT ca_suc FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
        UNION
        SELECT ca_suc FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
        UNION
        SELECT ca_suc FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
        UNION
        SELECT ca_suc FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
    );

    IF sucursal_exists = 0 THEN
        raise_application_error(-20112, 'ERROR: No existe una sucursal de origen con ese c骴igo');
    ELSE
        SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC_ORIGEN GROUP BY (ca_suc)
        );

        IF sucursal_exists = 0 THEN
        raise_application_error(-20112, 'ERROR: No existe una sucursal de destino con ese c骴igo');
        ELSE
            SELECT cod_sum INTO codigo_suministro FROM (
                    SELECT cod_sum FROM esmeralda1.suministro1 WHERE cod_vino_sum = NEW_COD_VINO AND cod_suc_sum = NEW_COD_SUC_ORIGEN    
                    UNION
                    SELECT cod_sum FROM esmeralda2.suministro2 WHERE cod_vino_sum = NEW_COD_VINO AND cod_suc_sum = NEW_COD_SUC_ORIGEN    
                    UNION
                    SELECT cod_sum FROM esmeralda3.suministro3 WHERE cod_vino_sum = NEW_COD_VINO AND cod_suc_sum = NEW_COD_SUC_ORIGEN    
                    UNION
                    SELECT cod_sum FROM esmeralda4.suministro4 WHERE cod_vino_sum = NEW_COD_VINO AND cod_suc_sum = NEW_COD_SUC_ORIGEN
            );
            CASE
                WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                    INSERT INTO esmeralda1.distribucion1 (
                        cod_suc_dis,
                        cod_sum_dis,
                        can_dis,
                        fecha_dis
                    )
                    VALUES(
                        NEW_COD_SUC_DESTINO,
                        codigo_suministro,
                        NEW_CAN_DIS,
                        NEW_FECHA_DIS
                    );
                WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                    INSERT INTO esmeralda2.distribucion2 (
                        cod_suc_dis,
                        cod_sum_dis,
                        can_dis,
                        fecha_dis
                    )
                    VALUES(
                        NEW_COD_SUC_DESTINO,
                        codigo_suministro,
                        NEW_CAN_DIS,
                        NEW_FECHA_DIS
                    );
                WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                    INSERT INTO esmeralda3.distribucion3 (
                        cod_suc_dis,
                        cod_sum_dis,
                        can_dis,
                        fecha_dis
                    )
                    VALUES(
                        NEW_COD_SUC_DESTINO,
                        codigo_suministro,
                        NEW_CAN_DIS,
                        NEW_FECHA_DIS
                    );
                WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                    INSERT INTO esmeralda4.distribucion4 (
                        cod_suc_dis,
                        cod_sum_dis,
                        can_dis,
                        fecha_dis
                    )
                    VALUES(
                        NEW_COD_SUC_DESTINO,
                        codigo_suministro,
                        NEW_CAN_DIS,
                        NEW_FECHA_DIS
                    );
            END CASE;
        END IF;
    END IF;

END ALTA_DISTRIBUCION;

/
--------------------------------------------------------
--  DDL for Procedure ALTA_NUEVA_SUCURSAL
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."ALTA_NUEVA_SUCURSAL" 
(
  NEW_COD_SUC IN NUMBER, 
  NEW_NOMBRE_SUC IN VARCHAR2, 
  NEW_CIUDAD_SUC IN VARCHAR2, 
  NEW_CA_SUC IN VARCHAR2, 
  NEW_DIRECTOR_SUC IN NUMBER
) AS
    director_exists NUMBER;
BEGIN
    /*SELECT COUNT(*) INTO director_exists FROM (
            -- Se hace asi porque tiene que buscar en todas las tablas empleado (1,2,3,4)
        SELECT 1 FROM esmeralda1.empleado1 WHERE cod_emp = NEW_director_suc     
        UNION
        SELECT 1 FROM esmeralda2.empleado2 WHERE cod_emp = NEW_director_suc
        UNION
        SELECT 1 FROM esmeralda3.empleado3 WHERE cod_emp = NEW_director_suc
        UNION
        SELECT 1 FROM esmeralda4.empleado4 WHERE cod_emp = NEW_director_suc
    );

    IF director_exists = 0 THEN
            raise_application_error(-20101, 'ERROR: No existe un empleado con ese c贸digo para ser director de sucursal');

    ELSE*/
        CASE
            WHEN(NEW_CA_SUC IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                INSERT INTO esmeralda1.sucursal1(
                    cod_suc,
                    ciudad_suc,
                    ca_suc,
                    director_suc,
                    nombre_suc
                )
                    VALUES (
                    NEW_cod_suc,
                    NEW_ciudad_suc,
                    NEW_ca_suc,
                    NEW_director_suc,
                    NEW_nombre_suc
                );
            WHEN(NEW_CA_SUC IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                INSERT INTO esmeralda2.sucursal2 (
                    cod_suc,
                    ciudad_suc,
                    ca_suc,
                    director_suc,
                    nombre_suc
                )
                VALUES (
                    NEW_cod_suc,
                    NEW_ciudad_suc,
                    NEW_ca_suc,
                    NEW_director_suc,
                    NEW_nombre_suc
                );
             WHEN(NEW_CA_SUC IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                INSERT INTO esmeralda3.sucursal3(            
                    cod_suc,
                    ciudad_suc,
                    ca_suc,
                    director_suc,
                    nombre_suc
                )
                VALUES (
                    NEW_cod_suc,
                    NEW_ciudad_suc,
                    NEW_ca_suc,
                    NEW_director_suc,
                    NEW_nombre_suc
                );

            WHEN(NEW_CA_SUC IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                INSERT INTO esmeralda4.sucursal4(            
                    cod_suc,
                    ciudad_suc,
                    ca_suc,
                    director_suc,
                    nombre_suc
                )
                VALUES (
                    NEW_cod_suc,
                    NEW_ciudad_suc,
                    NEW_ca_suc,
                    NEW_director_suc,
                    NEW_nombre_suc
                );
        END CASE;

            DBMS_OUTPUT.PUT_LINE('Nueva sucursal con c贸digo ' || NEW_cod_suc || ' dada de alta');
        /*END IF;*/
END ALTA_NUEVA_SUCURSAL;

/
--------------------------------------------------------
--  DDL for Procedure ALTA_NUEVO_CLIENTE
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."ALTA_NUEVO_CLIENTE" 
(
  NEW_COD_CLI IN NUMBER,
  NEW_DNI_CLI IN VARCHAR2,
  NEW_NOMBRE_CLI IN VARCHAR2,
  NEW_DIRECCION_CLI IN VARCHAR2,
  NEW_TIPO_CLI IN VARCHAR2,
  NEW_CA_CLI IN VARCHAR2 
) AS
    comunidad_exists NUMBER;
BEGIN
    CASE 
        WHEN(NEW_CA_CLI IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
            INSERT INTO esmeralda1.cliente1 (
                cod_cli,
                ca_cli,
                direccion_cli,
                dni_cli,
                nombre_cli,
                tipo_cli
            )
            VALUES (
                NEW_COD_CLI,
                NEW_CA_CLI,
                NEW_DIRECCION_CLI,
                NEW_DNI_CLI,
                NEW_NOMBRE_CLI,
                NEW_TIPO_CLI
            );

        WHEN(NEW_CA_CLI IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
            INSERT INTO esmeralda2.cliente2 (
                cod_cli,
                ca_cli,
                direccion_cli,
                dni_cli,
                nombre_cli,
                tipo_cli
            )
            VALUES (
                NEW_COD_CLI,
                NEW_CA_CLI,
                NEW_DIRECCION_CLI,
                NEW_DNI_CLI,
                NEW_NOMBRE_CLI,
                NEW_TIPO_CLI
            );

        WHEN(NEW_CA_CLI IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
            INSERT INTO esmeralda3.cliente3 (
                cod_cli,
                ca_cli,
                direccion_cli,
                dni_cli,
                nombre_cli,
                tipo_cli
            )
            VALUES (
                NEW_COD_CLI,
                NEW_CA_CLI,
                NEW_DIRECCION_CLI,
                NEW_DNI_CLI,
                NEW_NOMBRE_CLI,
                NEW_TIPO_CLI
            );

        WHEN(NEW_CA_CLI IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
            INSERT INTO esmeralda4.cliente4 (
                cod_cli,
                ca_cli,
                direccion_cli,
                dni_cli,
                nombre_cli,
                tipo_cli
            )
            VALUES (
                NEW_COD_CLI,
                NEW_CA_CLI,
                NEW_DIRECCION_CLI,
                NEW_DNI_CLI,
                NEW_NOMBRE_CLI,
                NEW_TIPO_CLI
            );
    END CASE;

    DBMS_OUTPUT.PUT_LINE('Nuevo cliente con c贸digo ' || NEW_COD_CLI || ' dado de alta');
END ALTA_NUEVO_CLIENTE;

/
--------------------------------------------------------
--  DDL for Procedure ALTA_NUEVO_EMPLEADO
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."ALTA_NUEVO_EMPLEADO" (
    NEW_COD_EMP IN NUMBER,
    NEW_DNI_EMP IN VARCHAR2,
    NEW_NOMBRE_EMP IN VARCHAR2,
    NEW_FECHA_INI_CONTRATO_EMP IN DATE,
    NEW_SALARIO_EMP IN NUMBER,
    NEW_DIRECCION_EMP IN VARCHAR2,
    NEW_COD_SUC_EMP IN NUMBER 
) AS

    sucursal_exists NUMBER;
    comunidad VARCHAR(50);

BEGIN
  SELECT COUNT(*) INTO sucursal_exists FROM (
        -- Se hace asi porque tiene que buscar en todas las tablas empleado (1,2,3,4)
        SELECT 1 FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC_EMP
        UNION
        SELECT 1 FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC_EMP
        UNION
        SELECT 1 FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC_EMP
        UNION
        SELECT 1 FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC_EMP
    );

    IF sucursal_exists = 0 THEN
        raise_application_error(-20101, 'ERROR: No existe una sucursal con ese c骴igo');
    ELSE
        SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_cod_suc_emp
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_cod_suc_emp
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_cod_suc_emp
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_cod_suc_emp
        );
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
            --  puede tener el mismo nombre la tabla empleado? 
                INSERT INTO esmeralda1.empleado1(            
                cod_emp,
                cod_suc_emp,
                direccion_emp,
                dni_emp,
                fecha_ini_contrato_emp,
                nombre_emp,
                salario_emp
            )
            VALUES (
                NEW_cod_emp,
                NEW_cod_suc_emp,
                NEW_direccion_emp,
                NEW_dni_emp,
                NEW_fecha_ini_contrato_emp,
                NEW_nombre_emp,
                NEW_salario_emp
            );

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
            --  puede tener el mismo nombre la tabla empleado? 
                INSERT INTO esmeralda2.empleado2(            
                cod_emp,
                cod_suc_emp,
                direccion_emp,
                dni_emp,
                fecha_ini_contrato_emp,
                nombre_emp,
                salario_emp
            )
            VALUES (
                NEW_cod_emp,
                NEW_cod_suc_emp,
                NEW_direccion_emp,
                NEW_dni_emp,
                NEW_fecha_ini_contrato_emp,
                NEW_nombre_emp,
                NEW_salario_emp
            );

                WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                --  puede tener el mismo nombre la tabla empleado? 
                    INSERT INTO esmeralda3.empleado3(            
                    cod_emp,
                    cod_suc_emp,
                    direccion_emp,
                    dni_emp,
                    fecha_ini_contrato_emp,
                    nombre_emp,
                    salario_emp
                )
                VALUES (
                    NEW_cod_emp,
                    NEW_cod_suc_emp,
                    NEW_direccion_emp,
                    NEW_dni_emp,
                    NEW_fecha_ini_contrato_emp,
                    NEW_nombre_emp,
                    NEW_salario_emp
                );

                WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                --  puede tener el mismo nombre la tabla empleado? 
                    INSERT INTO esmeralda4.empleado4(            
                    cod_emp,
                    cod_suc_emp,
                    direccion_emp,
                    dni_emp,
                    fecha_ini_contrato_emp,
                    nombre_emp,
                    salario_emp
                )
                VALUES (
                    NEW_cod_emp,
                    NEW_cod_suc_emp,
                    NEW_direccion_emp,
                    NEW_dni_emp,
                    NEW_fecha_ini_contrato_emp,
                    NEW_nombre_emp,
                    NEW_salario_emp
                );
            END CASE;

            DBMS_OUTPUT.PUT_LINE('Nuevo Empleado con c骴igo ' || NEW_cod_emp || ' dado de Alta');
        END IF;
END ALTA_NUEVO_EMPLEADO;

/
--------------------------------------------------------
--  DDL for Procedure BAJA_EMPLEADO
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."BAJA_EMPLEADO" 
(
  EMP_COD_EMP IN NUMBER 
)
IS
    sucursal_director NUMBER;
    comunidad VARCHAR(50);
BEGIN
      -- Verificar si el empleado es director de alguna sucursal
    SELECT COUNT(*) INTO sucursal_director FROM (
            -- Se hace asi porque tiene que buscar en todas las tablas empleado (1,2,3,4)
            SELECT 1 FROM esmeralda1.sucursal1 WHERE director_suc = EMP_COD_EMP
            UNION
            SELECT 1 FROM esmeralda2.sucursal2 WHERE director_suc = EMP_COD_EMP
            UNION
            SELECT 1 FROM esmeralda3.sucursal3 WHERE director_suc = EMP_COD_EMP
            UNION
            SELECT 1 FROM esmeralda4.sucursal4 WHERE director_suc = EMP_COD_EMP
        );

    SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1, esmeralda1.empleado1 WHERE esmeralda1.empleado1.cod_emp = EMP_COD_EMP AND esmeralda1.empleado1.cod_suc_emp = esmeralda1.sucursal1.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2, esmeralda2.empleado2 WHERE esmeralda2.empleado2.cod_emp = EMP_COD_EMP AND esmeralda2.empleado2.cod_suc_emp = esmeralda2.sucursal2.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3, esmeralda3.empleado3 WHERE esmeralda3.empleado3.cod_emp = EMP_COD_EMP AND esmeralda3.empleado3.cod_suc_emp = esmeralda3.sucursal3.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4, esmeralda4.empleado4 WHERE esmeralda4.empleado4.cod_emp = EMP_COD_EMP AND esmeralda4.empleado4.cod_suc_emp = esmeralda4.sucursal4.cod_suc GROUP BY (ca_suc)
    );

    IF sucursal_director > 0 THEN
        -- El empleado es director de alguna sucursal, actualizar la informaci贸n  
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                UPDATE esmeralda1.sucursal1 SET director_suc = NULL WHERE director_suc = EMP_cod_emp;
                DELETE FROM esmeralda1.empleado1 WHERE cod_emp = EMP_cod_emp;
            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                UPDATE esmeralda2.sucursal2 SET director_suc = NULL WHERE director_suc = EMP_cod_emp;
                DELETE FROM esmeralda2.empleado2 WHERE cod_emp = EMP_cod_emp;
            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                UPDATE esmeralda3.sucursal3 SET director_suc = NULL WHERE director_suc = EMP_cod_emp;
                DELETE FROM esmeralda3.empleado3 WHERE cod_emp = EMP_cod_emp;
            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                UPDATE esmeralda4.sucursal4 SET director_suc = NULL WHERE director_suc = EMP_cod_emp;  
                DELETE FROM esmeralda4.empleado4 WHERE cod_emp = EMP_cod_emp;
        END CASE;
    ELSE
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                DELETE FROM esmeralda1.empleado1 WHERE cod_emp = EMP_cod_emp;

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                DELETE FROM esmeralda2.empleado2 WHERE cod_emp = EMP_cod_emp;

            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                DELETE FROM esmeralda3.empleado3 WHERE cod_emp = EMP_cod_emp;

            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                DELETE FROM esmeralda4.empleado4 WHERE cod_emp = EMP_cod_emp;  
        END CASE;
    END IF;

    DBMS_OUTPUT.PUT_LINE('Empleado con c贸digo ' || EMP_cod_emp || ' dado de Baja');

END BAJA_EMPLEADO;

/
--------------------------------------------------------
--  DDL for Procedure BAJA_PEDIDO
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."BAJA_PEDIDO" 
(
  NEW_COD_CLI IN NUMBER, 
  NEW_COD_SUC IN NUMBER, 
  NEW_COD_VINO IN VARCHAR2, 
  NEW_FECHA_PED IN VARCHAR2 
) AS
    codigo_sucursaL NUMBER;
    comunidad VARCHAR(50);
BEGIN

    SELECT cod_suc_sum INTO codigo_sucursal FROM (
        SELECT cod_suc_sum FROM esmeralda1.suministro1 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT cod_suc_sum FROM esmeralda2.suministro2 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT cod_suc_sum FROM esmeralda3.suministro3 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
        UNION
        SELECT cod_suc_sum FROM esmeralda4.suministro4 WHERE cod_suc_sum = NEW_COD_SUC AND cod_vino_sum = NEW_COD_VINO
    );

    SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1 WHERE codigo_sucursaL = esmeralda1.sucursal1.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2 WHERE codigo_sucursaL = esmeralda2.sucursal2.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3 WHERE codigo_sucursaL = esmeralda3.sucursal3.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4 WHERE codigo_sucursaL = esmeralda4.sucursal4.cod_suc GROUP BY (ca_suc)
    );

    IF NEW_FECHA_PED IS NOT NULL THEN
        -- Si nos dan la fecha eliminamos suministros concretos 
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                DELETE FROM esmeralda1.pedido1 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC AND fecha_ped = NEW_FECHA_PED;

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                DELETE FROM esmeralda2.pedido2 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC AND fecha_ped = NEW_FECHA_PED;

            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                DELETE FROM esmeralda3.pedido3 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC AND fecha_ped = NEW_FECHA_PED;

            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                DELETE FROM esmeralda4.pedido4 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC AND fecha_ped = NEW_FECHA_PED;  

        END CASE;
    ELSE
        -- Si no eliminamos todos los suministros de ese vino por la sucursal al cliente    
        CASE 
            WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                DELETE FROM esmeralda1.pedido1 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC;

            WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                DELETE FROM esmeralda2.pedido2 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC;

            WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                DELETE FROM esmeralda3.pedido3 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC;

            WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                DELETE FROM esmeralda4.pedido4 WHERE cod_cli_ped = NEW_COD_CLI AND codigo_sucursaL = NEW_COD_SUC;  

        END CASE;
    END IF;
END BAJA_PEDIDO;

/
--------------------------------------------------------
--  DDL for Procedure MODIFICAR_DIRECTOR_SUCURSAL
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."MODIFICAR_DIRECTOR_SUCURSAL" 
(
  NEW_COD_SUC IN NUMBER, 
  NEW_DIRECTOR_SUC IN NUMBER 
) AS
    sucursal_exists NUMBER;
    director_sucursal NUMBER;
    comunidad VARCHAR(50);
BEGIN
  -- Verificar si la sucursal existe
    SELECT COUNT(*) INTO sucursal_exists FROM (
        -- Se hace asi porque tiene que buscar en todas las tablas empleado (1,2,3,4)
        SELECT 1 FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC
        UNION
        SELECT 1 FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC
        UNION
        SELECT 1 FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC
        UNION
        SELECT 1 FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC
    );

    IF sucursal_exists = 0 THEN
        raise_application_error(-20104, 'ERROR: No existe una sucursal con ese c骴igo');
    ELSE
        SELECT director_suc INTO director_sucursal FROM(
            SELECT director_suc FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC
            UNION
            SELECT director_suc FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC
            UNION
            SELECT director_suc FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC
            UNION
            SELECT director_suc FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC
        );
        IF director_sucursal = NEW_DIRECTOR_SUC THEN
            raise_application_error(-20104, 'ERROR: Este empleado ya dirige la sucursal');
        ELSE
            SELECT ca_suc INTO comunidad FROM(
                SELECT ca_suc FROM esmeralda1.sucursal1 WHERE cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
                UNION
                SELECT ca_suc FROM esmeralda2.sucursal2 WHERE cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
                UNION
                SELECT ca_suc FROM esmeralda3.sucursal3 WHERE cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
                UNION
                SELECT ca_suc FROM esmeralda4.sucursal4 WHERE cod_suc = NEW_COD_SUC GROUP BY (ca_suc)
            );
                CASE 
                    WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                        UPDATE esmeralda1.sucursal1 SET director_suc = NEW_DIRECTOR_SUC WHERE cod_suc = NEW_COD_SUC;
                    WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                        UPDATE esmeralda2.sucursal2 SET director_suc = NEW_DIRECTOR_SUC WHERE cod_suc = NEW_COD_SUC;
                    WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                        UPDATE esmeralda3.sucursal3 SET director_suc = NEW_DIRECTOR_SUC WHERE cod_suc = NEW_COD_SUC;
                    WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                        UPDATE esmeralda4.sucursal4 SET director_suc = NEW_DIRECTOR_SUC WHERE cod_suc = NEW_COD_SUC;
                END CASE;
                DBMS_OUTPUT.PUT_LINE('Director de la sucursal con c骴igo ' || NEW_COD_SUC || ' cambiado a ' || NEW_DIRECTOR_SUC);
        END IF;
    END IF;
END MODIFICAR_DIRECTOR_SUCURSAL;

/
--------------------------------------------------------
--  DDL for Procedure MODIFICAR_SALARIO_EMPLEADO
--------------------------------------------------------
set define off;

  CREATE OR REPLACE EDITIONABLE PROCEDURE "ESMERALDA1"."MODIFICAR_SALARIO_EMPLEADO" 
(
  EMP_COD_EMP IN NUMBER, 
  NEW_SALARIO_EMP IN NUMBER
) AS
    comunidad VARCHAR(50);
BEGIN
    SELECT ca_suc INTO comunidad FROM(
            SELECT ca_suc FROM esmeralda1.sucursal1, esmeralda1.empleado1 WHERE esmeralda1.empleado1.cod_emp = EMP_COD_EMP AND esmeralda1.empleado1.cod_suc_emp = esmeralda1.sucursal1.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda2.sucursal2, esmeralda2.empleado2 WHERE esmeralda2.empleado2.cod_emp = EMP_COD_EMP AND esmeralda2.empleado2.cod_suc_emp = esmeralda2.sucursal2.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda3.sucursal3, esmeralda3.empleado3 WHERE esmeralda3.empleado3.cod_emp = EMP_COD_EMP AND esmeralda3.empleado3.cod_suc_emp = esmeralda3.sucursal3.cod_suc GROUP BY (ca_suc)
            UNION
            SELECT ca_suc FROM esmeralda4.sucursal4, esmeralda4.empleado4 WHERE esmeralda4.empleado4.cod_emp = EMP_COD_EMP AND esmeralda4.empleado4.cod_suc_emp = esmeralda4.sucursal4.cod_suc GROUP BY (ca_suc)
        );
    CASE 
        WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
            UPDATE esmeralda1.empleado1 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;

        WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
            UPDATE esmeralda2.empleado2 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;

        WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
            UPDATE esmeralda3.empleado3 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;

        WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
            UPDATE esmeralda4.empleado4 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;

    END CASE;
END MODIFICAR_SALARIO_EMPLEADO;

/
