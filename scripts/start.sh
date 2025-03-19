#!/bin/bash

# ----- CONFIG -----
COUNTER_FILE="/tmp/firecracker_counter.txt"
SOCKET_BASE="/tmp/firecracker"
PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &>/dev/null && pwd)"
FIRECRACKER_DIR="$PROJECT_DIR/setup/firecracker"

# ----- INITIALISER LE COMPTEUR -----
if [ ! -f "$COUNTER_FILE" ]; then
    echo "0" > "$COUNTER_FILE"
fi

# ----- INCRÉMENTER ET LIRE LE COMPTEUR -----
VM_ID=$(($(cat "$COUNTER_FILE") + 1))
echo "$VM_ID" > "$COUNTER_FILE"

# ----- CRÉER LE NOM DU SOCKET -----
API_SOCKET="${SOCKET_BASE}_${VM_ID}.socket"

echo "Lancement de Firecracker instance $VM_ID avec socket : $API_SOCKET"

# ----- SUPPRIMER SOCKET EXISTANT -----
sudo rm -f "$API_SOCKET"

# ----- LANCER FIRECRACKER -----
sudo "$FIRECRACKER_DIR/firecracker" --api-sock "$API_SOCKET"
