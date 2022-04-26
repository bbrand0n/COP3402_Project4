#!/bin/bash

make
compiled=$?
if [[ $compiled != 0 ]]; then
	echo "does not compile"
	exit 1
fi

echo "Compiles"

echo -n "Symbol Table Test : "
./a.out symboltable.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
	exit 1
else
	diff -w -B output.txt symboltableout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
		exit 1
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "If and While Test : "
./a.out ifwhile.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
	exit 1
else
	diff -w -B output.txt ifwhileout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
		exit 1
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Expression, Term, and Factor Test : "
./a.out eft.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
	exit 1
else
	diff -w -B output.txt eftout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
		exit 1
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 1 : "

./a.out error1.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error1out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 2 : "

./a.out error2.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error2out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 3 : "

./a.out error3.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error3out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 4 : "

./a.out error4.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error4out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 5 : "

./a.out error5.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error5out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 6a : "

./a.out error6a.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error6aout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 6b : "

./a.out error6b.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error6bout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 7a : "

./a.out error7a.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error7aout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 7b : "

./a.out error7b.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error7bout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 8 : "

./a.out error8.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error8out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 9 : "

./a.out error9.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error9out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 10 : "

./a.out error10.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error10out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 11a : "

./a.out error11a.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error11aout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 11b : "

./a.out error11b.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error11bout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 12 : "

./a.out error12.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error12out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 13 : "

./a.out error13.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error13out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 14 : "

./a.out error14.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error14out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 15 : "

./a.out error15.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error15out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 16 : "

./a.out error16.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error16out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 17 : "

./a.out error17.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error17out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 18 : "

./a.out error18.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error18out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 19 : "

./a.out error19.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error19out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 20 : "

./a.out error20.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt error20out.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi
