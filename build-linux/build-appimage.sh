#!/bin/bash

# Remove old appdir
rm -rf AppDir

mkdir -p AppDir
cd AppDir

# make directories
mkdir -p opt/principia/
mkdir -p usr/lib/

# Copy over meta
cp ../principia.desktop principia.desktop
cp ../principia.png principia.png
ln -s principia.png .DirIcon

cat > AppRun <<\EOF
#!/bin/sh
PATH="$(dirname "$(readlink -f "${0}")")"
export LD_LIBRARY_PATH="${HERE}"/usr/lib/:"${HERE}"/lib/:"${LD_LIBRARY_PATH}"
EXEC="${PATH}/opt/principia/principia"
exec "${EXEC}"
EOF
chmod +x AppRun

# Copy over game and data
cp ../apparatus2 opt/principia/principia
cp -r ../../data-pc opt/principia
cp -r ../../data-shared opt/principia

# Copy over libraries
ldd ../apparatus2 | awk 'NF == 4 { system("cp " $3 " usr/lib/") }'



# D-D-D-D-D-D-D-Do the fucken thing yeah
cd ..
ARCH=x86_64 ./appimagetool-x86_64.AppImage AppDir/
