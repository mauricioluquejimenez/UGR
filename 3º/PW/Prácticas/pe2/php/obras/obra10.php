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
        <title> MRG | Inmaculada Concepción </title>
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
                    <figcaption id = "titulo_obras"> Inmaculada Concepción, Fray Juan Sánchez Cotán (1617-1618) </figcaption>
                    <img src = "../../imagenes/obras/obra10.jpg" alt = "Inmaculada Concepción">
                </figure>

                <p id = "descripcion"> Una obra religiosa que representa la pena y el dolor de la Virgen María, reflejando la profundidad del arte religioso del Renacimiento. </p>
            </main>        

            <aside id = "obras_relacionadas">
                <h2> Obras relacionadas </h2>
                <ul>
                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra11.php"><img src = "../../imagenes/obras/obra11.jpg" alt = "San Juan Evangelista en la isla de Patmos"></a>
                            <figcaption> San Juan Evangelista en la isla de Patmos, Alonso Cano (1645) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra12.php"><img src = "../../imagenes/obras/obra12.jpg" alt = "Sereno"></a>
                            <figcaption> El Entierro de Cristo, Jacobo Florentino, el Indaco (1521-1526) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra13.php"><img src = "../../imagenes/obras/obra13.jpg" alt = "Aparición de la Virgen a San Jacinto"></a>
                            <figcaption> Aparición de la Virgen a San Jacinto, Pedro de Raxis (1600)</figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra14.php"><img src = "../../imagenes/obras/obra14.jpg" alt = "Bodegón con cardo y zanahorias"></a>
                            <figcaption> Bodegón con cardo y zanahorias, Fray Juan Sánchez Cotán (1603-1627) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra15.php"><img src = "../../imagenes/obras/obra15.jpg" alt = "San Juan de Capistrano y San Bernardino de Siena"></a>
                            <figcaption> San Juan de Capistrano y San Bernardino de Siena, Alonso Cano (1652-1657) </figcaption>
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