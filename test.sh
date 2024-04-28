#!/bin/bash

# Chemin vers votre Minishell
MINISHELL="./minishell"
echo -e "YO BG" | $MINISHELL > prompt.txt 2> /dev/null
PROMPT=$(awk 'NR==1 {sub(/YO BG.*$/, ""); print}' prompt.txt)

# Commandes pour retirer les couleurs et les espaces
REMOVE_COLORS="sed 's/\x1B\[[0-9;]*[JKmsu]//g'"
REMOVE_SPACES="sed 's/^[ \t]*//g'"

# Fonction pour exécuter une commande dans votre Minishell et Bash, puis comparer les résultats
run_test() {
    command=$1
    echo -n "Test: $command ... "

    # Exécuter la commande dans Minishell et récupérer le statut
    echo -e "$command\necho \$?\nexit" | $MINISHELL > minishell_full.txt 2> minishell_err.txt
    minishell_status=$(grep -A 1 "echo \$?" minishell_full.txt | tail -1 | grep -o '[0-9]*')

    # Exécuter la même commande dans Bash POSIX et récupérer le statut
    echo "$command" | bash --posix > bash_out.txt 2> bash_err.txt
    bash_status=$?

    # Filtrer les erreurs pour ne garder que la partie après le dernier ':'
    bash_error_filtered=$(cat bash_err.txt | sed 's/^.*:\(.*:\)/\1/' | head -n1 | eval $REMOVE_SPACES)
    minishell_error_filtered=$(cat minishell_err.txt | eval $REMOVE_COLORS | sed 's/^.*:\(.*:\)/\1/'| head -n1 | eval $REMOVE_SPACES)

    # Nettoyer la sortie pour la comparaison
    grep -v "$PROMPT" minishell_full.txt | grep -v "$minishell_status" > minishell_out.txt
    sed -i '$d' minishell_out.txt

    # Comparer les sorties et les erreurs
    out_diff=$(diff --brief minishell_out.txt bash_out.txt)
    if [ "$bash_error_filtered" = "$minishell_error_filtered" ]; then
        err_diff="no_error"
    else
        err_diff="error"
    fi

    # Vérifier les résultats et imprimer le statut approprié
    if [ -z "$out_diff" ] && [ "$err_diff" = "no_error" ] && [ $minishell_status -eq $bash_status ]; then
        echo "✅"
    else
        echo "❌"
        if [ ! -z "$out_diff" ]; then
            echo "Output differs! MINISHELL: $(cat minishell_out.txt) BASH: $(cat bash_out.txt)"
        fi
        if [ "$err_diff" = "error" ]; then
            echo "Error output differs (filtered)! MINISHELL: $minishell_error_filtered BASH: $bash_error_filtered"
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
run_test "echo Hello, World! > testfile.txt"
run_test "commandthatdoesnotexist"
run_test "'ls' mmmmm"

# Nettoyage
rm minishell_full.txt minishell_out.txt minishell_err.txt bash_out.txt bash_err.txt prompt.txt
