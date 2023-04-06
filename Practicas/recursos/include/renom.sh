#!/bin/bash

shopt -s extglob

for nomhpp in *.hpp ; do
   nomh=${nomhpp/.hpp/.h}
   mv $nomhpp $nomh
done
