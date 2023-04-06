## --------------------------------------------------------------------------------
## Prácticas de Informática Gráfica
##
## Archivo de configuración para compilar usando CMake en Linux o macOS
## última modificación: 11-nov de 2019
##      (se cambia versión 3.10 a 3.5.1, es la versión de las aulas, imagen ig620)
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (pracs_ig_exe)

## opciones para el compilador
set (opcs_warnings "-Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field -Wfatal-errors" )

## include de todo lo demás, en 'recursos/make' ...
include( ../../recursos/make/include-cmake-unix.txt)
