# 🧠 Geniusrise
# Copyright (C) 2023  geniusrise.ai
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from PIL import Image
import numpy as np
import sys


def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def rgb_to_565_big_endian(r, g, b):
    color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    # Swap bytes for big-endian
    color = ((color & 0xFF) << 8) | (color >> 8)
    return color


def convert_image_to_header(png_file, output_file):
    # Open the image using PIL
    img = Image.open(png_file).convert("RGB")
    # Convert image to NumPy array
    img_data = np.asarray(img)

    with open(output_file, "w") as f:
        f.write(f"const unsigned short image_{png_file.split('/')[-1].replace('.png', '')}[] PROGMEM=" + " {\n")

        # Process each pixel
        for row in img_data:
            for col in row:
                r, g, b = col
                c = rgb_to_565_big_endian(r, g, b)
                f.write(f"0x{c:04x}, ")
            f.write("\n")
        f.write("};\n")


if __name__ == "__main__":
    png_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_image_to_header(png_file, output_file)
