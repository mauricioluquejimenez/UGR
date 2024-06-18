-- 12. Un vino solamente puede producirlo un productor.

-- 13. Un vino no puede existir si no existe un productor que lo produzca

-- 14.El stock de un vino nunca puede ser negativo ni mayor que la cantidad producida

-- 15. Los datos referentes a un vino solamente podrán eliminarse de la base de datos si la cantidad total suministrada de ese vino es 0 (o nunca ha sido suministrado).

CREATE TRIGGER eliminar_vino
BEFORE DELETE ON vino

BEGIN
    IF (OLD.can_sum_vino > 0) THEN
        RAISE_APPLICATION_ERROR(-20007, 'No se puede eliminar el vino');
    END IF;
END;

-- 16.Los datos referentes a un productor solamente podrán eliminarse de la base de datos si para cada vino que produce, la cantidad total suministrada es 0 o no existe ningún suministro.

-- 17.Una sucursal no puede realizar pedidos a sucursales de su misma delegación

-- 18.La cantidad total de cada vino que las sucursales piden a otras sucursales, no puede exceder la cantidad total que de ese vino solicitan los clientes.

-- 19.La sucursal a la que otra se dirige para hacer pedidos de vinos que ella no distribuye, tiene que suministrar directamente el vino que se solicita; es decir, si, por ejemplo, una sucursal de Andalucía requiere vino de Rioja, tiene que solicitarlo, necesariamente, a una sucursal de la delegación de Madrid. 

-- 20. La fecha del pedido de una sucursal S1 a otra S2 de un determinado vino, tiene que ser posterior a la fecha del último pedido que S1 haya cursado a S2 de ese vino

-- 21. La fecha de pedido de un vino de una sucursal S1 a otra S2, tiene que ser posterior a la última fecha de solicitud de suministro de ese mismo vino recibida en S1 por un cliente. Por ejemplo, si un cliente de Andalucía solicita suministro de vino de Rioja a la sucursal S1 en fecha F, y esa solicitud es la última que S1 ha recibido de vino de Rioja, el pedido de S1 a la sucursal de la delegación de Madrid correspondiente tiene que ser de fecha posterior a F.
