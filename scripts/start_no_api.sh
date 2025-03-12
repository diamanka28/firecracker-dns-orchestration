#!/bin/bash

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &>/dev/null && pwd)"
SETUP_DIR=$PROJECT_DIR/setup
BIN_DIR=$PROJECT_DIR/bin
ROOTFS_DIR=$SETUP_DIR/rootfs
FIRECRACKER_DIR=$SETUP_DIR/firecracker
cd $PROJECT_DIR

# Compilation du fichier source en un executable
make -C $PROJECT_DIR/src

# Montage du repertoire root
sudo mount $ROOTFS_DIR/ubuntu-24.04.ext4 /mnt

# Copie de l'executable
sudo cp $BIN_DIR/HelloWorld /mnt/root/HelloWorld

# Demontage du repertoire root
sudo umount /mnt

# Lancement de la microVM
$FIRECRACKER_DIR/firecracker --no-api --config-file $SETUP_DIR/vm-config.json

# Supprime les fichiers objets et l'executable
make clean -C $PROJECT_DIR/src
