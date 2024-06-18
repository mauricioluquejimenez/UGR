<!DOCTYPE html>
<html>
    <body>
        <?php

            $plato1 = "Pasta";
            $plato2 = "Tortilla";
            $plato3 = "Solomillo";
            
            $$plato1 = 12;
            $$plato2 = 15;
            $$plato3 = 18;

            echo "
            <table border = 1>
            <thead>
            	<tr>
                	<th> Platos </th>
                    <th> Precio </th>
                </tr>
            </thead>
            <tr><th> $plato1 </th><td> $Pasta € </td>
            <tr><th> $plato2 </th><td> $Tortilla € </td>
            <tr><th> $plato3 </th><td> $Solomillo € </td>";
        
            define("IVA", 1.21);

            $precio = $Pasta * IVA + $Tortilla * IVA + $Solomillo * IVA;
            
            echo "Precio final: $precio";
        ?>
    </body>
</html>