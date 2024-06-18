<?php
session_start();
require_once('../php/config.php');

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
    header("Location: ../index.php");
    exit();
}
?>

<!DOCTYPE html>
<html lang = "es">
    <head>
        <title> MRG | Visita </title>
        <base href = "../pe2/">
        <link rel = "stylesheet" type = "text/css" href = "../css/style.css"> 
    </head>

    <body>
        <header>
            <a href = "../index.php"><img id = "logo" src = "../imagenes/logo.png" alt = "Museo Real de Granada"></a>
            
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
                    <a href="../php/altausuarios.php">¿No tienes cuenta? Regístrate</a>
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
                    <li class = "menu_item"><a href = "../index.php"> INICIO </a></li>
                    <li class = "menu_item"><a href = "../php/coleccion_1.php"> COLECCIÓN </a></li>
                    <li class = "menu_item"><a href = "../php/visita.php"> VISITA </a></li>
                    <li class = "menu_item"><a href = "../php/exposiciones.php"> EXPOSICIONES </a></li>
                    <li class = "menu_item"><a href = "../php/informacion.php"> INFORMACIÓN </a></li>
                    <li class = "menu_item"><a href = "../php/experiencias.php"> EXPERIENCIAS </a></li>
                </ul>
            </nav>
        </header>

        <h1 id = "titulo_visita"> Bienvenidos al Museo Real de Granada </h1>

        <h2>Sala 1 - Granada Nazarí: Esencia del Siglo XV</h2>
        <section class = "sala">
            <p>Antes de la transición a la era moderna, esta sala se sumerge en el último siglo del Reino Nazarí de Granada. Exhibiendo una selección exquisita de arte islámico, incluye azulejos intrincadamente decorados, textiles y manuscritos que ilustran la sofisticación y el refinamiento cultural de Granada en el siglo XV. Las obras destacan la habilidad y el arte de los artesanos granadinos de la época, con especial atención a la Alhambra, cumbre del arte islámico en España, mostrando su influencia duradera en el arte y la arquitectura.</p>
            <img src = "../imagenes/salas/sala1.jpg" alt = "Mapa de la sala 1">
        </section>

        <h2>Sala 2 - Granada Renacentista: Esplendor del Siglo XVI</h2>
        <section class = "sala"> 
            <p>Esta sala del museo se dedica a explorar Granada durante el Renacimiento, un periodo de transición cultural y artística tras la Reconquista. Aquí se exhiben obras que reflejan la fusión de influencias islámicas y cristianas, con pinturas de artistas como Pedro Machuca y esculturas que adornaban lugares emblemáticos de la ciudad. Los frescos y retablos muestran un vívido retrato de la vida y la religiosidad de la época.</p>
            <img src = "../imagenes/salas/sala2.jpg" alt = "Mapa de la sala 2">
        </section>

        <h2>Sala 3 - Barroco Granadino: El Siglo XVII en Lienzo y Piedra</h2>
        <section class = "sala">
            <p>El siglo XVII en Granada es retratado a través de la grandiosidad del Barroco. En esta sala, los visitantes pueden admirar las dramáticas expresiones del arte barroco en obras de Alonso Cano, quien jugó un papel crucial en la decoración de la Catedral de Granada. Las pinturas y esculturas expuestas capturan la intensidad emocional y la profundidad espiritual características del periodo, siendo un testimonio de la riqueza cultural de la ciudad.</p>
            <img src = "../imagenes/salas/sala3.jpg" alt = "Mapa de la sala 3">
        </section>

        <h2>Sala 4 - Ilustración y Cambio: Granada en el Siglo XVIII</h2>
        <section class = "sala">
            <p>Esta sala ofrece una visión de Granada durante el Siglo de las Luces, destacando cómo la ciudad se adaptó a los ideales de la Ilustración. Se destacan mapas antiguos, grabados y obras de arte que ilustran los cambios urbanísticos y sociales. La colección incluye también instrumentos científicos de la época, reflejando el creciente interés por la ciencia y la educación en Granada.</p>
            <img src = "../imagenes/salas/sala4.jpg" alt = "Mapa de la sala 4">
        </section>

        <h2>Sala 5 - Romanticismo y Revolución: Granada del Siglo XIX</h2>
        <section class = "sala">
            <p>El siglo XIX está representado en esta sala a través de la lente del Romanticismo y los cambios políticos de la época. Obras que capturan la belleza dramática de la Alhambra y las pinturas costumbristas de Manuel Gómez-Moreno González ofrecen una visión nostálgica pero crítica de la Granada de ese tiempo. Los visitantes pueden sumergirse en una época donde la lucha por la identidad y la modernización moldearon la ciudad.</p>
            <img src = "../imagenes/salas/sala5.jpg" alt = "Mapa de la sala 5">
        </section>

        <h2>Sala 6 - Granada Modernista: Arte y Sociedad en el Siglo XX</h2>
        <section class = "sala">
            <p>La evolución de Granada en el siglo XX se explora en esta sala a través del Modernismo y otras corrientes artísticas contemporáneas. Aquí se muestran desde los vibrantes cuadros de José Guerrero hasta las influencias del Cubismo y el Surre </p>
            <img src = "../imagenes/salas/sala6.jpg" alt = "Mapa de la sala 6">
        </section>

        <footer id = "pie_de_página">
            <a href = "../php/contacto.php"> Contacto </a>
            <a href = "../como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>