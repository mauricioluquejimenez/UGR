ddocument.addEventListener('DOMContentLoaded', function() {
    document.getElementById('formulario').addEventListener('submit', function(event) {
        let usuario = document.getElementById('usuario_signup').value;
        let correo = document.getElementById('correo_signup').value;
        let errorMsg = '';

        // Validar que los campos no estén vacíos
        if (usuario.trim() === '') {
            errorMsg += 'El campo "Usuario" no puede estar vacío.\n';
        }
        if (correo.trim() === '') {
            errorMsg += 'El campo "Correo" no puede estar vacío.\n';
        } else {
            // Validar el formato del correo
            let correoValido = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}$/;
            if (!correoValido.test(correo)) {
                errorMsg += 'El formato del correo no es válido.\n';
            }
        }

        // Si hay errores, prevenir el envío del formulario y mostrar los errores
        if (errorMsg !== '') {
            event.preventDefault();
            alert(errorMsg);
        }
    });
});
