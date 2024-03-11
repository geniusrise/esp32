#!/bin/bash

# Directory containing your SVG files
SVG_DIR="./assets/svgs"
# Output directory for the C header files
OUTPUT_DIR="./src/svgs"
# Name of the master header file that includes all others
MASTER_HEADER="${OUTPUT_DIR}/svgs.h"

# Create output directories if they don't exist
mkdir -p "${OUTPUT_DIR}"
mkdir -p "${OUTPUT_DIR}"

# Convert all SVG files to BMP format with a black background
for svg_file in "${SVG_DIR}"/*.svg; do
    echo "Converting ${svg_file}..."
    base_name=$(basename "${svg_file}" .svg)
    convert "${svg_file}" \
        -distort SRT '1.2 0' \
        -resize 128x128 \
        -unsharp 2x5.0+1.7+0 \
        -colors 256 \
        -flatten \
        "${OUTPUT_DIR}/${base_name}.png"

done

# Convert BMP files to C header files
for png_file in "${OUTPUT_DIR}"/*.png; do
    echo "Processing ${png_file}..."
    base_name=$(basename "${png_file}" .png)

    echo "$png_file"

    # Run png2hex.py to generate the C header file
    python "$PWD"/scripts/bmp2hex.py "${png_file}" "${OUTPUT_DIR}/${base_name}.h"

    # Append an include directive to the master header file
    # echo "#include \"${base_name}.h\"" >> "${MASTER_HEADER}"
done

echo "Conversion complete. Master header created at ${MASTER_HEADER}."
