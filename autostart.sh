VBoxManage startvm server1 --type gui

sleep 30

ssh server1@192.168.56.103 "DISPLAY=:0 nohup fgfs --native-fdm=socket,out,60,192.168.56.1,5004,udp --native-ctrls=socket,out,60,192.168.56.1,5005,udp"| kill -SIGINT


fgfs --native-fdm=socket,in,60,192.168.56.1,5004,udp --native-ctrls=socket,in,60,192.168.56.1,5005,udp --fdm=external

