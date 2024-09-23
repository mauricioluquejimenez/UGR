-- 3. El director de una sucursal es empleado de la compañía (no tiene que ser empleado de la sucursal)
create trigger director_sucursal
before insert or update of director_suc on sucursal for each row

declare
dir_suc varchar(255);

begin
    select director_suc on dir_suc from sucursal, empleado where :new.cod_suc = empleado.cod_suc_emp and :new.director_suc = empleado.nombre_emp
    if(dir_suc is not null) then
        raise_application_error(-20007, 'El empleado de la compañía no puede ser director de la sucursal porque ya es empleado de la misma');
    endif;
end;
/

/* 6. El salario de un empleado nunca puede disminuir */
create trigger disminucion_salario
before update of salario_emp on empleado for each row

begin
    if (new.salario_emp < old.salario_emp) then
        raise_application_error(-20007, 'No se permite disminuir el salario del empleado');
    end if;
end;
/

/* 7.La sucursal donde trabaja un empleado debe existir */
create trigger buscar_sucursal
before insert or update of cod_suc_emp on empleado for each row

begin
    if (:new.cod_suc_emp not in (select cod_suc_emp from empleado))) then
        raise_aplication_error(-20007, 'La sucursal introducida no existe');
    end if;
end;
/

/* 9. Un cliente puede solicitar a la compañía suministros de cualquier vino, pero siempre tendrá que hacerlo a través de sucursales de la delegación a la que pertenece su comunidad autónoma*/

create trigger cliente_delegacion
before insert on pedido

declare
ca_cliente varchar(50);
ca_sucursal varchar(50);

begin
    select ca_cli into ca_cliente  from cliente, pedido              where :new.cod_cli_ped = cliente.cod_cli;
    select ca_suc into ca_sucursal from pedido, sucursal, suministro where :new.cod_sum_ped = suministro.cod_sum and suministro.cod_suc_sum = sucursal.cod_suc;
    
    if((((ca_cliente = 'Castilla-León') or (ca_cliente = 'Castilla-La Mancha') or (ca_cliente = 'Aragón') or (ca_cliente = 'Madrid') or (ca_cliente = 'La Rioja')) and
        ((ca_sucursal  <> 'Castilla-León') and (ca_sucursal  <> 'Castilla-La Mancha') and (ca_sucursal  <> 'Aragón') and (ca_sucursal  <> 'Madrid') and (ca_sucursal  <> 'La Rioja'))) or
        (((ca_cliente = 'Cataluña') or (ca_cliente = 'Baleares') or (ca_cliente = 'País Valenciano') or (ca_cliente = 'Murcia')) and
        ((ca_sucursal   <> 'Cataluña') and (ca_sucursal  <> 'Baleares') and (ca_sucursal  <> 'País Valenciano') and (ca_sucursal  <> 'Murcia'))) and
        (((ca_cliente = 'Galicia') or (ca_cliente = 'Asturias') or (ca_cliente = 'Cantabria') or (ca_cliente = 'País Vasco') or (ca_cliente = 'Navarra')) and
        ((ca_sucursal   <> 'Galicia') and (ca_sucursal  <> 'Asturias') and (ca_sucursal  <> 'Cantabria') and (ca_sucursal  <> 'País Vasco') and (ca_sucursal  <> 'Navarra'))) or
        (((ca_cliente = 'andalucía') or (ca_cliente = 'Extremadura') or (ca_cliente = 'Canarias') or (ca_cliente = 'Ceuta') or (ca_cliente = 'Melilla')) and
        ((ca_sucursal   <> 'andalucía') and (ca_sucursal  <> 'Extremadura') and (ca_sucursal  <> 'Canarias') and (ca_sucursal  <> 'Ceuta') and (ca_sucursal  <> 'Melilla')))) then
        raise_application_error(-20007, 'No se puede realizar pedidos a la delegación de esta sucursal');
    end if;
end;
/

