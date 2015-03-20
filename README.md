# mkgit
Commande linux pour créer un répertoire git et l'initialiser avec un README.md


## Build
This project uses CMake for building.

```bash
cmake && make
```

If you haven't set default cmake behaviour, try

```bash 
cmake -G "Unix Makefiles"
```

## Usage
```bash
mkgit <directory> ["<Readme message>"]
```

The Readme message is optional. If a readme message is provided, a README.md file will be created.

Nothing is committed.