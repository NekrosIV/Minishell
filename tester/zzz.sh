
export USER= 
env | grep USER 

export USER="@?/" 
env | grep USER 

export NEW="4 2" 
env | grep NEW 

export NEW="4=2" 
env | grep NEW 

export NEW=4=2 
env | grep 4=2 

export = 

export ''='' 

export ""="" 

export export 

export =============123 

export echo 

unset 

unset env 

unset PATH 
echo $PATH 
unset PATH 
echo $PATH 
unset HOME 
echo $HOME 

export NEW=42 
env | grep NEW 
unset NEW 
env | grep NEW 
unset NEW 
unset NEW 
unset NEW 
unset NEW 
unset NEW 
env | grep NEW 

 