#!/bin/bash

# Directory to clean
PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &>/dev/null && pwd)"
SETUP_DIR=$PROJECT_DIR/setup
cd $SETUP_DIR || { echo "Directory not found!"; exit 1; }

# Enable extended globbing
shopt -s extglob

# Files to keep (space-separated list)
EXCEPTIONS=("vm-config.json")
# Convert array to pattern (e.g., "important.txt|keep_this.docx")
EXCEPTION_PATTERN="!(${EXCEPTIONS[@]// /|})"

# Remove all files except for the exceptions
sudo rm -rf $EXCEPTION_PATTERN

echo "Cleanup completed successfully!"
