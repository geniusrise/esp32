![logo_with_text](https://github.com/geniusrise/.github/assets/144122/2f8e51ee-0fcd-4f74-90fd-97301ef7943d)

```bash
for file in *.svg; do convert -background black -alpha off -resize 128x128! -define bmp:subtype=RGB565 -colors 256 "$file" "${file%.*}.bmp"; done
```
