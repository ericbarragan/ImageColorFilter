# Filtro de Color de Imagen


## Aportacion:

📌 **BMP** 

* Tiene un manejo básico de imagenes en formato BMP sin requerir de más librerias que las base en GCC.


📌  **Filtro fácil de configurar**

* Filtra rangos configurables de color en mapas RGB en base a la formula de la elipse por pares de color R con G, G con B y G con R, formando 3 filtros que se complementan.
  Por lo que se configura con los tres pares de cordenadas con sus respectivas distancias focales

  
<div align=”center”>![Ejemplo Imagen a Procesar](https://github.com/ericbarragan/ImageColorFilter/blob/main/Mesa5mini.bmp) ![Ejemplo Imagen a Procesar](https://github.com/ericbarragan/ImageColorFilter/blob/main/Mesa5_Fmini.bmp)</div>


📌  **Otras ventajas**

* Es practico en el preprocesamiento de imagenes sinembargo no es eficiente debido a las operaciones matemáticas que realiza.

* Se puede utilizar tanto en forma positiva como negativa, por lo que igualmente sirve para filtrar zonas y objetos que no son de interes, o bien para tomar las zonas y objetos de interes y eliminar el resto.

* La figura elíptica permite extender la zona filtrada haciéndola más angosta conforme se acerca a las zonas oscuras y claras donde tiene menos rango de variación el color, por lo que se pueden abarcar sombras y brillo sin invadir otros rangos de color.


        ![Ejemplo de los 3 planos de Representacion de Color Filtrados](https://github.com/ericbarragan/ImageColorFilter/blob/main/PaletaColores.bmp)


## Un poco de abstraccion sobre Colores

  Recordemos que si graficamos las combinaciones en RGB dando a cada color una dimensión, obtenemos un cubo.
  
  En este cubo tendremos en vertices opuestos el blanco y el negro, y en los puntos intermedios todas las combinaciones de color.
  
  Cualquier combinacion de colores tiene versiones mas claras y más oscuras, hasta llegar a ser blancas o negras. Estos colores en los objetos, además, cambian según los reflejos que reciben de los objetos a su alrededor, produciendo cambios en el color del objeto.
  
  Para poder filtrar las variaciones de color que percibe la camara, por eso no solo seleccionamos un color, más requerimos un rango de variaciones, que será más grande entre más variantes de color tengan los objetos, en combinación con sus distancias y ángulos de estos.

  Al acercarse al vertice blanco o al negro la cantidad de variaciones posibles se va reduciendo hasta hacerse cero. Así que la figura que se forma es algo así como un cono con punta al negro y otro cono con punta al blanco.
  
  Ahora que este par de figuras son tridimencionales y saber si la combinación de colores está dentro o fuera no es tan sencillo.
  
  Una opción es dividir esta tarea en 3 planos de combinación de color, y además, en lugar de tomar de referencia las lineas que se forman, usar una figura parecida y más fácil de calcular.
  
  Con lo que encontramos la Elipse, que con solo dos puntos de referencia y la distancia focal basta, por cada plano. Incluso con el adicional, de que no requerimos omitir la zona de las puntas, donde los colores ya no son distinguibles, ni en el blanco ni en el negro. Haciendolo adecuado.
  
  Ofreciendo el adicional, de que al calcularse independientemente y en forma complementaria, nos da un extra de control.
  
  
