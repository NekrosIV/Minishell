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
/bin/echo hi >./outfiles/outfile01 | /bin/echo bye
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
grep hi <./test_files/infile
grep hi "<infile" <         ./test_files/infile
grep hi <./test_files/infile_big <./test_files/infile
cat <"./test_files/file name with spaces"
cat <./test_files/infile_big ./test_files/infile
cat <"1""2""3""4""5"
cat <"./test_files/infile"
echo <"./test_files/infile_big" | cat <"./test_files/infile"
echo <"./test_files/infile_big" | cat "./test_files/infile"
echo hi | cat <"./test_files/infile"
echo hi | cat "./test_files/infile"
cat <"./test_files/infile" | echo hi
cat <"./test_files/infile" | grep hello
cat <"./test_files/infile_big" | echo hi
cat <missing
cat <missing | cat
cat <missing | echo oi
cat <missing | cat <"./test_files/infile"
echo <123 <456 hi | echo 42
ls >./outfiles/outfile01
ls >         ./outfiles/outfile01
ls >./outfiles/outfile01 >./outfiles/outfile02
ls >./outfiles/outfile01 >./test_files/invalid_permission
ls >"./outfiles/outfile with spaces"
ls >"./outfiles/outfile""1""2""3""4""5"
ls >"./outfiles/outfile01" >./test_files/invalid_permission >"./outfiles/outfile02"
ls >./test_files/invalid_permission >"./outfiles/outfile01" >./test_files/invalid_permission
cat <"./test_files/infile" >"./outfiles/outfile01"
echo hi >./outfiles/outfile01 | echo bye
echo hi >./outfiles/outfile01 >./outfiles/outfile02 | echo bye
echo hi >./outfiles/outfile01 >./test_files/invalid_permission | echo bye
echo hi >./test_files/invalid_permission | echo bye
echo hi >./test_files/invalid_permission >./outfiles/outfile01 | echo bye
cat <"./test_files/infile" >./test_files/invalid_permission
cat >./test_files/invalid_permission <"./test_files/infile"
cat <missing >./outfiles/outfile01
cat >./outfiles/outfile01 <missing
cat <missing >./test_files/invalid_permission
cat >./test_files/invalid_permission <missing
cat >./outfiles/outfile01 <missing >./test_files/invalid_permission
ls >>./outfiles/outfile01
ls >>      ./outfiles/outfile01
ls >>./outfiles/outfile01 >./outfiles/outfile01
ls >./outfiles/outfile01 >>./outfiles/outfile01
ls >./outfiles/outfile01 >>./outfiles/outfile01 >./outfiles/outfile02
ls >>./outfiles/outfile01 >>./outfiles/outfile02
ls >>./test_files/invalid_permission
ls >>./test_files/invalid_permission >>./outfiles/outfile01
ls >>./outfiles/outfile01 >>./test_files/invalid_permission
ls >./outfiles/outfile01 >>./test_files/invalid_permission >>./outfiles/outfile02
ls <missing >>./test_files/invalid_permission >>./outfiles/outfile02
ls >>./test_files/invalid_permission >>./outfiles/outfile02 <missing
echo hi >>./outfiles/outfile01 | echo bye
echo hi >>./outfiles/outfile01 >>./outfiles/outfile02 | echo bye
echo hi >>./test_files/invalid_permission | echo bye
echo hi >>./test_files/invalid_permission >./outfiles/outfile01 | echo bye
cat <minishell.h>./outfiles/outfile
cat <minishell.h|ls
/bin/echo 42 > tmp_redir_out 42
cat tmp_redir_out
rm tmp_redir_out
/bin/echo "cat this text\\n" > tmp_redir_out
cat <tmp_redir_out
rm tmp_redir_out
cat | cat | ls
cat < /dev/null
/bin/echo 42 > /dev/null
/bin/echo 42 >> /dev/null
/bin/echo 42 > /dev/null > /dev/null > /dev/null > /dev/null > tmp_redir_out
cat tmp_redir_out
rm tmp_redir_out
/bin/echo 42 > tmp_redir_out > tmp_redir_out1 > tmp_redir_out2 > tmp_redir_out3
/bin/echo 0
cat tmp_redir_out
/bin/echo 1
cat tmp_redir_out1
/bin/echo 2
cat tmp_redir_out2
/bin/echo 3
cat tmp_redir_out3
rm tmp_redir_out tmp_redir_out1 tmp_redir_out2 tmp_redir_out3
/bin/echo 42 > /dev/null > tmp_redir_out
/bin/echo 2 >> /dev/null >> tmp_redir_out
cat tmp_redir_out
rm tmp_redir_out