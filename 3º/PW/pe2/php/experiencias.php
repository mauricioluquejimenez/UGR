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
        <title> MRG | Experiencias </title>
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

        <main>
            <section id = "comentarios">
                <h2 class = "titulo_reseñas"> Reseñas </h2>

                <fieldset class = "comentario">
                    <cite> Marco Márquez, 2 de junio de 2023 </cite>
                    <blockquote>
                            Visitar el Museo Real de Granada fue sin duda el punto culminante de mi viaje a España. La manera en que han integrado la arquitectura del palacio del siglo XVII con las exposiciones es simplemente sublime. Cada sala te transporta a una época distinta, permitiéndote apreciar la rica confluencia cultural de la región. No puedo recomendarlo suficientemente.
                    </blockquote>
                </fieldset>
                 

                <fieldset class = "comentario">
                    <cite> Fernando Fernández, 6 de marzo de 2021 </cite>
                    <blockquote>
                        Realmente impresionada con el Museo Real de Granada. La colección de arte morisco es espectacular, y la sección de arte contemporáneo proporciona un contraste fascinante. Además, el personal del museo fue increíblemente amable y conocedor. Definitivamente, una parada obligatoria para cualquier amante del arte que visite Granada.
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> Martín Martínez, 6 de agosto de 2022 </cite>
                    <blockquote>
                        El Museo Real de Granada excedió todas mis expectativas. La forma en que han conservado el edificio histórico mientras lo adaptan para exhibir obras de arte de manera tan efectiva es admirable. Las exposiciones temporales eran cautivadoras y ofrecían una perspectiva única sobre temas actuales. ¡Una experiencia educativa y enriquecedora!
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> Jimena Jiménez, 18 de septiembre de 2020 </cite>
                    <blockquote>
                        Increíble visita al Museo Real de Granada. Lo que más me gustó fue la variedad de la colección, que abarca desde artefactos antiguos hasta obras modernas. La sección dedicada a la influencia judía en Andalucía fue particularmente conmovedora. También, los jardines del museo son un lugar perfecto para reflexionar después de la visita.
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> John Johnson, 24 de noviembre de 2019 </cite>
                    <blockquote>
                        Como aficionado a la historia, encontré el Museo Real de Granada absolutamente fascinante. La integración de tecnologías interactivas para explicar el contexto histórico de las piezas expuestas hace que la visita sea accesible para todas las edades. Las audioguías son de gran ayuda y ofrecen insights que enriquecen la experiencia.
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> Luke Lookman, 20 de enero de 2024 </cite>
                    <blockquote>
                        Mi familia y yo quedamos maravillados con el Museo Real de Granada. Los talleres educativos para niños son una excelente manera de introducir a los pequeños al arte y la historia de una forma divertida y participativa. Las instalaciones son impresionantes y hay algo que captura el interés de todos, sin importar la edad.
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> Joan Jordi, 3 de junio de 2023 </cite>
                    <blockquote>
                        El Museo Real de Granada es un tesoro oculto. A pesar de no ser tan conocido como otros museos, ofrece una colección rica y variada que refleja la diversidad de Granada. Las vistas desde las terrazas del museo son simplemente espectaculares, brindando un marco incomparable para las obras de arte.
                    </blockquote>
                </fieldset> 

                <fieldset class = "comentario">
                    <cite> Trevor Trevorrow, 24 de septiembre de 2020 </cite>
                    <blockquote>
                        Sorprendente cómo el Museo Real de Granada logra equilibrar la solemnidad de su patrimonio con una atmósfera acogedora y accesible. La exposición sobre la convivencia de culturas en Andalucía es un testimonio poderoso de la historia de tolerancia y mezcla cultural de la región. La cafetería del museo también es excelente, con vistas que no te puedes perder.
                    </blockquote>
                </fieldset>

                <form id = "comentario_user">
                    <h2 class = "titulo_reseñas"> Añade tu propia reseña </h2>
                     
                    <fieldset>
                        <label style = "display: block;" for = "usuario_reseña"> Nombre de usuario: </label>
                        <input type = "text" id = "usuario_reseña" required>
                    </fieldset>

                    <fieldset>
                        <p> ¿Recomendarías el museo?</p>
                        <label style = "margin-left: 2.5%;" for = "visita_si"> Sí </label><input type = "radio" id = "visita_si" name = "visita" checked>
                        <label style = "margin-left: 2.5%;" for = "visita_no"> No </label><input type = "radio" id = "visita_no" name = "visita">
                    </fieldset> 
    
                    <fieldset> 
                        <label style = "display: block; margin-bottom: 2.5%;" for = "comentario"> Comentario: </label>
                        <textarea id = "comentario" rows = "5" cols = "50"> </textarea>
                        <p><button> Enviar </button></p>
                    </fieldset>                 
                </form>
            </section>
        </main>

        <footer id = "pie_de_página">
            <a href = "../php/contacto.php"> Contacto </a>
            <a href = "../como_se_hizo.pdf"> Cómo se hizo </a>
        </footer>
    </body>
</html>