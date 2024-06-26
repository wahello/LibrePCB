#!/usr/bin/env bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -euv -o pipefail

# Bash on Windows calls the Windows provided "find" tool instead of the one
# from MSYS, which is bullshit since it isn't compatible. As a workaround,
# we adjust PATH.
export PATH="/usr/bin:$PATH"

TARGET="$OS-$ARCH"

if [ "$ARCH" = "x86_64" ]
then
  cp -r ./dist/innosetup/* ./build/dist/innosetup/
  cp -r ./build/install/opt/. ./build/dist/innosetup/files
  iscc ./build/dist/innosetup/installer.iss \
    //O".\\artifacts\\nightly_builds" \
    //F"librepcb-installer-nightly-$TARGET"
else
  # Note: Omit the release candidate number (e.g. "rc1") as it is not allowed!
  ./dist/installer/update_metadata.sh "$TARGET" "1.1.1-1"
  PACKAGES_DIR="./artifacts/installer_packages/$TARGET"
  mkdir -p $PACKAGES_DIR/librepcb.stable.app/data/stable
  cp -r ./dist/installer/output/packages/. $PACKAGES_DIR/
  cp -r ./build/install/opt/. $PACKAGES_DIR/librepcb.stable.app/data/stable/
  binarycreator --online-only -c ./dist/installer/output/config/config.xml -p $PACKAGES_DIR \
                ./artifacts/nightly_builds/librepcb-installer-nightly-$TARGET.exe
fi
