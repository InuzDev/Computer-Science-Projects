mkdir -p Output
for f in null leak data AfterFree InvalidFree; do
   gcc -g -o "$f" "$f.c" 2> "Output/${f}_compilacion.txt"
   ./"$f" > "Output/${f}_directo.txt" 2>&1
   echo "Salida: $?" >> "Output/${f}_directo.txt"
   gdb -batch -ex run ./"$f" > "Output/${f}_gdb.txt" 2>&1
   valgrind --leak-check=yes ./"$f" > "Output/{$f}_valgrind.txt" 2>&1
done
echo "End - Check folder"
