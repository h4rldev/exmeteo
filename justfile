lib := "./exmeteo/"
files := "{{lib}}" / "gui.c"

default:
  just --list

build name:
  gcc -O3 -o {{name}} main.c 
