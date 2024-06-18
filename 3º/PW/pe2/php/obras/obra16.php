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
        <title> MRG | San Diego de Alcalá </title>
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
                    <figcaption id = "titulo_obras"> San Diego de Alcalá, Alonso Cano (1653-1657) </figcaption>
                    <img src = "../../imagenes/obras/obra16.jpg" alt = "San Diego de Alcalá">
                </figure>

                <p id = "descripcion"> Este retrato de San Diego refleja la importancia de este santo en la devoción popular española. </p>
            </main>        

            <aside id = "obras_relacionadas">
                <h2> Obras relacionadas </h2>
                <ul>
                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra17.php"><img src = "../../imagenes/obras/obra17.jpg" alt = "Decoraciones para las fiestas del Corpus"></a>
                            <figcaption> Decoraciones para las fiestas del Corpus, J. Ruano (1850) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra18.php"><img src = "../../imagenes/obras/obra18.jpg" alt = "Mapa topográfico de la ciudad de Granada"></a>
                            <figcaption> Mapa topográfico de la ciudad de Granada, Francisco Dalmau (1796)</figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra1.php"><img src = "../../imagenes/obras/obra1.jpg" alt = "Retrato de Ángel Ganivet"></a>
                            <figcaption> Retrato de Ángel Ganivet, José Ruiz de Almodóvar (Siglo XIX) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra2.php"><img src = "../../imagenes/obras/obra2.jpg" alt = "Danza del bolero en Granada"></a>
                            <figcaption> Danza del bolero en Granada, Dutailly (1805) </figcaption>
                        </figure>
                    </li>

                    <li>
                        <figure class = "obra_relacionada">
                            <a href = "../obras/obra3.php"><img src = "../../imagenes/obras/obra3.jpg" alt = "La plaza de Bib-Rambla"></a>
                            <figcaption> La plaza de Bib-Rambla, Luis Muriel (1834) </figcaption>
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