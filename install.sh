#!/bin/bash
set -e

REPO="sapirrior/cplus"
VERSION="v0.0.1"

OS_NAME="$(uname -s | tr '[:upper:]' '[:lower:]')"
ARCH_NAME="$(uname -m)"

case "$OS_NAME" in
  linux)
    if [ "$ARCH_NAME" = "aarch64" ] || [ "$ARCH_NAME" = "arm64" ]; then
      PLATFORM="linux-arm64"
    else
      PLATFORM="linux-x64"
    fi
    ;;
  darwin)
    PLATFORM="macos-universal"
    ;;
  *)
    echo "Unsupported operating system: $OS_NAME"
    exit 1
    ;;
esac

BINARY_NAME="${VERSION}-cplus-${PLATFORM}"
DOWNLOAD_URL="https://github.com/${REPO}/releases/download/${VERSION}/${BINARY_NAME}"

echo "Detected Platform: $OS_NAME ($ARCH_NAME)"
echo "Downloading C+ $VERSION ($PLATFORM)..."

if ! curl -L --fail -o cplus "$DOWNLOAD_URL"; then
  echo "Error: Failed to download binary from $DOWNLOAD_URL"
  exit 1
fi

chmod +x cplus

echo "Installing to /usr/local/bin/cplus (requires sudo)..."
if sudo mv cplus /usr/local/bin/cplus; then
  echo "Successfully installed C+!"
  echo "You can now run it using: cplus"
else
  echo "Installation failed. Try moving the 'cplus' file to your PATH manually."
  exit 1
fi
