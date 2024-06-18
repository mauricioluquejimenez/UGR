CREATE TABLE usuarios (
    usuario VARCHAR(50) PRIMARY KEY,
    correo VARCHAR(100) NOT NULL UNIQUE,
    telefono VARCHAR(20) NOT NULL,
    passwd VARCHAR(255) NOT NULL,
    visita VARCHAR(3),
    comentario VARCHAR(500),
    edad VARCHAR(255),
    tipo VARCHAR(6)
);
