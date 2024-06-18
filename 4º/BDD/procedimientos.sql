CREATE OR REPLACE PROCEDURE AltaNuevoEmpleado(
    NEW_cod_emp IN NUMBER,
    NEW_dni_emp IN VARCHAR2,
    NEW_nombre_emp IN VARCHAR2,
    NEW_fecha_ini_contrato_emp IN DATE,
    NEW_salario_emp IN NUMBER,
    NEW_direccion_emp IN VARCHAR2,
    NEW_cod_suc_emp IN NUMBER
)
IS
    sucursal_exists NUMBER;
    comunidad esmeralda1.sucursal.ca_suc%TYPE;
BEGIN

    SELECT COUNT(*) INTO sucursal_exists FROM sucursal WHERE cod_suc = NEW_cod_suc_emp;

        IF sucursal_exists = 0 THEN
            raise_application_error(-20101, 'ERROR: No existe una sucursal con ese c骴igo');
        ELSE
            SELECT ca_suc INTO comunidad FROM sucursal WHERE cod_suc = NEW_cod_suc_emp;
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
                    NEW_suc_emp,
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
                    NEW_suc_emp,
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
                    NEW_cod_empleado,
                    NEW_cod_sucursal,
                    NEW_direccion,
                    NEW_dni,
                    NEW_fecha_ini_contrato,
                    NEW_nombre,
                    NEW_salario
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
                    NEW_suc_emp,
                    NEW_direccion_emp,
                    NEW_dni_emp,
                    NEW_fecha_ini_contrato_emp,
                    NEW_nombre_emp,
                    NEW_salario_emp
                );
            END CASE;

            DBMS_OUTPUT.PUT_LINE('Nuevo Empleado con c骴igo ' || NEW_cod_empleado || ' dado de Alta');
        END IF; 
END AltaNuevoEmpleado;




CREATE OR REPLACE PROCEDURE BajaEmpleado(
    EMP_cod_emp IN NUMBER
)
IS
    sucursal_director NUMBER;
BEGIN
    -- Verificar si el empleado es director de alguna sucursal
    SELECT COUNT(*) INTO sucursal_director FROM sucursal WHERE director_suc = EMP_cod_emp;

    IF sucursal_director > 0 THEN
        -- El empleado es director de alguna sucursal, actualizar la informaci贸n
        UPDATE sucursal SET director_suc = NULL WHERE director_suc = EMP_cod_emp;
    END IF;

    -- NO SE SI HACE FALTA ALGUNA COMPROBACION ANTES
    DELETE FROM esmeralda1.empleado1 WHERE cod_emp = EMP_cod_emp;
    DELETE FROM esmeralda2.empleado2 WHERE cod_emp = EMP_cod_emp;
    DELETE FROM esmeralda3.empleado3 WHERE cod_emp = EMP_cod_emp;
    DELETE FROM esmeralda4.empleado4 WHERE cod_emp = EMP_cod_emp;

    DBMS_OUTPUT.PUT_LINE('Empleado con c贸digo ' || EMP_cod_emp || ' dado de Baja');
END BajaEmpleado;



CREATE OR REPLACE PROCEDURE ModificarSalarioEmpleado(
    EMP_cod_emp IN NUMBER,
    NEW_salario_emp IN NUMBER
)
IS
    empleado_exists NUMBER;
BEGIN

    UPDATE esmeralda1.empleado1 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;
    UPDATE esmeralda2.empleado2 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;
    UPDATE esmeralda3.empleado3 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;
    UPDATE esmeralda4.empleado4 SET salario_emp = NEW_salario_emp WHERE cod_emp = EMP_cod_emp;

    DBMS_OUTPUT.PUT_LINE('Salario del empleado con c骴igo ' || EMP_cod_emp || ' modificado a ' || NEW_salario_emp);
END ModificarSalarioEmpleado;




CREATE OR REPLACE PROCEDURE AltaNuevaSucursal(
    NEW_cod_suc IN NUMBER,
    NEW_nombre_suc IN VARCHAR2,
    NEW_ciudad_suc IN VARCHAR2,
    NEW_ca_suc IN VARCHAR2,
    NEW_director_suc IN NUMBER DEFAULT NULL
)
IS
    director_exists NUMBER;
