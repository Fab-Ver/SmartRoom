import 'dart:convert';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:lite_rolling_switch/lite_rolling_switch.dart';
import 'package:sleek_circular_slider/sleek_circular_slider.dart';

import 'DiscoverDevicePage.dart';

class HomePage extends StatefulWidget {
  	const HomePage({super.key});

	@override
	State<HomePage> createState() => _HomePage();
}

class _HomePage extends State<HomePage> {
	BluetoothState _bluetoothState = BluetoothState.UNKNOWN;
	BluetoothDevice? _selectedDevice;
	BluetoothConnection? _connection;
	bool isConnecting = false;
	bool get isConnected => (_connection?.isConnected ?? false);
	bool isDisconnecting = false;

	String _messageBuffer = '';
	bool? currentLightState;
	int? currentRollerBlindState;

	@override
	void initState() {
		super.initState();
		/**Get the current bluetooth state */
		FlutterBluetoothSerial.instance.state.then((state) {
      		setState(() {
        		_bluetoothState = state;
     		 });
    	});
		/**Listen for bluetooth state change. */
		FlutterBluetoothSerial.instance.onStateChanged().listen((BluetoothState state) {
			setState(() {
				_bluetoothState = state;
			});		
		});
	}

	@override
	void dispose(){
		FlutterBluetoothSerial.instance.setPairingRequestHandler(null);
		if(isConnected) {
			isDisconnecting = true;
			_connection?.dispose();
			_connection = null;
		}
		super.dispose();
	}

  	@override
  	Widget build(BuildContext context) {
		return Scaffold(
      		appBar: AppBar(
        		title: const Text('Smart Room'),
				actions: <Widget>[
					IconButton(
						onPressed: _bluetoothState == BluetoothState.STATE_ON
							? () async {
								final selectedDevice = await Navigator.of(context).push(MaterialPageRoute(
									builder : (context) {
										return const DiscoverDevicePage();
									}
								));
								if (selectedDevice != null) {
									_selectedDevice = selectedDevice;
                      				print('Discovery -> selected ${_selectedDevice?.address}');
									_connectToDevice();
                    			} else {
                      				print('Discovery -> no device selected');
                    			}
							}
							: () => showDialog(
								context: context, 
								builder: (BuildContext context) => AlertDialog(
									title: const Text('Bluetooth State'),
									content: const Text('Turn on Bluetooth to continue'),
									actions: <Widget>[
										TextButton(
              								onPressed: () => Navigator.pop(context, 'Cancel'),
              								child: const Text('Cancel'),
           								),
            							TextButton(
              								onPressed: () async {
												final navigator = Navigator.of(context);
												await FlutterBluetoothSerial.instance.requestEnable();
												navigator.pop();
											},
              								child: const Text('OK'),
            							),
									],
								)
							),
						icon: _bluetoothState == BluetoothState.STATE_OFF ?  const Icon(Icons.bluetooth_disabled) : const Icon(Icons.bluetooth),
						tooltip: 'Discover devices',),
					],
      		),
      		body: isConnecting 
				? SizedBox(
              		width: MediaQuery.of(context).size.width,
              		height: MediaQuery.of(context).size.height,
              		child: const Center(child: CircularProgressIndicator())
				)
				: !isConnected 
					? SizedBox(
              			width: MediaQuery.of(context).size.width,
              			height: MediaQuery.of(context).size.height,
              			child: const Center(child: Text('Connect to the Smart Room Device to use the app...'))
					)
					: Center(
        				child: Column(
          					mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          					children: <Widget>[
								Row(
									mainAxisAlignment: MainAxisAlignment.spaceEvenly,
									children: <Widget>[
										Chip(
											label: currentLightState == null ? const Text('UNKNOWN',style: TextStyle(color: Colors.white),) : currentLightState == true ? const Text('ON',style: TextStyle(color: Colors.white),) : const Text('OFF',style: TextStyle(color: Colors.white),),
											avatar: CircleAvatar(
												backgroundColor: Colors.white,
												child: Icon(Icons.light_mode,color: currentLightState == null ? Colors.yellow : currentLightState == true ? Colors.green : Colors.red,),
											),
											backgroundColor: currentLightState == null ? Colors.yellow : currentLightState == true ? Colors.green : Colors.red,
											elevation: 3.0,
											padding: const EdgeInsets.all(3.0),
										),
										Chip(
											label: currentRollerBlindState == null ? const Text('UNKNOWN',style: TextStyle(color: Colors.white),) : Text(currentRollerBlindState.toString(),style: const TextStyle(color: Colors.white),),
											avatar: CircleAvatar(
												backgroundColor: Colors.white,
												child: Icon(Icons.roller_shades,color: currentRollerBlindState == null ? Colors.yellow : Colors.lightBlue),
											),
											backgroundColor: currentRollerBlindState == null ? Colors.yellow : Colors.lightBlue,
											elevation: 3.0,
											padding: const EdgeInsets.all(3.0),
										),
									],
								),
            					LiteRollingSwitch(
              						value: false,
              						textOn: 'ON',
              						textOff: 'OFF',
              						colorOn: Colors.green,
              						colorOff: Colors.red,
              						iconOn: Icons.light_mode,
              						iconOff: Icons.light_mode_outlined,
              						animationDuration: const Duration(milliseconds: 100),
              						onChanged: (bool state) {
										if(isConnected){
											_sendMessage(jsonEncode({'light' : state}));
										}
              						},
              						onDoubleTap: () {},
              						onSwipe: () {},
              						onTap: () {},
            					),
            					SleekCircularSlider(
              						min: 0,
              						max: 100,
              						initialValue: 50,
              						appearance: CircularSliderAppearance(
                						startAngle: 180,
                						angleRange: 180,
                						customColors: CustomSliderColors(
                  							progressBarColor: Colors.blue,
                  							trackColor: Colors.grey.withOpacity(0.4),
                						),
                						customWidths: CustomSliderWidths(
                  							progressBarWidth: 10,
                  							trackWidth: 10,
				  							handlerSize: 12,
                						),
                						size: 200,
										infoProperties: InfoProperties(
											bottomLabelText: 'Roller Blinds',
											bottomLabelStyle: const TextStyle(fontWeight: FontWeight.bold),
											modifier: (double value) {
												return '${value.toInt()}%';
											}
										)
              						),
              						onChangeEnd: (double value) {
										if(isConnected){
											_sendMessage(jsonEncode({'roller_blind' : value.toInt()}));
										}
              						},
            					)
          					],
        				),
      				),
    	);
  	}

