default:
  just --list

compile name="exmeteo":
  scripts/build.sh -c {{name}}

link name="exmeteo":
  scripts/build.sh -l {{name}}

build name="exmeteo":
  just compile {{name}}
  just link {{name}}
  mv out/{{name}} .

run name="exmeteo":
  just compile {{name}}
  just link {{name}}
  mv out/{{name}} .
  ./{{name}}
