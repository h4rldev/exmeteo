default:
  just --list

@compile name="exmeteo":
  scripts/build.sh -c {{name}}

@link name="exmeteo":
  scripts/build.sh -l {{name}}

@build name="exmeteo":
  just compile {{name}}
  just link {{name}}

@run name="exmeteo" args="":
  just compile {{name}}
  just link {{name}}
  ./bin/{{name}} {{args}}
