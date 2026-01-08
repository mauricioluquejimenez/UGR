import random
import re
import sys

argv = sys.argv

if (len(argv) < 3):
    print('El formato es generator.py nombre_fichero_salida n_palabras')
    sys.exit()

out_file = argv[1]
n_words = int(argv[2])

valid_words = list(filter(lambda w: re.match('^[a-z]+$', w), list(open('diccionario.txt'))))
words = random.sample(list(valid_words), min(len(valid_words), n_words))
with open(out_file, 'w') as dictionary:
    dictionary.write(''.join(words))
