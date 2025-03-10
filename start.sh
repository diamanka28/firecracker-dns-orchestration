#!/bin/bash

# Compilation du fichier source en un executable
make -C src

# Montage du repertoire root
sudo mount setup/ubuntu-24.04.ext4 /mnt

# Copie de l'executable
sudo cp bin/HelloWorld /mnt/root/HelloWorld

# Demontage du repertoire root
sudo umount /mnt

# Lancement de la microVM
./firecracker --no-api --config-file setup/vm-config.json

# This is a comment
#echo "Hello, World!"

# Print the current date
#echo "Today's date is: $(date)"

# List files in the current directory
#echo "Files in this directory:"
#ls

# Create a new directory
#mkdir -p my_new_directory

# Print a message if the directory was created
#if [ -d "my_new_directory" ]; then
#    echo "Directory 'my_new_directory' was created successfully."
#else
#    echo "Failed to create directory."
#fi

# Supprime les fichiers objets et l'executable
make clean -C src