	void _connectToDevice(){
		if(_connection != null){
			_connection?.close();
			_connection?.dispose();
			_connection = null;
		}
		setState(() {
		  isConnecting = true;
		});
		BluetoothConnection.toAddress(_selectedDevice!.address).then((connection) {
			print('Connected to the device');
			_connection = connection;
			setState(() {
			  isConnecting = false;
			  isDisconnecting = false;
			});
			_connection!.input!.listen(_onDataReceived).onDone(() {
				if (isDisconnecting) {
          			print('Disconnecting locally!');
        		} else {
          			print('Disconnected remotely!');
        		}
        		if (mounted) {
          			setState(() {});
        		}
			});
		}).catchError((error){
			print('Cannot connect, exception occurred');
			print(error);
			setState(() {
			  isConnecting = false;
			  isDisconnecting = false;
			  _connection = null;
			  _selectedDevice = null;
			});
			showDialog(
				context: context, 
				builder: (BuildContext context){
					return AlertDialog(
						title: const Text('ERROR'),
						content: const Text('An error occurred while connecting to the device, try later.'),
						actions: <Widget>[
							TextButton(
								onPressed: (){
									Navigator.of(context).pop();
								}, 
								child: const Text('Close'))
						],
					);
				}
			);
		});
	}

	void _onDataReceived(Uint8List data) {
		/**Allocate buffer for parsed data */
		int backspacesCounter = 0;
    	for(var byte in data) {
      		if (byte == 8 || byte == 127) {
        		backspacesCounter++;
      		}
    	}

		/**Apply backspace control character */
    	Uint8List buffer = Uint8List(data.length - backspacesCounter);
    	int bufferIndex = buffer.length;
		backspacesCounter = 0;
    	for (int i = data.length - 1; i >= 0; i--) {
      		if (data[i] == 8 || data[i] == 127) {
        		backspacesCounter++;
      		} else {
        		if (backspacesCounter > 0) {
          			backspacesCounter--;
        		} else {
          			buffer[--bufferIndex] = data[i];
       			}
      		}
    	}

		/**Create message if there is new line character */
		String dataString = String.fromCharCodes(buffer);
		int index = buffer.indexOf(13);
		if (~index != 0) {
			String message = (backspacesCounter > 0 ? _messageBuffer.substring(0, _messageBuffer.length - backspacesCounter) : _messageBuffer + dataString.substring(0, index)).replaceAll("\n", "").trim();
			_changeState(message.substring(message.indexOf('{'),message.length));
        	_messageBuffer = '';
    	} else {
      		_messageBuffer = (backspacesCounter > 0 ? _messageBuffer.substring(0, _messageBuffer.length - backspacesCounter) : _messageBuffer + dataString);
    	}	
	}

	void _sendMessage(String text) async {
		text = text.trim();
		if(text.isNotEmpty){
			try {
				_connection!.output.add(Uint8List.fromList(utf8.encode('$text\n')));
				//await _connection!.output.allSent;
			} catch (exception){
				print(exception);
			}
		}
	}

	void _changeState(String message) {
		try {
			final data = jsonDecode(message);
			if(data['light'] != null){
				setState(() {
					currentLightState = data['light'];
				});
			}
			if(data['roller_blind'] != null){
				setState(() {
					currentRollerBlindState = data['roller_blind'];
				});
			}
		} catch (exception) {
		  print("Invalid json format");
		}
	}
}