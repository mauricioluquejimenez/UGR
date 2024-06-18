DROP TABLE pedido;
DROP TABLE distribucion;
DROP TABLE almacen;
DROP TABLE vino;
DROP TABLE empleado;
DROP TABLE sucursal;
DROP TABLE productor;
DROP TABLE cliente;

-- Relación CLIENTE
create table cliente (
    cod_cli                     number          primary key,
    
    ca_cli                      varchar(50),
    direccion_cli               varchar(255),
    dni_cli                     varchar(9) unique not null,
    nombre_cli                  varchar(255),
    tipo_cli                    varchar(50)
    
    constraint tipo check (tipo_cli in ('A', 'B', 'C'))
);

-- Relación PRODUCTOR
create table productor (
    cod_pro                     number          primary key,
    
    direccion_pro               varchar(255),
    dni_pro                     varchar(9),
    nombre_pro                  varchar(255)
);

-- Relación SUCURSAL
create table sucursal (
    cod_suc                     number          primary key,
    
    ciudad_suc                  varchar(255),
    ca_suc                      varchar(50),
    director_suc                number          unique,
    nombre_suc                  varchar(255)
);

-- Relación EMPLEADO
create table empleado (
    cod_emp                     number          primary key,
    
    cod_suc_emp                 number          not null,
    direccion_emp               varchar(255),
    dni_emp                     varchar(9)      unique not null,
    fecha_ini_contrato_emp      date            default sysdate     not null,
    nombre_emp                  varchar(255)    not null,
    salario_emp                 number          default 0           not null,
    
    foreign key(cod_suc_emp)    references sucursal(cod_suc)
);

-- Relación VINO
create table vino (
    cod_vino                    number          primary key,
    
    can_prod_vino               number          default 0,
    can_stock_vino              number          default 0,          
    cod_pro_vino                number          not null,
    ca_vino                     varchar(50),
    cosecha_vino                number,
    grad_vino                   decimal(5, 2),
    marca_vino                  varchar(255),
    origen_vino                 varchar(255),
    vinedo_vino                 varchar(255),
    
    foreign key (cod_pro_vino)  references productor (cod_pro),
    
    constraint stock check (can_stock_vino between 0 and can_prod_vino)
);

-- Relación DISTRIBUCION
create table distribucion (
    cod_suc_compra_dis          number,
    cod_suc_vende_dis           number,
    cod_vino_dis                number,
    can_dis                     number          default 0,
    fecha_dis                   date            default sysdate,
    
    foreign key (cod_suc_compra_dis)    references sucursal (cod_suc),
    
    primary key(cod_suc_compra_dis, cod_suc_vende_dis, cod_vino_dis, fecha_dis)
);

-- Relación PEDIDO
create table pedido (
    can_ped                     number          default 0,
    cod_cli_ped                 number,
    cod_suc_ped                 number,
    cod_vino_ped                number,
    fecha_ped                   date            default sysdate,
    
    foreign key (cod_cli_ped)    references cliente (cod_cli),
    foreign key (cod_suc_ped)    references sucursal (cod_suc),
    
    primary key(cod_cli_ped, cod_suc_ped, cod_vino_ped, fecha_ped)
);

commit;