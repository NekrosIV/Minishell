#!/bin/bash


# Chemin vers votre Minishell
MINISHELL="../minishell"
MINISHELL_DIR="minishell_output"
BASH_DIR="bash_output"
# Création des dossiers pour stocker les fichiers créés par Minishell et Bash
mkdir -p $MINISHELL_DIR
mkdir -p $BASH_DIR
mkdir -p tests
echo -e "YO BG" | $MINISHELL > ./tests/prompt.test 2> /dev/null
PROMPT=$(awk 'NR==1 {sub(/YO BG.*$/, ""); print}' ./tests/prompt.test)
NB_SUCESSES=0
TOTALE=0

# Commandes pour retirer les couleurs et les espaces
REMOVE_COLORS="sed 's/\x1B\[[0-9;]*[JKmsu]//g'"
REMOVE_SPACES="sed 's/^[ \t]*//g'"

RESET="\033[0m"
BOLD="\033[1m"
ULINE="\033[4m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
BOLD='\033[1m' 

#Vérifier si 'valgrind' est passé en argument
if [ "$1" = "valgrind" ]; then
    USE_VALGRIND=1
    VALGRIND_COMMAND="valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all --log-file=./tests/valgrind_output.test --suppressions=readline.supp "
else
    USE_VALGRIND=0
fi


