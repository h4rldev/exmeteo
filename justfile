default:
  just --list

@fix_perms:
  chmod +x scripts/build.sh
  chmod +x scripts/clear_vgcores.sh

@compile-debug name="exmeteo":
  scripts/build.sh -c {{name}} --debug

@compile name="exmeteo":
  scripts/build.sh -c {{name}}

@link name="exmeteo":
  scripts/build.sh -l {{name}}

@build name="exmeteo":
  just compile {{name}}
  just link {{name}}

@run args="" name="exmeteo":
  just compile {{name}}
  just link {{name}}
  ./bin/{{name}} {{args}}

@debug args="" name="exmeteo":
  just compile-debug {{name}}
  just link {{name}}
  valgrind ./bin/{{name}} {{args}}

@clear_cores args="":
  scripts/clear_vgcores.sh {{args}}

@install name="exmeteo":
  just build {{name}}
  sudo mv bin/{{name}} /usr/bin/