BEGIN
        IF NEW_director_suc IS NOT NULL THEN
            SELECT COUNT(*) INTO director_exists FROM (
                -- Se hace asi porque tiene que buscar en todas las tablas empleado (1,2,3,4)
                SELECT 1 FROM esmeralda1.empleado1 WHERE cod_emp = NEW_director_suc     
                UNION
                SELECT 1 FROM esmeralda2.empleado2 WHERE cod_emp = NEW_director_suc
                UNION
                SELECT 1 FROM esmeralda3.empleado3 WHERE cod_emp = NEW_director_suc
                UNION
                SELECT 1 FROM esmeralda4.empleado4 WHERE cod_emp = NEW_director_suc
            );
        END IF;

        IF director_exists = 0 THEN
                raise_application_error(-20101, 'ERROR: No existe un empleado con ese c贸digo para ser director de sucursal');

        ELSE
            CASE 
                WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                    INSERT INTO esmeralda1.sucursal1(
                        cod_suc,
                        ciudad_suc,
                        ca_suc,
                        ca_suc,
                        nombre_suc
                    )
                    VALUES (
                        NEW_cod_suc,
                        NEW_ciudad_suc,
                        NEW_comunidad,
                        NEW_ca_suc,
                        NEW_nombre_suc
                    );
                WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
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
                        NEW_comunidad,
                        NEW_ca_suc,
                        NEW_nombre_suc
                    );
                 WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
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
                        NEW_comunidad,
                        NEW_ca_suc,
                        NEW_nombre_suc
                    );

                WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                    INSERT INTO esmeralda4.sucursal4(            
                    ccod_suc,
                        ciudad_suc,
                        ca_suc,
                        director_suc,
                        nombre_suc
                    )
                    VALUES (
                        NEW_cod_suc,
                        NEW_ciudad_suc,
                        NEW_comunidad,
                        NEW_ca_suc,
                        NEW_nombre_suc
                    );
            END CASE;
        
            DBMS_OUTPUT.PUT_LINE('Nueva sucursal con c贸digo ' || NEW_cod_suc || ' dada de alta');
        END IF; 
    
END AltaNuevaSucursal;




CREATE OR REPLACE PROCEDURE CambiarDirectorSucursal(
    P_cod_suc IN NUMBER,
    P_nuevo_director_suc IN NUMBER
)
IS
    sucursal_exists NUMBER;
    comunidad esmeralda1.sucursal.ca_suc%TYPE;
