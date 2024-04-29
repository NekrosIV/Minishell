#!/bin/bash


# Chemin vers votre Minishell
MINISHELL="./minishell"
MINISHELL_DIR="minishell_output"
BASH_DIR="bash_output"
# Création des dossiers pour stocker les fichiers créés par Minishell et Bash
mkdir -p $MINISHELL_DIR
mkdir -p $BASH_DIR
mkdir -p tests
echo -e "YO BG" | $MINISHELL > ./tests/prompt.test 2> /dev/null
PROMPT=$(awk 'NR==1 {sub(/YO BG.*$/, ""); print}' ./tests/prompt.test)

# Commandes pour retirer les couleurs et les espaces
REMOVE_COLORS="sed 's/\x1B\[[0-9;]*[JKmsu]//g'"
REMOVE_SPACES="sed 's/^[ \t]*//g'"

#Vérifier si 'valgrind' est passé en argument
if [ "$1" = "valgrind" ]; then
    USE_VALGRIND=1
    VALGRIND_COMMAND="valgrind --track-fds=yes --trace-children=yes --log-file=./tests/valgrind_output.test --suppressions=readline.supp "
else
    USE_VALGRIND=0
fi


# Fonction pour ajuster les chemins des fichiers de redirection
adjust_redirection() {
    local command=$1
    local dir=$2
    local new_command=""
    local file=0

    # Lire chaque mot dans la commande
    for word in $command; do
        if [[ "$word" == ">" ]]; then
            new_command+="$word "
            file=1
        elif [[ "$word" == ">>" ]]; then
            new_command+="$word "
            file=1
        elif [[ $file -eq 1 ]]; then
            new_command+="$dir/$word "
            file=0
        else
            new_command+="$word "
        fi
    done
    echo "$new_command"
}



