erase tmp.lnk
command /c call ..\test\all echo file >> tmp.lnk
echo wlink form dos op dosseg d a op map name wdisasm lib clibc,mathc @tmp.lnk
wlink form dos op dosseg d a op map name wdisasm lib clibc,mathc @tmp.lnk
