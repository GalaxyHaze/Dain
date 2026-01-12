#!/bin/bash

# 1. Determine Latest Version Tag
REPO="GalaxyHaze/Nova" # CHANGE THIS
API_URL="https://api.github.com/repos/$REPO/releases/latest"

# Get tag name from JSON response
VERSION=$(curl -s $API_URL | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')

echo "Installing Nova version: $VERSION"

# 2. Detect OS and Architecture
OS="$(uname -s)"
ARCH="$(uname -m)"

FILE_NAME=""

case "$OS" in
  Linux*)  FILE_NAME="nova-linux-amd64" ;;
  Darwin*) FILE_NAME="nova-macos-amd64" ;;
  CYGWIN*|MINGW*|MSYS*) FILE_NAME="nova-windows-amd64.exe" ;;
  *)      echo "OS not supported"; exit 1 ;;
esac

DOWNLOAD_URL="https://github.com/$REPO/releases/download/$VERSION/$FILE_NAME"

# 3. Download the binary
echo "Downloading from $DOWNLOAD_URL..."
curl -L -o Nova $DOWNLOAD_URL

# 4. Install to /usr/local/bin (requires sudo)
if [ "$OS" = "Darwin" ] || [ "$OS" = "Linux" ]; then
    chmod +x Nova
    sudo mv Nova /usr/local/bin/Nova
    echo "Installation complete! Run 'Nova --help' to get started."
else
    echo "For Windows, please move the downloaded 'mylang.exe' to a folder in your PATH."
fi
