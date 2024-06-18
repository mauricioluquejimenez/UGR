<?php
session_start();
require_once('php/config.php');

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
    header("Location: index.php");
    exit();
}
?>

<!DOCTYPE html>
<html lang = "es">
<head>
    <title> MRG | Inicio </title>
    <base href = "../pe2/">
    <link rel = "stylesheet" type = "text/css" href = "./css/style.css">
</head>
<body>
    <header>
        <a href = "./index.php"><img id = "logo" src = "./imagenes/logo.png" alt = "Museo Real de Granada"></a>

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
                    <a href="php/altausuarios.php">¿No tienes cuenta? Regístrate</a>
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
                <li class = "menu_item"><a href = "./index.php"> INICIO </a></li>
                <li class = "menu_item"><a href = "./php/coleccion_1.php"> COLECCIÓN </a></li>
                <li class = "menu_item"><a href = "./php/visita.php"> VISITA </a></li>
                <li class = "menu_item"><a href = "./php/exposiciones.php"> EXPOSICIONES </a></li>
                <li class = "menu_item"><a href = "./php/informacion.php"> INFORMACIÓN </a></li>
                <li class = "menu_item"><a href = "./php/experiencias.php"> EXPERIENCIAS </a></li>
            </ul>
        </nav><br>
    </header>

    <main>
        <h2 id = "titulo_inicio">Museo Real de Granada</h2>
        <p style = "margin-left: 10%; margin-right: 10%;">Granada, joya andaluza, es un crisol de culturas y belleza arquitectónica.
            Su famosa Alhambra es solo el comienzo de lo que esta ciudad tiene para ofrecer.
            Este museo captura la esencia de Granada: desde su patrimonio histórico
            hasta sus vibrantes calles y la belleza natural que la envuelve.
            Cada imagen es una invitación a explorar los rincones más emblemáticos de Granada,
            ofreciendo una perspectiva única de su encanto. Disfruten de este viaje visual por una
            ciudad que enamora a primera vista.</p>
        <img id = "portada" src = "./imagenes/portada.jpg" alt = "Museo Real de Granada">
    </main>

    <footer id = "pie_de_página">
        <a href = "./php/contacto.php">Contacto</a>
        <a href = "como_se_hizo.pdf">Cómo se hizo</a>
    </footer>
</body>
</html>
