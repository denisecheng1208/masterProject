VBoxManage startvm $1 --type gui

sleep 30

ssh -t -t server1@$2 << remotessh
DISPLAY=:0 nohup fgfs --native-fdm=socket,out,60,$3,5004,udp --native-ctrls=socket,out,60,$3,5005,udp&

exit
remotessh
