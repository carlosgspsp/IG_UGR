## ----------------------------------------------------------------------------
## Prácticas de Informática Gráfica
##
## Archivo de configuración para compilar usando Make en Linux o macOS
## última modificación: 20 Julio de 2019
## ----------------------------------------------------------------------------

## ---------------------------------------------------------------------
## aspectos configurables

# seleccionar depuración (-g) u optimización (-O3)
opt_dbg_flag := -g

# sirve para abortar después de primer error
exit_first := -Wfatal-errors

# sirve para ver todos los warnings al compilar,
# excepto el warning producido por no usar un parámetro en una función o no usar un elemento privado en algún método
warn_all := -Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field

## ---------------------------------------------------------------------
## invocar las definiciones y targets en el archivo 'include.make'

include ../../recursos/make/include-make-unix.make
