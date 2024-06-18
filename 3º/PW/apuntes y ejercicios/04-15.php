<!DOCTYPE html>
<html>
<body>

<?php
	class Formulario
    {
    	private $campos[];
        
    	public function  formulario($id, $name, $label)
        {
        	$this -> campos[$id] = [$name, $label];
        }
        
        public function print()
        {
        	$out = "<form method = get>";
            
            foreach($campos as $id => $name, $label)
            	$out = $out."<label for = $id $name and </label>"."<input type = text id = $id";
                $out = $out
            }
        }
    }
    
    $form = new Formulario;
?> 

</body>
</html>