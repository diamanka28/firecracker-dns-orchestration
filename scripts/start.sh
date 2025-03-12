API_SOCKET="/tmp/firecracker.socket"
PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &>/dev/null && pwd)"
FIRECRACKER_DIR=$PROJECT_DIR/setup/firecracker

# Remove API unix socket
sudo rm -f $API_SOCKET

# Run firecracker
sudo $FIRECRACKER_DIR/firecracker --api-sock "${API_SOCKET}"
