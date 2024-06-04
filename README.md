# exMeteo

![[LICENSE](https://github.com/h4rldev/exmeteo/blob/main/LICENSE)](https://img.shields.io/github/license/h4rldev/exmeteo?style=flat-square)
![[OpenSSF Scorecard](https://securityscorecards.dev/viewer/?uri=github.com/h4rldev/exmeteo)](https://api.securityscorecards.dev/projects/github.com/h4rldev/exmeteo/badge?style=flat-square)


> [!NOTE]
> This project is only a CLI by now, GUI will possibly never be implemented.

A weather & currency converter CLI written in C.

## How do I install?

I will possibly never build and upload a release binary for this, so you will need to build it yourself.

> Requirements

- libjansson
- libcurl
- gcc
- just (if you want to quickly compile the program without typing several commands)


### With just

```sh
just build {name, default being exmeteo}
```

or if you want it in /usr/bin

```sh
just install {name, default being exmeteo}
```

### Without just

```sh
chmod +x scripts/build.sh
scripts/build.sh -c {name, default being exmeteo}
scripts/build.sh -l {name, default being exmeteo}
```

now the script should be in projectroot/bin.
