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
        <title> MRG | Contacto </title>
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

        <table id = "contacto">
            <thead>
                <tr><th colspan = "2"> Datos del desarrollador </th></tr>
            </thead>

            <tbody>
                <tr>
                    <th> Nombre completo </th>
                    <td> Mauricio Luque Jiménez</td>
                </tr>

                <tr>
                    <th> Centro de estudios </th>
                    <td> Escuela Técnica Superior de Ingeniería Informática y Telecomunicaciones </td>
                </tr>

                <tr>
                    <th> Grado </th>
                    <td> Ingeniería Informática </td>
                </tr>

                <tr>
                    <th> Especialidad </th>
                    <td> Sistemas de Información </td>
                </tr>

                <tr>
                    <th> Curso </th>
                    <td> 3º </td>
                </tr>

                <tr>
                    <th> Asignatura </th>
                    <td> Programación Web </td>
                </tr>

                <tr>
                    <th> Subgrupo de prácticas </th>
                    <td> A3 </td>
                </tr>

                <tr>
                    <th> Correo electrónico </th>
                    <td> maulujim@correo.ugr.es </td>
                </tr>
            </tbody>
        </table>

        <footer id = "pie_de_página">
            <a href = "../php/contacto.php"> Contacto </a>
            <a href = "../como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>