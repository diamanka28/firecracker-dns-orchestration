#!/bin/bash

# Directory containing files necessary to launch Firecracker
PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &>/dev/null && pwd)"
SETUP_DIR=$PROJECT_DIR/setup
cd $SETUP_DIR

# Architecture of the machine
ARCH="$(uname -m)"

	# Download a Firecracker binary
if [ ! -d "firecracker" ]; then {
	echo -e "\n\t-----\tDownloading a Firecracker binary\t-----\n\n"
	
	release_url="https://github.com/firecracker-microvm/firecracker/releases"
	latest=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} ${release_url}/latest))
	curl -L ${release_url}/download/${latest}/firecracker-${latest}-${ARCH}.tgz | tar -xz
	mv release-${latest}-$(uname -m) firecracker
	mv firecracker/firecracker-${latest}-${ARCH} firecracker/firecracker
	
} else
	echo -e "\n\t-----\tFirecracker binary is already installed\t-----"
fi

	# Download a Linux kernel binary
KERNEL_DIR=kernel
if [ ! -d "$KERNEL_DIR" ]; then {
	mkdir $KERNEL_DIR
	echo -e "\n\n\t-----\tDownloading a Linux kernel binary\t-----\n\n"
	
	latest=$(wget "http://spec.ccfc.min.s3.amazonaws.com/?prefix=firecracker-ci/v1.11/$ARCH/vmlinux-5.10&list-type=2" -O - 2>/dev/null | grep -oP "(?<=<Key>)(firecracker-ci/v1.11/$ARCH/vmlinux-5\.10\.[0-9]{1,3})(?=</Key>)")
	wget -P $KERNEL_DIR "https://s3.amazonaws.com/spec.ccfc.min/${latest}"
	
} else
	echo -e "\n\n\t-----\tLinux kernel binary is already installed\t-----\n"
fi

	# Download a rootfs
ROOTFS_DIR=rootfs
if [ ! -d "$ROOTFS_DIR" ]; then {
	mkdir $ROOTFS_DIR; cd $ROOTFS_DIR
	echo -e "\n\t-----\tDownloading a rootfs\t-----\n\n"
	
		# Download a squashfs
	echo -e "-----\t1. Downloading a squashfs\t-----\n"
	
	wget "https://s3.amazonaws.com/spec.ccfc.min/firecracker-ci/v1.11/${ARCH}/ubuntu-24.04.squashfs"
	
		# Unsquashfs starting
	echo -e "-----\t2. Unsquashfs starting\t-----\n"
	unsquashfs ubuntu-24.04.squashfs
	rm ubuntu-24.04.squashfs
	
		# Create an ssh key for the rootfs
	echo -e "\n-----\t3. Creating an ssh key for the rootfs\t-----\n"
	ssh-keygen -f id_rsa -N ""
	cp -v id_rsa.pub squashfs-root/root/.ssh/authorized_keys
	
		# Create an ext4 filesystem image
	echo -e "\n-----\t4. Creating an ext4 filesystem image\t-----\n"
	sudo chown -R root:root squashfs-root
	truncate -s 400M ubuntu-24.04.ext4
	sudo mkfs.ext4 -d squashfs-root -F ubuntu-24.04.ext4
} else
	echo -e "\n\t-----\trootfs is already installed\t-----\n"
fi
