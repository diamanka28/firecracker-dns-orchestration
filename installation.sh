#!/bin/bash

# --- Récupération du rootfs et du kernel ---
cd setup

# Récupérer l'architecture du système
ARCH="$(uname -m)"

# Trouver la dernière version du noyau Linux disponible
latest=$(wget "http://spec.ccfc.min.s3.amazonaws.com/?prefix=firecracker-ci/v1.11/$ARCH/vmlinux-5.10&list-type=2" -O - 2>/dev/null | grep -oP "(?<=<Key>)(firecracker-ci/v1.11/$ARCH/vmlinux-5\.10\.[0-9]{1,3})(?=</Key>)")

# Téléchargement du kernel
wget "https://s3.amazonaws.com/spec.ccfc.min/${latest}"

# Téléchargement du kernel
wget -O ubuntu-24.04.squashfs.upstream "https://s3.amazonaws.com/spec.ccfc.min/firecracker-ci/v1.11/${ARCH}/ubuntu-24.04.squashfs"

# Création d'une clé ssh pour le rootfs
unsquashfs ubuntu-24.04.squashfs.upstream
ssh-keygen -f id_rsa -N ""
cp -v id_rsa.pub squashfs-root/root/.ssh/authorized_keys
mv -v id_rsa ./ubuntu-24.04.id_rsa

# Création du système de fichier ext4
sudo chown -R root:root squashfs-root
truncate -s 400M ubuntu-24.04.ext4
sudo mkfs.ext4 -d squashfs-root -F ubuntu-24.04.ext4

# --- Récupérer les binaires de Firecracker ---
release_url="https://github.com/firecracker-microvm/firecracker/releases"
latest=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} ${release_url}/latest))
curl -L ${release_url}/download/${latest}/firecracker-${latest}-${ARCH}.tgz | tar -xz

# Renommage du binaire en "firecracker"
mv release-${latest}-$(uname -m)/firecracker-${latest}-${ARCH} firecracker
