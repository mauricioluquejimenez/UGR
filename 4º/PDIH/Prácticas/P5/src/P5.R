library(tuneR)
library(seewave)

# Establecer el path concreto en cada caso a la carpeta de trabajo.
setwd("/home/mauricio/Documentos/PDIH/P5/wav")

# 1. Leer dos ficheros de sonido (WAV o MP3) de unos pocos segundos de duración cada uno.
nombre <- readWave('nombre.wav')
apellido <- readWave('apellido.wav')

# 2. Dibujar la forma de onda de ambos sonidos.
oscillo(nombre)
oscillo(apellido)

# 3. Obtener la información de las cabeceras de ambos sonidos.
print(nombre)
print(apellido)

# 4. Unir ambos sonidos en uno nuevo.
mezcla <- pastew(apellido, nombre, output = "Wave")
print(mezcla)
writeWave(mezcla, file.path("nombre_completo.wav"))

# 5. Dibujar la forma de onda de la señal resultante.
oscillo(mezcla)
spectro(wave = mezcla)

# 6. Pasarle un filtro de frecuencia para eliminar las frecuencias entre 10000Hz y 20000Hz
fs <- mezcla@samp.rate
fs <- as.numeric(fs)

filtro <- fir(wave = mezcla, f = fs, from = 2000, to = 4000, bandpass = FALSE, output = "Wave")
print(filtro)
oscillo(filtro)
spectro(filtro)

# 7. Almacenar la señal obtenida como un fichero WAV denominado “mezcla.wav”.
writeWave(filtro, file.path("mezcla.wav"))

#8. Cargar un nuevo archivo de sonido, aplicarle eco y a continuación darle la vuelta al sonido. Almacenar la señal obtenida como un fichero WAV denominado “alreves.wav”.
eco <- readWave("nombre.wav")
eco <- echo(eco, f = 22050, amp=c(10000,5000,2500), delay=c(1,2,3), output = "Wave")
print(eco)
oscillo(eco)
writeWave(eco, file.path("eco.wav"))

alreves <- revw(eco, output = "Wave")
print(alreves)
oscillo(alreves)
writeWave(alreves, file.path("alreves.wav"))
