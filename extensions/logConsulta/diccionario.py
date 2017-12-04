import time

def imprimir_diccionario(diccionario):
	for palabras_dic, valor in diccionario.items():
		print palabras_dic, ":", valor

def buscar_en_diccionario(diccionario, palabra_leida, seq, interest):


	if diccionario.has_key(palabra_leida): #Si existe la palabra
		print palabra_leida, ":", diccionario[palabra_leida]   # Imprime el valor
		valor = int(diccionario[palabra_leida])
		interest.write("%d\n" % valor)
		#interest.close()
		return seq
	else: #Si no esta la palabra en diccionario
		diccionario [palabra_leida] = seq
		print "PALABRA NUEVA AGREGADA AL DICCIONARIO"
		interest.write("%d\n" % seq)
		#interest.close()
		seq = seq + 1
		return seq
		
# Leer archivo .dat (log de consulta)

#diccionario = {'daphne earle hall': '0'}
diccionario = {}
palabras_log = []
archivo = open("log.dat", "r")
interest = open("LogInterest.dat","w")
seq_dic = 0
n_consulta = 1

for linea in archivo.readlines(): #Leo la linea completa
	 linea = linea[:-1] #Elimina el salto de linea \n
	 palabras_log = linea.split("\t") #Corto la linea segun tabulador y la guardo en un array
	 print ("La palabra leida en el log es:", palabras_log [1])
	 seq_dic = buscar_en_diccionario(diccionario, palabras_log [1], seq_dic, interest)
	 n_consulta = n_consulta + 1

	 #if (n_consulta==100000):
	 #	break


imprimir_diccionario(diccionario)