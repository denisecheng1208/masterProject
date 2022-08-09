package application;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ResourceBundle;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;

public class Controller implements Initializable{
	
    private FGFSConnection fgfs;
    private String host;
    private int telnetPort;
    private double throttle;
    private double rudder;
	
	@FXML
	private Label statusLabel;
	
	@FXML
	private Slider myThrottle;
	
	@FXML
	private Slider myRudder;

	public void open(ActionEvent e) {
		System.out.println("open");
	
		String line = null;
        try {
            Process process = Runtime.getRuntime().exec("./src/script/client.o 172.25.76.117 5001 open");
        	BufferedReader br = new BufferedReader(new InputStreamReader(process.getInputStream())); 
          
            while ((line = br.readLine()) != null) {
                System.out.println(line);
		if (line.substring(0, 6).equals("buffer"))
			telnetPort = Integer.parseInt(line.substring(8));
            }
            br.close();
            
            Runtime.getRuntime().exec("./src/script/runclient.sh");
        } catch (Exception ep) {
            ep.printStackTrace();
        }
        
        System.out.println("Telnet port:" + telnetPort);
		statusLabel.setText("Status: connected!");
		host = "172.25.76.117";
		
		try {
			fgfs = new FGFSConnection(host, telnetPort);
			System.out.println("Telnet connect successfully!");
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			System.out.println("Telnet connection try failed!");
			e1.printStackTrace();
		}
		
	}
	
	public void close(ActionEvent e) {
		System.out.println("close");
        try {
            Runtime.getRuntime().exec("./src/script/client.o 172.25.76.117 5001 close"); 
        } catch (Exception ep) {
            ep.printStackTrace();
        }
		statusLabel.setText("Status: unconnected!");
	}
	
	public void autostart(ActionEvent e) {
		try {
			fgfs.nasal("c172p.autostart();");
			//System.out.println("set throttle 0.5");
			//fgfs.setDouble("/controls/engines/current-engine/throttle", 0.5);
		} catch (IOException e1) {
			System.out.println("autostart fail");
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		/*
		throttle = myThrottle.getValue();
		rudder = myRudder.getValue();
		System.out.println("set throttle "+throttle);
		System.out.println("set rudder "+rudder);
		try {
			fgfs.setDouble("/controls/engines/current-engine/throttle", throttle);
			fgfs.setDouble("/controls/flight/rudder", rudder);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/
	}

	@Override
	public void initialize(URL arg0, ResourceBundle arg1) {		
		
		myThrottle.valueProperty().addListener(new ChangeListener<Number>(){

			@Override
			public void changed(ObservableValue<? extends Number> arg0, Number arg1, Number arg2) {
				throttle = myThrottle.getValue();
				System.out.println("set throttle "+throttle);
				try {
					fgfs.setDouble("/controls/engines/current-engine/throttle", throttle);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}	
			}
		
		});
		
		myRudder.valueProperty().addListener(new ChangeListener<Number>(){

			@Override
			public void changed(ObservableValue<? extends Number> arg0, Number arg1, Number arg2) {
				rudder = myRudder.getValue();
				System.out.println("set rudder "+rudder);
				try {
					fgfs.setDouble("/controls/flight/rudder", rudder);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}	
			}
		
		});
		
	}

	
}
