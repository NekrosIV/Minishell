#!/bin/bash

# Chemin vers votre Minishell
MINISHELL="./minishell"
echo -e "YO BG" | $MINISHELL > prompt.test 2> /dev/null
PROMPT=$(awk 'NR==1 {sub(/YO BG.*$/, ""); print}' prompt.test)

# Commandes pour retirer les couleurs et les espaces
REMOVE_COLORS="sed 's/\x1B\[[0-9;]*[JKmsu]//g'"
REMOVE_SPACES="sed 's/^[ \t]*//g'"

#Vérifier si 'valgrind' est passé en argument
if [ "$1" = "valgrind" ]; then
    USE_VALGRIND=1
    VALGRIND_COMMAND="valgrind --track-fds=yes --trace-children=yes --log-file=valgrind_output.test --suppressions=readline.supp "
else
    USE_VALGRIND=0
fi

# Fonction pour exécuter une commande dans votre Minishell et Bash, puis comparer les résultats
run_test() {
    command=$1
    echo -n "Test: $command ... "

    if [ $USE_VALGRIND -eq 1 ]; then
        # Exécuter la commande dans Minishell avec Valgrind et récupérer le statut
        echo -e "$command\necho \$?\nexit" | $VALGRIND_COMMAND $MINISHELL > minishell_full.test 2> minishell_err.test
        # Vérifier les résultats de Valgrind
        valgrind_check_fd=$(grep --text 'FILE DESCRIPTORS' valgrind_output.test | tail -n 1 | awk '{num6 = $6; gsub(/[^0-9]/, "", num6); if (($4 == "4" && num6 != "3") || ($4 - 1 != num6)) print "fd_leak";}')
        valgrind_check_leak=$(awk '/definitely lost:/ {if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline;}' valgrind_output.test)
    else
        # Exécuter la commande dans Minishell sans Valgrind
        echo -e "$command\necho \$?\nexit" | $MINISHELL > minishell_full.test 2> minishell_err.test
    fi
    minishell_status=$(grep -A 1 "echo \$?" minishell_full.test | tail -1 | grep -o '[0-9]*')


    # Exécuter la même commande dans Bash POSIX et récupérer le statut
    echo "$command" | bash --posix > bash_out.test 2> bash_err.test
    bash_status=$?

    # Filtrer les erreurs pour ne garder que la partie après le dernier ':'
    bash_error_filtered=$(cat bash_err.test | sed 's/^.*:\(.*:\)/\1/' | head -n1 | eval $REMOVE_SPACES)
    minishell_error_filtered=$(cat minishell_err.test | eval $REMOVE_COLORS | sed 's/^.*:\(.*:\)/\1/'| head -n1 | eval $REMOVE_SPACES)

    # Nettoyer la sortie pour la comparaison
    grep -v "$PROMPT" minishell_full.test | grep -v "$minishell_status" > minishell_out.test
    sed -i '$d' minishell_out.test

    # Comparer les sorties et les erreurs
    out_diff=$(diff --brief minishell_out.test bash_out.test)
    if [ "$bash_error_filtered" = "$minishell_error_filtered" ]; then
        err_diff="no_error"
    else
        err_diff="error"
    fi

    # Vérifier les résultats et imprimer le statut approprié
    if [ -z "$out_diff" ] && [ "$err_diff" = "no_error" ] && [ "$valgrind_check_fd" = "" ] && [  "$valgrind_check_leak" = "" ]&& [ $minishell_status -eq $bash_status ]; then
        echo "✅"
    else
        echo "❌"
        if [ ! -z "$out_diff" ]; then
            echo "Output differs! MINISHELL: $(cat minishell_out.test) BASH: $(cat bash_out.test)"
        fi
        if [ "$err_diff" = "error" ]; then
            echo "Error output differs (filtered)! MINISHELL: $minishell_error_filtered BASH: $bash_error_filtered"
        fi
        if [ ! -z "$valgrind_check_fd" ]; then
            echo "Valgrind detected issues: $valgrind_check_fd"
        fi
        if [ ! -z "$valgrind_check_leak" ]; then
            echo "Valgrind detected issues: leak"
        fi
        if [ $minishell_status -ne $bash_status ]; then
            echo "Exit status differs! Minishell: $minishell_status, Bash: $bash_status"
        fi
    fi
    echo "-----------------------"
}

# Tests
echo "Running tests..."
run_test "echo Hello, World!"
run_test "echo Hello, World! > testfile.test"
run_test "'ls' mmmmm"
run_test "commandthatdoesnotexist"

# Nettoyage
rm *.test