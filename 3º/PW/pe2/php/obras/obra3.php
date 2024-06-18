<?php
session_start();
require_once('../../php/config.php');

$login = isset($_SESSION['usuario']);

if (isset($_POST['usuario']) && isset($_POST['passwd']))
{
    $usuario = $_POST['usuario'];
    $passwd = $_POST['passwd'];

    $sql = "SELECT usuario, tipo FROM usuarios WHERE usuario = :usuario AND passwd = :passwd";

    try {
        $parametros = $conexion_pdo->prepare($sql);
        $parametros->bindParam(':usuario', $usuario);
        $parametros->bindParam(':passwd', $passwd);
        $parametros->execute();

        if ($parametros->rowCount() > 0)
        {
            $usuarioData = $parametros->fetch(PDO::FETCH_ASSOC);
            $_SESSION['usuario'] = $usuarioData['usuario'];
            $_SESSION['tipo'] = $usuarioData['tipo'];
            $login = true;
        }
    } catch (PDOException $e) {
        echo "Consulta fallida: " . $e->getMessage();
    }
}

if (isset($_POST['logout'])) {
    session_destroy();
    header("Location: ../../index.php");
    exit();
}
?>

<!DOCTYPE html>
<html lang = "es">
    <head>
        <title> MRG | La plaza de Bib-Rambla </title>
        <base href = "../pe2/"> 
        <link rel = "stylesheet" type = "text/css" href = "../../css/style.css">
    </head>

    <body>
        <header>
            <a href = "../../index.php"><img id = "logo" src = "../../imagenes/logo.png" alt = "Museo Real de Granada"></a>
            
            <section id = "login">
            <?php
                if (!$login) {
                    echo '
                    <form id="loginForm" method="POST">
                    <label for="usuario">Nombre de usuario:</label>
                    <input type="text" id="usuario" name="usuario">
                    <label for="passwd">Contraseña:</label>
                    <input type="text" id="passwd" name="passwd">
                    <p><button type="submit">Iniciar Sesión</button></p>
                    <a href="../../php/altausuarios.php">¿No tienes cuenta? Regístrate</a>
                    </form>';
                } else
                {
                    echo '<h2>¡Bienvenido, ' . $_SESSION['usuario'] . '!</h2>';
                    echo '<h3> Tipo: ' . $_SESSION['tipo'] . '</h3>';
                    echo '
                        <form method = "POST">
                        <button type = "submit" name = "logout"> Cerrar Sesión </button>
                        </form>';
                }
            ?>
            </section>
        
            <nav>
                <ul id = "menu">
                    <li class = "menu_item"><a href = "../../index.php"> INICIO </a></li>
                    <li class = "menu_item"><a href = "../coleccion_1.php"> COLECCIÓN </a></li>
                    <li class = "menu_item"><a href = "../visita.php"> VISITA </a></li>
                    <li class = "menu_item"><a href = "../exposiciones.php"> EXPOSICIONES </a></li>
                    <li class = "menu_item"><a href = "../informacion.php"> INFORMACIÓN </a></li>
                    <li class = "menu_item"><a href = "../experiencias.php"> EXPERIENCIAS </a></li>
                </ul>
            </nav>
        </header>

        <div id = "contenido">
            <main id = "obras_coleccion">
                <figure id = "obra">
                    <figcaption id = "titulo_obras"> La plaza de Bib-Rambla, Luis Muriel (1834) </figcaption>
                    <img src = "../../imagenes/obras/obra3.jpg" alt = "La plaza de Bib-Rambla">
                </figure>

                <p id = "descripcion"> Esta pintura muestra la famosa plaza de Granada, un lugar central en la vida social y económica de la ciudad, destacando su arquitectura y actividad cotidiana. </p>
            </main>        

            <aside id = "obras_relacionadas">
                <h2> Obras relacionadas </h2>
                <ul>
                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra4.php"><img src = "../../imagenes/obras/obra4.jpg" alt = "Retrato del duque de San Pedro de Galatino"></a>
                            <figcaption> Retrato del duque de San Pedro de Galatino, Gabriel Morcillo Raya (1922) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra5.php"><img src = "../../imagenes/obras/obra5.jpg" alt = "Retrato de dama con loro"></a>
                            <figcaption> Retrato de dama con loro, Joaquín de la Rosa (1839) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra6.php"><img src = "../../imagenes/obras/obra6.jpg" alt = "Monumento a Isidoro Máiquez"></a>
                            <figcaption> Monumento a Isidoro Máiquez, F. Pérez (1838) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra7.php"><img src = "../../imagenes/obras/obra7.jpg" alt = "Sereno"></a>
                            <figcaption> Sereno, Luis Miguel (1834) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra8.php"><img src = "../../imagenes/obras/obra8.jpg" alt = "Ayuntamiento Viejo de Granada"></a>
                            <figcaption> Ayuntamiento Viejo de Granada, Mariano Fortuny y Marsal (1873) </figcaption>
                        </figure>
                    </li>
                </ul>
            </aside>
        </div>

        <footer id = "pie_de_página">
            <a href = "../contacto.php"> Contacto </a>
            <a href = "../../como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>