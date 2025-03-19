#!/bin/bash

# Configurations
COUNTER_FILE="/tmp/firecracker_counter.txt"
SOCKET_BASE="/tmp/firecracker"

# Lire le dernier ID utilisé
if [ ! -f "$COUNTER_FILE" ]; then
    echo "Aucune VM à arrêter : compteur introuvable."
    exit 1
fi

VM_ID=$(cat "$COUNTER_FILE")
API_SOCKET="${SOCKET_BASE}_${VM_ID}.socket"

# Vérifier si le socket existe
if [ ! -e "$API_SOCKET" ]; then
    echo "Aucune VM active avec le socket : $API_SOCKET"
    exit 1
fi

echo "Tentative d'arrêt de la VM #$VM_ID via le socket : $API_SOCKET"

# Envoyer la commande InstanceStop à Firecracker
sudo curl -s -X PUT --unix-socket "/tmp/firecracker_<VM_ID>.socket" \
    -H "Accept: application/json" \
    -d '{
        "action_type": "InstanceStop"
    }' \
    "http://localhost/actions"

# Attendre un peu que la VM s'arrête
sleep 1

# Supprimer le socket
sudo rm -f "$API_SOCKET"
echo "Socket $API_SOCKET supprimé."

# Facultatif : supprimer l'interface réseau TAP si tu en as une spécifique
TAP_DEV="tap${VM_ID}"
sudo ip link del "$TAP_DEV" 2>/dev/null && echo "Interface $TAP_DEV supprimée."

# Mise à jour du compteur (revenir en arrière ou reset)
VM_ID=$((VM_ID - 1))
if [ $VM_ID -lt 0 ]; then
    VM_ID=0
fi
echo "$VM_ID" > "$COUNTER_FILE"

echo "VM #$((VM_ID + 1)) arrêtée et ressources nettoyées."
