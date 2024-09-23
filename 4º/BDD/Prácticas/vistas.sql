CREATE VIEW clientes AS
    (SELECT * FROM esmeralda1.cliente)
  UNION
    (SELECT * FROM esmeralda2.cliente)
  UNION
    (SELECT * FROM esmeralda3.cliente)
  UNION
    (SELECT * FROM esmeralda4.cliente);

-- Vista de la tabla empleado

CREATE VIEW empleados AS
    (SELECT * FROM esmeralda1.empleado)
  UNION
    (SELECT * FROM esmeralda2.empleado)
  UNION
    (SELECT * FROM esmeralda3.empleado)
  UNION
    (SELECT * FROM esmeralda4.empleado);

-- Vista de la tabla Encarga

CREATE VIEW pedidos AS
    (SELECT * FROM esmeralda1.pedido)
  UNION
    (SELECT * FROM esmeralda2.pedido)
  UNION
    (SELECT * FROM esmeralda3.pedido)
  UNION
    (SELECT * FROM esmeralda4.pedido);

-- Vista de la tabla Suscursal

CREATE VIEW sucursales AS
    (SELECT * FROM esmeralda1.sucursal)
  UNION
    (SELECT * FROM esmeralda2.sucursal)
  UNION
    (SELECT * FROM esmeralda3.sucursal)
  UNION
    (SELECT * FROM esmeralda4.sucursal);

-- Vista de la tabla Almacena

CREATE VIEW almacenes AS
    (SELECT * FROM esmeralda1.almacen)
  UNION
    (SELECT * FROM esmeralda2.almacen)
  UNION
    (SELECT * FROM esmeralda3.almacen)
  UNION
    (SELECT * FROM esmeralda4.almacen);

-- Vista de la tabla Vino

CREATE VIEW vinos AS
    (SELECT * FROM esmeralda1.vino)
  UNION
    (SELECT * FROM esmeralda2.vino)
  UNION
    (SELECT * FROM esmeralda3.vino)
  UNION
    (SELECT * FROM esmeralda4.vino);
COMMIT;