/* 10. Para cada cliente, la fecha de un suministro tendrá que ser siempre igual o posterior a la fecha de su último suministro*/
create trigger comprobar_fecha_pedido
before insert on pedido

declare
fecha date;

begin
    select max(fecha_ped) into fecha from pedido;
    if (:new.fecha_ped < fecha) then
        raise_application_error(-20007, 'La fecha del nuevo pedido es incorrecta');
    end if;
end;
/

/* 11. No se puede suministrar un vino que no existe.*/
create trigger buscar_vino
before insert or update of cod_vino_sum on suministro for each row

begin
    if (:new.cod_vino_sum not in (select cod_vino_sum from suministro))) then
        raise_aplication_error(-20007, 'El vino introducido no existe');
    end if;
end;
/

/* 15. Los datos referentes a un vino solamente podrán eliminarse de la base de datos si la cantidad total suministrada de ese vino es 0 (o nunca ha sido suministrado).*/
create trigger eliminar_vino
before delete on vino

begin
    if (old.can_sum_vino > 0) then
        raise_application_error(-20007, 'No se puede eliminar el vino');
    end if;
end;
/

/* 16.Los datos referentes a un productor solamente podrán eliminarse de la base de datos si para cada vino que produce, la cantidad total suministrada es 0 o no existe ningún suministro.*/
create trigger eliminar_productor
before delete on productor

declare
total_sum int;

begin 
    select count(*) into total_sum from vino where cod_pro_vino = :old.cod_pro_vino and can_sum_vino <> 0;
    if (total_sum > 0) then
        raise_application_error(-20007, 'No se puede eliminar al productor');
    end if;
end;
/

/* 17.Una sucursal no puede realizar pedidos a sucursales de su misma delegación */
create trigger misma_delegacion
before insert on distribucion

declare
ca_compra varchar(50);
ca_vende varchar(50);

begin
    select ca_suc into ca_compra from distribucion, sucursal             where :new.cod_suc_dis = sucursal.cod_suc;
    select ca_suc into ca_vende  from distribucion, sucursal, suministro where :new.cod_sum_dis = suministro.cod_sum and suministro.cod_suc_sum = sucursal.cod_suc;

    if((((ca_compra = 'Castilla-León') or (ca_compra = 'Castilla-La Mancha') or (ca_compra = 'Aragón') or (ca_compra = 'Madrid') or (ca_compra = 'La Rioja')) and
        ((ca_vende  <> 'Castilla-León') and (ca_vende  <> 'Castilla-La Mancha') and (ca_vende  <> 'Aragón') and (ca_vende  <> 'Madrid') and (ca_vende  <> 'La Rioja'))) or
        (((ca_compra = 'Cataluña') or (ca_compra = 'Baleares') or (ca_compra = 'País Valenciano') or (ca_compra = 'Murcia')) and
        ((ca_vende   <> 'Cataluña') and (ca_vende  <> 'Baleares') and (ca_vende  <> 'País Valenciano') and (ca_vende  <> 'Murcia'))) and
        (((ca_compra = 'Galicia') or (ca_compra = 'Asturias') or (ca_compra = 'Cantabria') or (ca_compra = 'País Vasco') or (ca_compra = 'Navarra')) and
        ((ca_vende   <> 'Galicia') and (ca_vende  <> 'Asturias') and (ca_vende  <> 'Cantabria') and (ca_vende  <> 'País Vasco') and (ca_vende  <> 'Navarra'))) or
        (((ca_compra = 'andalucía') or (ca_compra = 'Extremadura') or (ca_compra = 'Canarias') or (ca_compra = 'Ceuta') or (ca_compra = 'Melilla')) and
        ((ca_vende   <> 'andalucía') and (ca_vende  <> 'Extremadura') and (ca_vende  <> 'Canarias') and (ca_vende  <> 'Ceuta') and (ca_vende  <> 'Melilla')))) then
        raise_application_error(-20007, 'No se puede realizar pedidos a la delegación de esta sucursal');
    end if;
