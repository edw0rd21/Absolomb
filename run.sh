BUILD_DIR="build"
BUILD_TYPE="Release"
EXECUTABLE="Absolomb.exe"

cd "$BUILD_DIR/bin/$BUILD_TYPE"
echo "Running the executable..."
"./$EXECUTABLE"
cd ../../..
