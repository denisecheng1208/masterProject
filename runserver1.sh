VBoxManage startvm $1 --type gui

sleep 30

ssh server1@192.168.56.103 "DISPLAY=:0 nohup fgfs --native-fdm=socket,out,60,$2,5004,udp --native-ctrls=socket,out,60,$2,5005,udp &"| exit