BEGIN
    -- Verificar si la sucursal existe
    SELECT COUNT(*) INTO sucursal_exists FROM sucursal WHERE cod_suc = P_cod_suc;

    IF sucursal_exists = 0 THEN
        raise_application_error(-20104, 'ERROR: No existe una sucursal con ese c骴igo');
    ELSE    
        SELECT ca_suc INTO comunidad FROM sucursal WHERE cod_suc = P_cod_suc;
            CASE 
                WHEN(comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                    UPDATE esmeralda1.sucursal1 SET director_suc = P_nuevo_director_suc WHERE cod_suc = P_cod_suc;
                WHEN(comunidad IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                    UPDATE esmeralda2.sucursal2 SET director_suc = P_nuevo_director_suc WHERE cod_suc = P_cod_suc;
                WHEN(comunidad IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                    UPDATE esmeralda3.sucursal3 SET director_suc = P_nuevo_director_suc WHERE cod_suc = P_cod_suc;
                WHEN(comunidad IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                    UPDATE esmeralda4.sucursal4 SET director_suc = P_nuevo_director_suc WHERE cod_suc = P_cod_suc;
            END CASE;
            DBMS_OUTPUT.PUT_LINE('Director de la sucursal con c骴igo ' || P_cod_suc || ' cambiado a ' || P_nuevo_director_suc);
    END IF; 
END CambiarDirectorSucursal;




CREATE OR REPLACE PROCEDURE AltaNuevoCliente(
    NEW_cod_cli IN NUMBER,
    P_dni_cli IN VARCHAR2,
    P_nombre_cli IN VARCHAR2,
    P_direccion_cli IN VARCHAR2,
    P_tipo_cli IN VARCHAR2,
    P_ca_cli IN VARCHAR2
)
IS
    comunidad_exists NUMBER;
BEGIN
    -- Verificar si la comunidad aut贸noma existe
    SELECT COUNT(*) INTO comunidad_exists FROM sucursal WHERE ca_suc = P_ca_cli;

    IF comunidad_exists = 0 THEN
        raise_application_error(-20107, 'ERROR: No existe una comunidad aut贸noma con ese nombre');
    ELSE
        CASE 
            WHEN(P_ca_cli IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja')) THEN
                INSERT INTO esmeralda1.cliente1 (
                    cod_cli,
                    ca_cli,
                    direccion_cli,
                    dni_cli,
                    nombre_cli,
                    tipo_cli
                )
                VALUES (
                    P_cod_cli,
                    P_ca_cli,
                    P_direccion_cli,
                    P_dni_cli,
                    P_nombre_cli,
                    P_tipo_cli
                );
                
            WHEN(P_ca_cli IN ('Catalu馻', 'Baleares','Pa韘 Valenciano', 'Murcia') ) THEN
                INSERT INTO esmeralda2.cliente2 (
                    cod_cli,
                    ca_cli,
                    direccion_cli,
                    dni_cli,
                    nombre_cli,
                    tipo_cli
                )
                VALUES (
                    P_cod_cli,
                    P_ca_cli,
                    P_direccion_cli,
                    P_dni_cli,
                    P_nombre_cli,
                    P_tipo_cli
                );

            WHEN(P_ca_cli IN ('Galicia', 'Asturias', 'Cantabria', 'Pa韘 Vasco', 'Navarra')) THEN
                INSERT INTO esmeralda3.cliente3 (
                    cod_cli,
                    ca_cli,
                    direccion_cli,
                    dni_cli,
                    nombre_cli,
                    tipo_cli
                )
                VALUES (
                    P_cod_cli,
                    P_ca_cli,
                    P_direccion_cli,
                    P_dni_cli,
                    P_nombre_cli,
                    P_tipo_cli
                );

            WHEN(P_ca_cli IN ('Andaluc韆','Extremadura','Canarias','Ceuta y Melilla')) THEN
                INSERT INTO esmeralda4.cliente4 (
                    cod_cli,
                    ca_cli,
                    direccion_cli,
                    dni_cli,
                    nombre_cli,
                    tipo_cli
                )
                VALUES (
                    P_cod_cli,
                    P_ca_cli,
                    P_direccion_cli,
                    P_dni_cli,
                    P_nombre_cli,
                    P_tipo_cli
                );
        END CASE;

        DBMS_OUTPUT.PUT_LINE('Nuevo cliente con c贸digo ' || P_cod_cliente || ' dado de alta');
    END IF; 
END AltaNuevoCliente;



CREATE OR REPLACE PROCEDURE AltaActualizarSuministro(
    P_cod_cli IN NUMBER,
    P_cod_suc IN NUMBER,
    P_cod_vino IN NUMBER,
    P_fecha_solicitud IN DATE,
    P_can_sum IN NUMBER
)
IS
    suministro_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO suministro_exists
    FROM suministro
    WHERE cod_suc_sum = P_cod_suc
      AND cod_vino_sum = P_cod_vino
      AND fecha_sum = P_fecha_solicitud;

    IF suministro_exists > 0 THEN
        UPDATE suministro
        SET can_sum_sum = can_sum_sum + P_can_sum
        WHERE cod_suc_sum = P_cod_suc
          AND cod_vino_sum = P_cod_vino
          AND fecha_sum = P_fecha_solicitud;
    ELSE
        INSERT INTO suministro (
            can_sum_sum,
            cod_suc_sum,
            cod_vino_sum,
            fecha_sum
        )
        VALUES (
            P_can_sum,
            P_cod_suc,
            P_cod_vino,
            P_fecha_solicitud
        );
    END IF;

    DBMS_OUTPUT.PUT_LINE('Suministro registrado o actualizado para el cliente con c贸digo ' || P_cod_cli);
END AltaActualizarSuministro;



CREATE OR REPLACE PROCEDURE BajaSuministros(
    P_cod_cli IN NUMBER,
    P_cod_suc IN NUMBER,
    P_cod_vino IN NUMBER,
    P_fecha_sum IN DATE DEFAULT NULL
)
IS
BEGIN
    IF P_fecha_suministro IS NOT NULL THEN
        -- Si nos dan la fecha eliminamos suministros concretos 
        DELETE FROM esmeralda1.suministro1 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino AND fecha_sum = P_fecha_sum;
        DELETE FROM esmeralda2.suministro2 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino AND fecha_sum = P_fecha_sum;
        DELETE FROM esmeralda3.suministro3 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino AND fecha_sum = P_fecha_sum;
        DELETE FROM esmeralda4.suministro4 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino AND fecha_sum = P_fecha_sum;
    ELSE
        -- Si no eliminamos todos los suministros de ese vino por la sucursal al cliente
        DELETE FROM esmeralda1.suministro1 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino;
        DELETE FROM esmeralda2.suministro2 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino;
        DELETE FROM esmeralda3.suministro3 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino;
        DELETE FROM esmeralda4.suministro4 WHERE cod_suc_sum = P_cod_suc AND cod_vino_sum = P_cod_vino;
    END IF;

    DBMS_OUTPUT.PUT_LINE('Suministros eliminados para el cliente con c贸digo ' || P_cod_cli);

END BajaSuministros;





CREATE OR REPLACE PROCEDURE AltaPedidoSucursal(
    P_cod_suc_origen IN NUMBER,
    P_cod_suc_destino IN NUMBER,
    P_cod_vino IN NUMBER,
    P_fecha_ped IN DATE,
    P_can_ped IN NUMBER
)
IS
    sucursal_exists NUMBER;
    comunidad esmeralda1.sucursal.ca_suc%TYPE;
BEGIN
    -- Verificar si la sucursal origen existe
    SELECT COUNT(*) INTO sucursal_exists FROM sucursal WHERE cod_suc = P_cod_suc_origen;

    IF sucursal_exists = 0 THEN
        raise_application_error(-20112, 'ERROR: No existe una sucursal de origen con ese c骴igo');
    ELSE
        -- Obtener la comunidad aut贸noma de la sucursal destino
        SELECT ca_suc INTO comunidad FROM sucursal WHERE cod_suc = P_cod_suc_destino;

        CASE
            WHEN comunidad IN ('Castilla-Le髇', 'Castilla-La Mancha', 'Arag髇', 'Madrid', 'La Rioja') THEN
                -- Insertar en la tabla correspondiente de esmeralda1
                INSERT INTO esmeralda1.pedido (cod_cli_ped,
                    cod_sum_ped,
                    fecha_ped,
                    can_ped
                )
                VALUES (

