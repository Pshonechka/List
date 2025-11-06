# ./solver.exe ^
for file in ./*.dot; do
    filename=$(basename "$file" .dot)
    dot "$file" -T svg -o ${filename}.svg
done