# Fonction pour exécuter une commande dans votre Minishell et Bash, puis comparer les résultats
run_test() {
    command=$1
    echo -n "Test: $command ... "
    local minishell_command=$(adjust_redirection "$command" "$MINISHELL_DIR")
    local bash_command=$(adjust_redirection "$command" "$BASH_DIR")
    if [ $USE_VALGRIND -eq 1 ]; then
        # Exécuter la commande dans Minishell avec Valgrind et récupérer le statut
        echo -e "$minishell_command" | $VALGRIND_COMMAND $MINISHELL > ./tests/minishell_full.test 2> ./tests/minishell_err.test
        minishell_status=$?
        # Vérifier les résultats de Valgrind
        valgrind_check_fd=$(grep --text 'FILE DESCRIPTORS' ./tests/valgrind_output.test | tail -n 1 | awk '{num6 = $6; gsub(/[^0-9]/, "", num6); if (($4 == "4" && num6 != "3") || ($4 - 1 != num6)) print "fd_leak";}')
        valgrind_check_leak=$(awk '/definitely lost:/ {if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline;}' ./tests/valgrind_output.test)
    else
        # Exécuter la commande dans Minishell sans Valgrind
        echo -e "$minishell_command" | $MINISHELL > ./tests/minishell_full.test 2> ./tests/minishell_err.test
        minishell_status=$?
    fi
    #minishell_status=$(grep -A 1 "echo \$?" ./tests/minishell_full.test | tail -1 | grep -o '[0-9]*')


    # Exécuter la même commande dans Bash POSIX et récupérer le statut
    echo "$bash_command" | bash --posix > ./tests/bash_out.test 2> ./tests/bash_err.test
    bash_status=$?

    # Filtrer les erreurs pour ne garder que la partie après le dernier ':'
    bash_error_filtered=$(cat ./tests/bash_err.test | grep -o '[^:]*$'  | head -n1 | eval $REMOVE_SPACES)
    minishell_error_filtered=$(cat ./tests/minishell_err.test | eval $REMOVE_COLORS | grep -o '[^:]*$' | head -n1 | eval $REMOVE_SPACES)

    # Nettoyer la sortie pour la comparaison
    tail -n +2  ./tests/minishell_full.test > ./tests/minishell_out.test

    # Comparer les sorties et les erreurs
    out_diff=$(diff ./tests/minishell_out.test ./tests/bash_out.test)
    if [ "$bash_error_filtered" = "$minishell_error_filtered" ]; then
        err_diff="no_error"
    else
        err_diff="error"
    fi

    # Comparer les listes de fichiers dans les dossiers Minishell et Bash
    local minishell_files=$(ls -1 $MINISHELL_DIR)
    local bash_files=$(ls -1 $BASH_DIR)

    # Vérifier les résultats et imprimer le statut approprié
    if [ -z "$out_diff" ] && [ "$err_diff" = "no_error" ] && [ "$valgrind_check_fd" = "" ] && [  "$valgrind_check_leak" = "" ]&& [ $minishell_status -eq $bash_status ] && [ "$minishell_files" == "$bash_files" ]; then
        echo "✅"
    else
        echo "❌"
        if [ ! -z "$out_diff" ]; then
            echo "Output differs!
            DIFF: $out_diff"
        fi
        if [ "$err_diff" = "error" ]; then
            echo "Error output differs (filtered)!
            YOU:  $minishell_error_filtered
            BASH: $bash_error_filtered"
        fi
        if [ $minishell_status -ne $bash_status ]; then
            echo "Exit status differs! 
            YOU:  $minishell_status 
            Bash: $bash_status"
        fi
        if [ "$minishell_files" != "$bash_files" ]; then
            echo "Files created differ!
            YOU:  $minishell_files
            BASH: $bash_files"
        fi
        if [ ! -z "$valgrind_check_fd" ]; then
            echo "Valgrind detected issues: $valgrind_check_fd"
        fi
        if [ ! -z "$valgrind_check_leak" ]; then
            echo "Valgrind detected issues: leak"
        fi
    fi
    echo "-----------------------"
    rm -rf $MINISHELL_DIR/* $BASH_DIR/*
}

# Tests
echo "Running tests..."
# run_test "echo Hello, World!"
# run_test "echo Hello, World! > /testfile.test"
# run_test "'ls' mmmmm"
# run_test "commandthatdoesnotexist"
# run_test ">>>>>>>>>>"
# run_test "'\$USER'"
# run_test ""\$USER""
# run_test "echo '\$USER'"

IFS=""

while read -r LINE
do
    run_test $LINE
done < zzz
# cmd=""''echo hola""'''' que""'' tal""''
# run_test 'echo hola"""""""""""" $USER'
# run_test ' :'
# run_test ' !'
# run_test ' >'
# run_test ' <'
# run_test ' >>'
# run_test ' <<'
# run_test ' <>'
# run_test ' >>>>>'
# run_test ' >>>>>>>>>>>>>>>'
# run_test ' <<<<<'
# run_test ' <<<<<<<<<<<<<<<<'
# run_test ' > > > >'
# run_test ' >> >> >> >>'
# run_test ' >>>> >> >> >>'
# run_test ' hola que tal'
# run_test ' Makefile'
# run_test ' echo'
# run_test ' echo -n'
# run_test ' echo Hola'
# run_test ' echoHola'
# run_test ' echo-nHola'
# run_test ' echo -n Hola'
# run_test ' echo "-n" Hola'
# run_test ' echo -nHola'
# run_test ' echo Hola -n'
# run_test ' echo Hola Que Tal'
# run_test ' echo         Hola'
# run_test ' echo    Hola     Que    Tal'
# run_test ' echo      \n hola'
# run_test ' echo "         " | cat -e'
# run_test ' echo           | cat -e'
# run_test ' ""\'\'echo hola""\'\'\'\' que""\'\' tal""\'\''
# run_test $cmd
# run_test ' echo $'
# run_test ' echo $?'
# run_test ' echo $?$'
# run_test ' echo $? | echo $? | echo $?'
# run_test ' echo $:$= | cat -e'
# run_test ' echo " $ " | cat -e'
# run_test ' echo \' $ \' | cat -e'
# run_test ' echo $HOME'
# run_test ' echo \$HOME'
# run_test ' echo my shit terminal is [$TERM]'
# run_test ' echo my shit terminal is [$TERM4'
# run_test ' echo my shit terminal is [$TERM4]'
# run_test ' echo $UID'
# run_test ' echo $HOME9'
# run_test ' echo $9HOME'
# run_test ' echo $HOME%'
# run_test ' echo $UID$HOME'
# run_test ' echo Le path de mon HOME est $HOME'
# run_test ' echo $USER$var\$USER$USER\$USERtest$USER'
# run_test ' echo $hola*'
# run_test ' echo -nnnn $hola'
# run_test ' echo > <'
# run_test ' echo | |'
# run_test ' EechoE'
# run_test ' .echo.'
# run_test ' >echo>'
# run_test ' <echo<'
# run_test ' >>echo>>'
# run_test ' |echo|'
# run_test ' |echo -n hola'
# run_test ' echo *'
# run_test ' echo \'*\''
# run_test ' echo D*'
# run_test ' echo *Z'
# run_test ' echo *t hola'
# run_test ' echo *t'
# run_test ' echo $*'
# run_test ' echo hola*hola *'
# run_test ' echo $hola*'
# run_test ' echo $HOME*'
# run_test ' echo $""'
# run_test ' echo "$"""'
# run_test ' echo \'$\'\'\''
# run_test ' echo $"HOME"'
# run_test ' echo $\'\'HOME'
# run_test ' echo $""HOME'
# run_test ' echo "$HO"ME'
# run_test ' echo \'$HO\'ME'
# run_test ' echo "$HO""ME"'
# run_test ' echo \'$HO\'\'ME\''
# run_test ' echo "\'$HO\'\'ME\'"'
# run_test ' echo ""$HOME'
# run_test ' echo "" $HOME'
# run_test ' echo \'\'$HOME'
# run_test ' echo \'\' $HOME'
# run_test ' echo $"HO""ME"'
# run_test ' echo $\'HO\'\'ME\''
# run_test ' echo $\'HOME\''
# run_test ' echo "$"HOME'
# run_test ' echo $=HOME'
# run_test ' echo $"HOLA"'
# run_test ' echo $\'HOLA\''
# run_test ' echo $DONTEXIST Hola'
# run_test ' echo "hola"'
# run_test ' echo \'hola\''
# run_test ' echo \'\'hola\'\''
# run_test ' echo \'\'h\'o\'la\'\''
# run_test ' echo "\'\'h\'o\'la\'\'"'
# run_test ' echo "\'"h\'o\'la"\'"'
# run_test ' echo"\'hola\'"'
# run_test ' echo "\'hola\'"'
# run_test ' echo \'"hola"\''
# run_test ' echo \'\'\'ho"\'\'\'\'l"a\'\'\''
# run_test ' echo hola""""""""""""'
# run_test ' echo hola"\'\'\'\'\'\'\'\'\'\'"'
# run_test ' echo hola\'\'\'\'\'\'\'\'\'\'\'\''
# run_test ' echo hola\'""""""""""\''
# run_test ' e"cho hola"'
# run_test ' e\'cho hola\''
# run_test ' echo "hola     " | cat -e'
# run_test ' echo ""hola'
# run_test ' echo "" hola'
# run_test ' echo ""             hola'
# run_test ' echo ""hola'
# run_test ' echo "" hola'
# run_test ' echo hola""bonjour'
# run_test ' "e"\'c\'ho \'b\'"o"nj"o"\'u\'r'
# run_test ' ""e"\'c\'ho \'b\'"o"nj"o"\'u\'r"'
# run_test ' echo "$DONTEXIST"Makefile'
# run_test ' echo "$DONTEXIST""Makefile"'
# run_test ' echo "$DONTEXIST" "Makefile"'
# run_test ' $?'
# run_test ' $?$?'
# run_test ' ?$HOME'
# run_test ' $'
# run_test ' $HOME'
# run_test ' $HOMEdskjhfkdshfsd'
# run_test ' "$HOMEdskjhfkdshfsd"'
# run_test ' \'$HOMEdskjhfkdshfsd\''
# run_test ' $DONTEXIST'
# run_test ' $LESS$VAR'



# Nettoyage
rm -rf tests $MINISHELL_DIR $BASH_DIR