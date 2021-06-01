# Projet optimisation
 
Le rapport peut être trouvé dans le dossier [rendu](rendu/rendu.pdf)

## Exécuter le programme

Le programme compilé pour Windows est `Project_opti.exe`. Une fois à la racine du projet, il peut être lancé depuis une invite de commande Windows avec la commande `.\Project_opti.exe`.

Le programme peut prendre plusieurs arguments (dans n'importe quel ordre)

- `question <numéro>` (par défaut `question 1`)
  
  Défini le code à exécuter en fonction de la question.

- `dir <chemin>` (par défaut `dir resources/`)
  
  Défini le chemin d'accès du dossier contenant les jeux de données, utilisé par les questions dont le temps d'exécution est suffisamment court pour pouvoir s'exécuter sur tous les les jeux de données en un temps raisonnable.

- `file <chemin>` (par défaut `file resources/binpack1d_00.txt`)
  
  Défini le chemin d'accès du jeu de données à utiliser.

- `verbose`
  
  Si présent, le programme affichera le détail des solutions trouvées.

## Compiler le programme

Le plus simple pour compiler le projet sous Windows est d'installer MSYS2 :

- Installer [MSYS2](https://www.msys2.org/)
- Ouvrir la console de MSYS2
- Installer les outils de build gcc :
    ```bash
    pacman -S --needed base-devel mingw-w64-x86_64-toolchain
    ```
- Si `cmake` n'a pas été installé automatiquement, l'installer :
    ```bash
    pacman -S --needed cmake
    ```

Sous linux, vous aurez besoin du compilateur `g++` (de préférence une version supérieure à 10.3) et de `cmake`. Vous aurez également besoin d'installer le paquet `glpk` (pour avoir la commande `glpsol`) pour pouvoir exécuter la question 3 (résolution linéaire).

Dans les deux cas, la compilation se fera avec le script `build.sh` (il sera peut-être nécessaire de créer le dossier `build` avant de lancer le script).