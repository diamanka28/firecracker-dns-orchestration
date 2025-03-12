#!/bin/bash

# Compilation du fichier source en un executable
make -C src

# Montage du repertoire root
sudo mount setup/ubuntu-24.04.ext4 /mnt

# Copie de l'executable
sudo cp bin/HelloWorld /mnt/root/HelloWorld

# Demontage du repertoire root
sudo umount /mnt

# Supprime les fichiers objets et l'executable
make clean -C src
