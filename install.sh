#!/bin/bash
set -e

REPO_URL="https://github.com/sapirrior/cplus.git"
REPO="sapirrior/cplus"
VERSION="v0.0.1"

OS_NAME="$(uname -s | tr '[:upper:]' '[:lower:]')"
ARCH_NAME="$(uname -m)"
IS_TERMUX=false

if [ -n "$PREFIX" ] && [ -d "$PREFIX/bin" ]; then
  IS_TERMUX=true
fi

# Function to compile from source (highly recommended for Termux/ARM)
install_from_source() {
  echo "Termux detected. Compiling natively for maximum compatibility..."
  
  if ! command -v clang++ &> /dev/null; then
    echo "Error: 'clang' (clang++) is not installed."
    echo "Please run: pkg install clang git -y"
    exit 1
  fi

  TMP_DIR=$(mktemp -d)
  echo "Cloning repository to $TMP_DIR..."
  git clone --depth 1 "$REPO_URL" "$TMP_DIR"
  
  cd "$TMP_DIR"
  echo "Building C+..."
  clang++ -std=c++20 src/main.cpp src/environment/*.cpp src/lexer/*.cpp src/parser/*.cpp src/ast/*.cpp -I src/ -o cplus
  
  echo "Installing to $PREFIX/bin/cplus..."
  mv cplus "$PREFIX/bin/cplus"
  chmod +x "$PREFIX/bin/cplus"
  
  cd - > /dev/null
  rm -rf "$TMP_DIR"
  
  echo "Successfully installed C+ natively!"
  echo "You can now run it using: cplus"
}

# Standard binary installation
install_binary() {
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

  INSTALL_DIR="/usr/local/bin"
  echo "Installing to $INSTALL_DIR/cplus (requires sudo)..."

  if sudo mv cplus "$INSTALL_DIR/cplus"; then
    echo "Successfully installed C+!"
    echo "You can now run it using: cplus"
  else
    echo "Installation failed. Try moving the 'cplus' file to your PATH manually."
    exit 1
  fi
}

if [ "$IS_TERMUX" = true ]; then
  install_from_source
else
  install_binary
fi