end;
/
/* 18.La cantidad total de cada vino que las sucursales piden a otras sucursales, no puede exceder la cantidad total que de ese vino solicitan los clientes.*/
create trigger cantidad_vino
before insert on suministro for each row

declare
cantidad_dis int;
cantidad_ped int;

begin
    select sum(cantidad_dis) into cantidad_dis from distribucion, suministro where :new.cod_sum = distribucion.cod_sum_dis group by cod_vino_sum;
    select sum(cantidad_ped) into cantidad_ped from suministro, pedido where :new.cod_sum = pedido.cod_sum_ped group by cod_vino_sum;
    
    if(cantidad_dis > cantidad_ped) then
        raise_application_error(-20007, 'Cantidad de vino distribuida excedida');
    end if;
end;
/

/* 19.La sucursal a la que otra se dirige para hacer pedidos de vinos que ella no distribuye, tiene que suministrar directamente el vino que se solicita */
create trigger 
before insert on distribucion

declare
ca_vino varchar(50);
ca_suc_sum varchar(50);

begin
    select ca_vino into ca_vino     from distribucion, suministro, vino     where :new.cod_suc_dis = suministro.cod_sum and suministro.cod_vino_sum = vino.cod_vino;
    select ca_suc  into ca_suc_sum  from distribucion, sucursal, suministro where :new.cod_suc_dis = suministro.cod_sum and suministro.cod_suc_sum = sucursal.cod_suc;
    
    if((((ca_vino = 'Castilla-León') or (ca_vino = 'Castilla-La Mancha') or (ca_vino = 'Aragón') or (ca_vino = 'Madrid') or (ca_vino = 'La Rioja')) and
        ((ca_suc_sum  <> 'Castilla-León') and (ca_suc_sum  <> 'Castilla-La Mancha') and (ca_suc_sum  <> 'Aragón') and (ca_suc_sum  <> 'Madrid') and (ca_suc_sum  <> 'La Rioja'))) or
        (((ca_vino = 'Cataluña') or (ca_vino = 'Baleares') or (ca_vino = 'País Valenciano') or (ca_vino = 'Murcia')) and
        ((ca_suc_sum   <> 'Cataluña') and (ca_suc_sum  <> 'Baleares') and (ca_suc_sum  <> 'País Valenciano') and (ca_suc_sum  <> 'Murcia'))) and
        (((ca_vino = 'Galicia') or (ca_vino = 'Asturias') or (ca_vino = 'Cantabria') or (ca_vino = 'País Vasco') or (ca_vino = 'Navarra')) and
        ((ca_suc_sum   <> 'Galicia') and (ca_suc_sum  <> 'Asturias') and (ca_suc_sum  <> 'Cantabria') and (ca_suc_sum  <> 'País Vasco') and (ca_suc_sum  <> 'Navarra'))) or
        (((ca_vino = 'andalucía') or (ca_vino = 'Extremadura') or (ca_vino = 'Canarias') or (ca_vino = 'Ceuta') or (ca_vino = 'Melilla')) and
        ((ca_suc_sum   <> 'andalucía') and (ca_suc_sum  <> 'Extremadura') and (ca_suc_sum  <> 'Canarias') and (ca_suc_sum  <> 'Ceuta') and (ca_suc_sum  <> 'Melilla')))) then
        raise_application_error(-20007, 'No se puede realizar pedidos a la delegación de esta sucursal');
    end if;
end;
/

/* 20. La fecha del pedido de una sucursal S1 a otra S2 de un determinado vino, tiene que ser posterior a la fecha del último pedido que S1 haya cursado a S2 de ese vino */
create trigger comprobar_fecha_distribucion
before insert on distribucion

declare
fecha date;

begin
    select max(fecha_dis) into fecha from distribucion group by vino;
    if (:new.fecha_dis < fecha) then
        raise_application_error(-20007, 'La fecha del nuevo pedido entre sucursales es incorrecta');
    end if;
end;
/