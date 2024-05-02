/bin/echo ""$?""
/bin/echo $?"42"
/bin/echo ''$?''"42"
/bin/echo '"$USER"'
/bin/echo "'$USER'"
/bin/echo ""'$USER'""
/bin/echo '"'$USER'"'
/bin/echo "''$USER''"
/bin/echo "'"'$USER'"'"
/bin/echo '"'"$USER"'"'
/bin/echo $"HOME"$USER
/bin/echo $"HOM"E$USER
/bin/echo "exit_code ->$? user ->$USER home -> $HOME"
/bin/echo $"HOME"
/bin/echo $"42$"
/bin/echo "$ "
/bin/echo hi >outfile01 | /bin/echo bye
/bin/echo <123 <456 hi | /bin/echo 42
/bin/echo '$='
/bin/echo '$ '
/bin/echo "$?"
/bin/echo '$?'
/bin/echo "'$?'"
/bin/echo "cat lol.c | cat > lol.c"
/bin/echo "cat lol.c '|' cat > lol.c"
/bin/echo '$USER' "$USER" "text  ' text"
/bin/echo $USER =intergalaktikus miaf*szomez
/bin/echo -n"-n" bonjour
/bin/echo "'$USER'"
/bin/echo " '$USER' "
/bin/echo text"$USER"
/bin/echo text"'$USER'" ' $USER '
/bin/echo "text"   "$USER"    "$USER"
/bin/echo '              $USER          '
/bin/echo ''''''''''$USER''''''''''
/bin/echo """"""""$USER""""""""
/bin/echo '""""""""$USER""""""""'
/bin/echo ""'""""""$USER""""""'""
/bin/echo """"""""'$USER'""""""""
/bin/echo """""""'"$USER"'"""""""
/bin/echo """"""'""$USER""'""""""
/bin/echo ""'""'""""$USER""""'""'""
/bin/echo '""'""'""""$USER""""'""'""'
/bin/echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''
/bin/echo "text" "text$USER" ... "$USER"
/bin/echo
/bin/echo ''
/bin/echo ""
/bin/echo '' ""
/bin/echo /bin/echo
/bin/echo test1
/bin/echo 'test1'
/bin/echo "/bin/echo hallo"
/bin/echo /bin/echo"1"
/bin/echo "test1 test2"
/bin/echo "test1  test2" test3
/bin/echo " test1 test2 " 'test3 '
/bin/echo test1		test2
/bin/echo $USER$TESTNOTFOUND$HOME$
/bin/echo $USER$TESTNOTFOUND$HOME$WTF$PWD
/bin/echo -n test1
/bin/echo -n "test1 test2"
/bin/echo -n "test1 test2" test3
/bin/echo -n " test1 test2 " 'test3 '
/bin/echo -n test1		test2
/bin/echo test1 -n
/bin/echo "test1 -n"
/bin/echo -n -n test1
/bin/echo -n -n -n -n -n test1
/bin/echo "  -nn "
/bin/echo "-n test1 -n test2"
/bin/echo "test1 -n test2"
/bin/echo ~42
/bin/echo -n -n -nasd
/bin/echo -n -n -n-nnnnn
/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n-n
/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n feel my pain
/bin/echo -n -n -n-n
/bin/echo "'totally logical'"
/bin/echo 'totally logical'
/bin/echo ''totally logical''
/bin/echo ""'totally logical'""
grep hi <infile
grep hi "<infile" <         infile
grep hi <infile_big <infile
cat <"file name with spaces"
cat <infile_big infile
cat <"1""2""3""4""5"
cat <"infile"
echo <"infile_big" | cat <"infile"
echo <"infile_big" | cat "infile"
echo hi | cat <"infile"
echo hi | cat "infile"
cat <"infile" | echo hi
cat <"infile" | grep hello
cat <"infile_big" | echo hi
cat <missing
cat <missing | cat
cat <missing | echo oi
cat <missing | cat <"infile"
echo <123 <456 hi | echo 42
ls >outfile01
ls >         outfile01
ls >outfile01 >outfile02
ls >outfile01 >invalid_permission
chmod 000 invalid_permission
ls >"outfile with spaces"
ls >"outfile""1""2""3""4""5"
ls >"outfile01" >invalid_permission >"outfile02"
ls >invalid_permission >"outfile01" >invalid_permission
cat <"infile" >"outfile01"
echo hi >outfile01 | echo bye
echo hi >outfile01 >outfile02 | echo bye
echo hi >outfile01 >invalid_permission | echo bye
echo hi >invalid_permission | echo bye
echo hi >invalid_permission >outfile01 | echo bye
cat <"infile" >invalid_permission
cat >invalid_permission <"infile"
cat <missing >outfile01
cat >outfile01 <missing
cat <missing >invalid_permission
cat >invalid_permission <missing
cat >outfile01 <missing >invalid_permission
ls >>outfile01
ls >>      outfile01
ls >>outfile01 >outfile01
ls >outfile01 >>outfile01
ls >outfile01 >>outfile01 >outfile02
ls >>outfile01 >>outfile02
ls >>invalid_permission
ls >>invalid_permission >>outfile01
ls >>outfile01 >>invalid_permission
ls >outfile01 >>invalid_permission >>outfile02
ls <missing >>invalid_permission >>outfile02
ls >>invalid_permission >>outfile02 <missing
echo hi >>outfile01 | echo bye
echo hi >>outfile01 >>outfile02 | echo bye
echo hi >>invalid_permission | echo bye
echo hi >>invalid_permission >outfile01 | echo bye
cat <minishell.h>outfile
cat <minishell.h|ls
/bin/echo 42 > tmp_redir_out 42
cat > tmp_redir_out
/bin/echo "cat this text\\n" > tmp_redir_out
cat <tmp_redir_out
cat | cat | ls
cat < /dev/null
/bin/echo 42 > /dev/null
/bin/echo 42 >> /dev/null
/bin/echo 42 > /dev/null > /dev/null > /dev/null > /dev/null > tmp_redir_out
cat > tmp_redir_out
/bin/echo 42 > tmp_redir_out > tmp_redir_out1 > tmp_redir_out2 > tmp_redir_out3
/bin/echo 0
cat > tmp_redir_out
/bin/echo 1
cat > tmp_redir_out1
/bin/echo 2
cat > tmp_redir_out2
/bin/echo 3
cat > tmp_redir_out3
/bin/echo 42 > /dev/null > tmp_redir_out
/bin/echo 2 >> /dev/null >> tmp_redir_out
cat > tmp_redir_out
