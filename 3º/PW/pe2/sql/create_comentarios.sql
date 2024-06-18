CREATE TABLE comentarios (
    usuario VARCHAR(50),
    recomendacion VARCHAR(3),
    comentario VARCHAR(500),

    PRIMARY KEY (usuario, comentario)
);