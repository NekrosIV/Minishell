grep hi ">infile" >infile
echo hi >"outfile with spaces"
echo hi >"outfile""1""2""3""4""5"
echo hi >"outfile01" <invalid_permission 
echo hi >invalid_permission < outfile01
