# Requisitos e instrucciones

## Requisitos
- Windows o Linux (Linux preferiblemente ya que con Windows el processo
de instal·lación de g++ via MinGW es más complicado). 

- Tanto compresscdi.cpp como uncompresscdi.cpp han sido escritos en
C++, por lo tanto se debe tener instalado el compilador g++. 

- Terminal que tenga la capacidad de ejecutar Makefiles (se ha incluido
un fichero Makefile con el objetivo de facilitar el proceso de compilación). 

## Instrucciones
#### Para comprimir:  
- Abrimos una consola (terminal de Windows o Linux) dentro del directorio donde
tenemos los ficheros de la entrega. 

- Ejecutamos el siguiente comando: <code>make all</code>.

- Ejecutamos el ejecutable <b>compresscdi</b> para comprimir el fichero original
passandole como primer argumento el fichero original (el que se desea comprimir),
y como segundo el nombre del fichero nuevo que queremos que tenga el fichero comprimido. 
Ejemplo: <code>./compresscdi ./pwned-passwords-sha1-ordered-by-count-v7_reducido.txt ./pwned-passwords-sha1-v7.cdi</code>

#### Para descomprimir

- Ejecutamos el ejecutable <b>uncompresscdi</b> para descomprimir el fichero
que se ha comprimido, passandole como primer argumento el fichero comprimido, 
y como segundo el nombre del fichero que queremos que tenga el fichero descomprimido. 
Éste fichero será igual que el fichero original. 
Ejemplo: <code>./uncompresscdi ./pwned-passwords-sha1-v7.cdi ./uncompressed.txt</code>

## Notas adicionales
- El ratio de compresion del programa, tal como nos indica el ejecutable <b>compresscdi</b> cuando
lo ejecutamos, es de: <b>2.20495</b>
- Para obtener un rendimiento máximo (comprimir y descomprimir con el menor tiempo posible), es
recomendable tener almenos 8GB de RAM. 
- <b>Runtime</b> (i7-10870h, 32GB DDR4, NVMe SSD): 
    - compresscdi: <b>(aprox) 2 segundos</b>
    - uncompresscdi: <b>(aprox) 5 segundos</b>
    