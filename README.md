# Editeur de lien
### Notre programme éditionneur de lien est un simulation du commande native de linux "ld" , ce programme est commposé de 2 phases :
### - la première est l'étape de la lecture de l'entête d'un fichier ELF 32bit et puis l'affichage de ces informations.
### - la deuxième partie consiste à fusionner les fichiers .o en un un seule fichier ( qui sera executable en fin du projet).
***Remarque : Malheursement , due à des problèmes d'implémentations et des difficultées rencontrées , on n'a pas pu y arriver au bout.***
## Fichiers du programmes :
**Le programme est composé principalement de deux répertoires *phase1* et *phase2* :**
### phase 1 :
- **E1_elf_header.c** : affichage de l'entête du fichiers elf étape 1.
- **E2_table_section.c** : affichage du table des sections étape 2.
- **E3_content_of_section.c** : affichage du contenu des sections étape 3.
- **E4_symbole_table.c** : affichage du table des symboles en étape 4.
- **E5_table_of_reimplantation.c** : fichier de l'étape 5 , affichage du table de reimplémentation.
- **Makefile** : Makefile pour la compilation du fichier.
- **main.c** : programme principale.
- **elf_reader.c** : contient read_section et read_elf_header.
- **tests.sh** : un fichier bash contenant des test pour des programes, Exemple :
```bash
./tests <fichier_elf_pour_test>
```
### phase 2 : 

## Un simple Mode d’emploi :
### Phase 1 :
- Etape 1 : se placer dans le dossier phase1 :
```bash
cd phase1
```
- Etape 2 : compiler le code avec via la commande :
```bash
make
```
- Etape 3 : exécuter le code 
```bash
./main <nom fichier> <option>
```
**Les options permettent d’afficher les différentes parties du fichier ELF**
- Liste des options :  
-h : permet d’afficher le header du fichier ELF.  
-S : permet d’afficher la table des sections  
-x : permet d’afficher le contenu d’une section  
*Pour l’exécution de cette étape nous avons besoin d’un argument de plus qui représente le nom 
     de la section.*  
-s : permet d’afficher la table des symboles  
-r : permet d’afficher des tables de réimplantations  
- Exemple d’exécution du programme pour les étapes **1**, **2**, **4** et **5** :  
```bash
./main file1.o -h (affiche l’entête du fichier ELF file1)
./main file1.o -S (affiche la table des sections du fichier file1.o)
```
- Exemple d’exécution du programme pour la partie 3 :
```bash
./main file1.o -x .text (affiche le contenue de la section .text du fichier file1)
```
***N.B : le programme peut prendre plusieurs options à la fois***  
- Exemple : 
```bash
./main file1.o -hSr affiche l’entête, la table des sections ainsi que les tables de réimplantation du fichier file1.o
```
