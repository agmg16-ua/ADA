# Historial de versiones
# 2.0 (17/03/2020): corregido un error que hacía que la práctica no se recompilara para algunos ficheros de prueba
#

#Si tus ficheros de prueba no comienzan por "tad", cambia "tad*.cpp" por el prefijo de tus ficheros, por ejemplo "prueba*.cpp":
for FILE in TESTS_UNITARIOS/*.maze ; do 
	echo "Evaluando $FILE"
	
	cp $FILE maze.cc 
        rm -f ./tad
	make
	./maze > $FILE.out
	diff -b -B -i $FILE.out $FILE.sol
	if [ "$?" == "0" ]; then
		echo "OK"
	else
		echo "ERROR"
	fi
	
done