# Fonction pour ajuster les chemins des fichiers de redirection
adjust_redirection() {
    local command=$1
    local dir=$2
    local new_command=""
    local file=0
    local temp=$IFS
    local i=1;
    local total=$(echo "$command" | wc -l)

    IFS="   "
for word in $command; do
    if [[ "$word" =~ ^([<>]{1,2})(.*)$ ]]; then
        # Extraire les redirections et les noms de fichiers accolés
        redirect="${BASH_REMATCH[1]}"  # Capture la redirection (>, >>, <)
        filename="${BASH_REMATCH[2]}"  # Capture le nom de fichier suivant directement
        if [[ -n "$filename" ]]; then  # Si un nom de fichier est attaché à la redirection
            if [[ "$filename" == /* || "$redirect" == "<<" ]]; then
                new_command+="$redirect$filename "
            elif [[ "$filename" == \"* ]]; then
                # Si le nom de fichier commence par un guillemet
                new_command+="${redirect}\"$dir/${filename:1} "
            else
                new_command+="$redirect$dir/$filename "
            fi
        else
            new_command+="$redirect "  # Ajoute juste la redirection, attend le prochain mot pour le fichier
            if [[ "$redirect" != "<<" ]]; then
                file=1
            fi
        fi
    elif [[ $file -eq 1 ]]; then
        # Vérifier si le chemin est déjà absolu ou commence par un guillemet
        if [[ "$word" == /* ]]; then
            new_command+="$word "  # Ajouter le chemin absolu sans modification
        elif [[ "$word" == \"* ]]; then
            new_command+="\"$dir/${word:1} "  # Ajouter le dir après le guillemet ouvrant
        else
            new_command+="$dir/$word "  # Ajouter le chemin préfixé par $dir
        fi
        file=0  # Désactiver le flag après avoir ajouté le fichier
    else
        new_command+="$word "  # Ajouter le mot normal à la commande
    fi
    ((i++))
done

new_command="${new_command% }"  # Enlever l'espace final s'il y en a un
IFS=$temp

if [[ "$dir" == "$MINISHELL_DIR" ]]; then
    echo "$new_command" >> vm.txt
fi
echo "$new_command"

}


print_result() {

    local len=$((${#command} + 9))  # 9 caractères pour "TEST : "
    local total_len=75
    local padding=$(($total_len - $len))

    # Positionner le curseur correctement pour l'affichage du résultat
    if [ -z "$out_diff" ] && [ "$err_diff" = "no_error" ] &&
       [ -z "$valgrind_check_fd" ] && [ -z "$valgrind_check_leak" ] &&
       [ "$minishell_status" -eq "$bash_status" ] &&
       [ "$minishell_files" = "$bash_files" ]; then
        printf "\r${BOLD}${GREEN}TEST[%d] : %s %-${padding}s[OK]${RESET}\n" "$TOTALE" "$command"
        NB_SUCESSES=$(($NB_SUCESSES + 1))
    else
        printf "\r${BOLD}${RED}TEST[%d] : %s %-${padding}s[KO]${RESET}\n"  "$TOTALE"  "$command"
        print_details
    fi
    
    echo "-----------------------"
}

# Function to print details about the failures
print_details() {
    [ -n "$out_diff" ] && printf "Output differs!    \nDIFF: %s\n" "$out_diff"
    [ "$err_diff" = "error" ] && printf "Error output differs (filtered)!\nYOU:  %s\nBASH: %s\n" "$minishell_error_filtered" "$bash_error_filtered"
    [ "$minishell_status" -ne "$bash_status" ] && printf "Exit status differs!\nYOU:  %s\nBASH: %s\n" "$minishell_status" "$bash_status"
    [ "$minishell_files" != "$bash_files" ] && printf "Files created differ!\nYOU:  %s\nBASH: %s\n" "$minishell_files" "$bash_files"
    [ -n "$valgrind_check_fd" ] && printf "Valgrind detected issues: %s\n" "$valgrind_check_fd"
    [ -n "$valgrind_check_leak" ] && printf "Valgrind detected issues: leak\n"
}


# Fonction pour exécuter une commande dans votre Minishell et Bash, puis comparer les résultats
run_test() {
    command=$1
    TOTALE=$(($TOTALE + 1))
    local minishell_command=$command
    local bash_command=$(printf "%s" "$command" | sed "s|minishell_output/|$BASH_DIR/|g")

    printf "Test[%d] : %s ... " "$TOTALE" "$command"
    if [ $USE_VALGRIND -eq 1 ]; then
        # Exécuter la commande dans Minishell avec Valgrind et récupérer le statut
        echo -e "$minishell_command" | $VALGRIND_COMMAND $MINISHELL > ./tests/minishell_full.test 2> ./tests/minishell_err.test
        minishell_status=$?
        # Vérifier les résultats de Valgrind
        valgrind_check_fd=$(grep --text 'FILE DESCRIPTORS' ./tests/valgrind_output.test | tail -n 1 | awk '{num6 = $6; gsub(/[^0-9]/, "", num6); if (($4 == "4" && num6 != "3") || ($4 - 1 != num6)) print "fd_leak";}')
        valgrind_check_leak=$(awk '/definitely lost:/ {if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline; if ($4 != "0") print "leak"; getline;}' ./tests/valgrind_output.test)
    else
        # Exécuter la commande dans Minishell sans Valgrind
        echo  "$minishell_command" | $MINISHELL > ./tests/minishell_full.test 2> ./tests/minishell_err.test
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
    # tail -n +2  ./tests/minishell_full.test > ./tests/minishell_out.test
    cp ./tests/minishell_full.test ./tests/minishell_out.test
    # Comparer les sorties et les erreurs
    out_diff=$(diff <(sed 's|minishell_output/||g' ./tests/minishell_out.test) <(sed 's|bash_output/||g' ./tests/bash_out.test))
    if [ "$bash_error_filtered" = "$minishell_error_filtered" ]; then
        err_diff="no_error"
    else
        err_diff="error"
    fi

    # Comparer les listes de fichiers dans les dossiers Minishell et Bash
    local minishell_files=$(ls -1 $MINISHELL_DIR)
    local bash_files=$(ls -1 $BASH_DIR)

    # Vérifier les résultats et imprimer le statut approprié
    print_result
}

# Tests
echo "Running tests..."

# while IFS= read -r LINE
# do
#     run_test "$LINE"
# done < vm.txt

LINE=$(< eof.sh)
run_test "$LINE"


printf "%d/%d\n" $NB_SUCESSES $TOTALE



# Nettoyage
# rm -rf tests $MINISHELL_DIR $BASH_DIR