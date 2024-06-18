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
<html>
    <head>
        <title> MRG | Nuevo usuario </title>
        <base href = "../pe2/">
        <link rel = "stylesheet" type = "text/css" href = "../css/style.css">
        <script src="../js/validation.js"></script>
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

        <?php
            if (empty($_POST["consulta"]))
                $_POST["consulta"] = 0;

            $_POST["consulta"]++;
        ?>

        <form id = "signup" action = "" method = "post">
        <input type = "hidden" name = "consulta" value = "<?php echo $_POST['consulta']; ?>"/>
            <?php
                if ($_POST["consulta"] == 1)
                {
                    echo '<label for = "usuario_signup"> Nombre de usuario </label><br>';
                    echo '<input type = "text" name ="usuario_signup" id = "usuario_signup" value = ""/><br>';
                }
                else
                {
                    $valor = empty($_POST['usuario_signup']) ? NULL : $_POST['usuario_signup'];
                    echo "<input type = 'hidden' name = 'usuario_signup' id = 'usuario_signup' value = '$valor'>";
                }
            ?>

            <?php
                if ($_POST["consulta"] == 2)
                {
                    echo '<label for = "correo_signup"> Correo electrónico </label><br>';
                    echo '<input type = "text" name ="correo_signup" id = "correo_signup" value = ""/><br>';
                }
                else
                {
                    $valor = empty($_POST['correo_signup']) ? NULL : $_POST['correo_signup'];
                    echo "<input type = 'hidden' name = 'correo_signup' id = 'correo_signup' value = '$valor'>";
                }
            ?>

            <?php
                if ($_POST["consulta"] == 3)
                {
                    echo '<label for = "passwd_signup"> Contraseña </label><br>';
                    echo '<input type = "text" name ="passwd_signup" id = "passwd_signup" value = ""/><br>';
                }
                else
                {
                    $valor = empty($_POST['passwd_signup']) ? NULL : $_POST['passwd_signup'];
                    echo "<input type = 'hidden' name = 'passwd_signup' id = 'passwd_signup' value = '$valor'>";
                }
            ?>            

            <?php
                if ($_POST["consulta"] == 4)
                {
                    echo '<label for = "telefono_signup"> Número de teléfono </label><br>';
                    echo '<input type = "text" name ="telefono_signup" id = "telefono_signup" value = ""/><br>';
                }
                else
                {
                    $valor = empty($_POST['telefono_signup']) ? NULL : $_POST['telefono_signup'];
                    echo "<input type = 'hidden' name = 'telefono_signup' id = 'telefono_signup' value = '$valor'>";
                }
            ?>

            <?php
                if ($_POST["consulta"] == 5)
                {
                    echo '<label for = "edad">Rango de edad</label><br>';
                    echo '<select name = "edad" id = "edad" size = "1">';
                    echo '<option value = "Menor de 18">Menor de 18</option>';
                    echo '<option value = "Entre 18 y 25">Entre 18 y 25</option>';
                    echo '<option value = "Entre 25 y 60">Entre 25 y 60</option>';
                    echo '<option value = "Mayor de 60">Mayor de 60</option>';
                    echo '</select>';
                }
                else
                {
                    $valor = empty($_POST['edad']) ? NULL : $_POST['edad'];
                    echo "<input type = 'hidden' name = 'edad' id = 'edad' value = '$valor'>";
                }
            ?>

            <?php
                if ($_POST["consulta"] == 6)
                {
                    echo '<label for = visita> &iquest;Has visitado el museo anteriormente&#63; </label><br>';
                    echo '<label for = "visita_si"> Sí </label>';
                    echo '<input type = "radio" id = "visita_si" name = "visita" value = "Si" checked>';
                    echo '<label for = "visita_no"> No </label>';
                    echo '<input type = "radio" id = "visita_no" name = "visita" value = "No"><br>';
                }
                else
                {
                    $valor = empty($_POST['visita']) ? NULL : $_POST['visita'];
                    echo "<input type = 'hidden' name = 'visita' id = 'visita' value = '$valor'>";
                }
            ?>            

            <?php
                if ($_POST["consulta"] == 7)
                {
                    echo '<label for = "comentarios">&iquest;Alg&uacute;n comentario?</label>';
                    echo '<textarea name = "comentarios" id = "comentarios" rows = "4" cols = "50"></textarea>';
                }
                else
                {
                    $valor = empty($_POST['comentarios']) ? NULL : $_POST['comentarios'];
                    echo "<input type = 'hidden' name = 'comentarios' id = 'comentarios' value = '$valor'>";
                }
            ?>

            <?php
                if ($_POST["consulta"] != 8) { echo "<input type = 'submit' onclick = 'empty_form()' name = 'envio' id = 'envio' value = 'Siguiente' />"; }
            ?>
        
            <?php
                if ($_POST["consulta"] == 8)
                {
                    require_once('config.php');

                    $usuario = $_POST['usuario_signup'];
                    $correo = $_POST['correo_signup'];
                    $telefono = $_POST['telefono_signup'];
                    $passwd = $_POST['passwd_signup'];
                    $edad = $_POST['edad'];
                    $visita = $_POST['visita'];
                    $comentario = empty($_POST['comentarios']) ? "Sin comentarios" : $_POST["comentarios"];
                    $tipo = "user";

                    $sql = "INSERT INTO usuarios (usuario, correo, telefono, passwd, visita, comentario, edad, tipo)
                    VALUES (:usuario, :correo, :telefono, :passwd, :visita, :comentario, :edad, :tipo)";

                    try
                    {
                        $parametros = $conexion_pdo->prepare($sql);
                        $parametros->bindParam(':usuario', $usuario);
                        $parametros->bindParam(':correo', $correo);
                        $parametros->bindParam(':telefono', $telefono);
                        $parametros->bindParam(':passwd', $passwd);
                        $parametros->bindParam(':visita', $visita);
                        $parametros->bindParam(':comentario', $comentario);
                        $parametros->bindParam(':edad', $edad);
                        $parametros->bindParam(':tipo', $tipo);
                        $parametros->execute();

                        echo "<h2> ¡Genial! Tu usuario ya está registrado. ¡Disfruta del arte! </h2>";
                        echo "La información introducida ha sido:\n";
                        echo "<ul>\n";
                        echo "<li><strong>Nombre de usuario</strong>: $usuario</li>\n";
                        echo "<li><strong>Correo electrónico</strong>: $correo</li>\n";
                        echo "<li><strong>Número de teléfono</strong>: $telefono</li>\n";
                        echo "<li><strong>Rango de edad</strong>: $edad</li>\n";
                        echo "<li><strong>Visitado anteriormente</strong>: $visita</li>\n";
                        echo "<li><strong>Comentarios</strong>: $comentario</li>\n";
                        echo "</ul>";
                    }
                    catch (PDOException $e) {
                        echo "Consulta fallida: " . $e->getMessage();
                    }
                }
            ?>
        </form>
        <footer id = "pie_de_pagina">
            <a href = "./php/contacto.php"> Contacto </a>
            <a href = "como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>