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
        <title> MRG | Mariano Fortuny y Marsal </title>
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
            <aside id = "colecciones">
                <div id = "autores">
                    <h3> Autores </h3>
                    <ul>
                        <li><a href = "../colecciones/coleccion_cano.php"> Alonso Cano </a></li>
                        <li><a href = "../colecciones/coleccion_dutailly.php"> Dutailly </a></li>
                        <li><a href = "../colecciones/coleccion_perez.php"> F. Pérez </a></li>
                        <li><a href = "../colecciones/coleccion_chacon.php"> Francisco Chacón </a></li>
                        <li><a href = "../colecciones/coleccion_dalmau.php"> Francisco Dalmau </a></li>
                        <li><a href = "../colecciones/coleccion_cotan.php"> Fray Juan Sánchez Cotán </a></li>
                        <li><a href = "../colecciones/coleccion_morcillo.php"> Gabriel Morcillo Raya </a></li>
                        <li><a href = "../colecciones/coleccion_indaco.php"> Jacobo Florentino, el Indaco </a></li>
                        <li><a href = "../colecciones/coleccion_delarosa.php"> Joaquín de la Rosa </a></li>
                        <li><a href = "../colecciones/coleccion_almodovar.php"> José Ruiz de Almodóvar </a></li>
                        <li><a href = "../colecciones/coleccion_ruano.php"> J. Ruano </a></li>
                        <li><a href = "../colecciones/coleccion_muriel.php"> Luis Muriel </a></li>
                        <li><a href = "../colecciones/coleccion_fortuny.php"> Mariano Fortuny y Marsal </a></li>
                        <li><a href = "../colecciones/coleccion_raxis.php"> Pedro de Raxis </a></li>
                    </ul>
                </div>
                
                <div id = "epocas">
                    <h3> Épocas </h3>
                    <ul>
                        <li><a href = "../colecciones/coleccion_XV.php"> Siglo XV </a></li>
                        <li><a href = "../colecciones/coleccion_XVI.php"> Siglo XVI </a></li>
                        <li><a href = "../colecciones/coleccion_XVII.php"> Siglo XVII </a></li>
                        <li><a href = "../colecciones/coleccion_XVIII.php"> Siglo XVIII </a></li>
                        <li><a href = "../colecciones/coleccion_XIX.php"> Siglo XIX </a></li>
                        <li><a href = "../colecciones/coleccion_XX.php"> Siglo XX </a></li>
                    </ul>
                </div>
    
                <div id = "temas">
                    <h3> Temas </h3>
                    <ul>
                        <li><a href = "../colecciones/coleccion_retratos.php"> Retratos y figuras notables </a></li>
                        <li><a href = "../colecciones/coleccion_cultura.php"> Cultura y vida cotidiana </a></li>
                        <li><a href = "../colecciones/coleccion_religion.php"> Religión y espiritualidad </a></li>
                        <li><a href = "../colecciones/coleccion_arte.php"> Arte histórico y documental </a></li>
                    </ul>
                </div>
            </aside>
    
            <main id = "obras_coleccion">
                <h1 id = "titulo_obras"> Explora la colección </h1>
    
                <div class = "row">
                    <figure class = "obra_coleccion">
                        <figcaption> Ayuntamiento Viejo de Granada, Mariano Fortuny y Marsal (1873) </figcaption>
                        <a href = "../obras/obra8.php"><img src = "../../imagenes/obras/obra8.jpg" alt = "Ayuntamiento Viejo de Granada"></a>
                    </figure>
                </div>
            </main>
        </div>

        <footer id = "pie_de_página">
            <a href = "../contacto.php"> Contacto </a>
            <a href = "../../como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>