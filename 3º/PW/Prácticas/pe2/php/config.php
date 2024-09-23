<?php
$server_pdo = "mysql:host=localhost;dbname=dbmaulujim_pw2324";
$user = "pwmaulujim";
$password = "23maulujim24";
$database = "dbmaulujim_pw2324";

try
{
    $conexion_pdo = new PDO( $server_pdo, $user, $password );
    $conexion_pdo -> setAttribute( PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION );
} catch ( PDOException $e ) { echo "Conexión fallida: " . $e -> getMessage(); }
?>