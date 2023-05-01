import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class DiscoverDevicePage extends StatefulWidget {
	final bool start;

  	const DiscoverDevicePage({super.key, this.start = true});

	@override
	State<DiscoverDevicePage> createState() => _DiscoverDevicePage();
}

class _DiscoverDevicePage extends State<DiscoverDevicePage>{
	StreamSubscription<BluetoothDiscoveryResult>? _streamSubscription;
	List<BluetoothDiscoveryResult> results = List<BluetoothDiscoveryResult>.empty(growable: true);
	bool isDiscovering = false;

	_DiscoverDevicePage();

	@override
	void initState() {
		super.initState();
		isDiscovering = widget.start;
		if(isDiscovering){
			_startDiscovery();
		}
	}

	void _startDiscovery() {
		_streamSubscription = FlutterBluetoothSerial.instance.startDiscovery().listen((event) { 
			setState(() {
				final existingIndex = results.indexWhere((element) => element.device.address == event.device.address);
				if(existingIndex >= 0){
					results[existingIndex] = event;
				} else {
					results.add(event);
				}
			});	
		});
		_streamSubscription!.onDone(() {
			setState(() {
			  isDiscovering = false;
			});
		});
	}

	void _restartDiscovery() {
		setState(() {
		  results.clear();
		  isDiscovering = true;
		});
		_startDiscovery();
	}

	@override 
	void dispose() {
		_streamSubscription?.cancel();
    	super.dispose();
  	}

  	@override
  	Widget build(BuildContext context) {
		return Scaffold(
			appBar: AppBar(
				title: isDiscovering
					? const Text('Discovering devices')
					: const Text('Discovered devices'),
				actions: <Widget>[
					isDiscovering
						? FittedBox(
							child: Container(
								margin: const EdgeInsets.all(16.0),
								child: const CircularProgressIndicator(valueColor: AlwaysStoppedAnimation<Color>(Colors.white),),
							),
						)
						: IconButton(onPressed: _restartDiscovery, icon: const Icon(Icons.replay)),
						IconButton(
							onPressed: () => showDialog(
								context: context, 
								builder: (BuildContext context) => AlertDialog(
									title: const Text('Bluetooth Info'),
									content: const Text('- Click on device to connect to it\n- Hold on the device to pair/unpair to it'),
									actions: <Widget>[
										TextButton(
              								onPressed: () => Navigator.pop(context, 'Close'),
              								child: const Text('Close'),
           								),
									],
								)
							), 
							icon: Icon(Icons.info_outline))
				],
			),
			body: ListView.builder(
				itemCount: results.length,
				itemBuilder: (BuildContext context, index) {
					BluetoothDiscoveryResult result = results[index];
					final device = result.device;
					final address = device.address;
					return ListTile(
						onTap: (){
							Navigator.of(context).pop(result.device);
						},
						onLongPress: () async {
							try{
								bool bonded = false;
								if(device.isBonded){
									print('Unbonding from ${device.address}...');
									await FlutterBluetoothSerial.instance.removeDeviceBondWithAddress(address);
									print('Unbonding from ${device.address} has succeed');
								} else {
									print('Bonding with ${device.address}...');
									bonded = (await FlutterBluetoothSerial.instance.bondDeviceAtAddress(address))!;
									print('Bonding with ${device.address} has ${bonded ? 'succeed' : 'failed'}.');
								}
								setState(() {
									results[results.indexOf(result)] = BluetoothDiscoveryResult(
										device: BluetoothDevice(
											name: device.name ?? '',
											address: address,
											type: device.type,
											bondState: bonded
												? BluetoothBondState.bonded
												: BluetoothBondState.none,
										),
										rssi: result.rssi,
									);
								});
							} catch (exception) {
								showDialog(
									context: context, 
									builder: (BuildContext context){
										return AlertDialog(
											title: const Text('Error occurred while bonding'),
											content: Text(exception.toString()),
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
							}
						},
						enabled: true,
						leading: const Icon(Icons.devices),
						title: Text(device.name ?? ''),
						subtitle: Text(device.address.toString()),
						trailing: Row(
							mainAxisSize: MainAxisSize.min,
							children: <Widget>[
								result.rssi != null
									? Container(
										margin: const EdgeInsets.all(8.0),
										child: DefaultTextStyle(
											style: _getTextStyle(result.rssi),
											child: Column(
												mainAxisSize: MainAxisSize.min,
												children: <Widget>[
													Text(result.rssi.toString()),
													const Text('dBm'),
												],
											)
										),
									)
									: const SizedBox(width: 0, height: 0,),
								device.isConnected
									? const Icon(Icons.import_export)
									: const SizedBox(width: 0, height: 0,),
								device.isBonded
									? const Icon(Icons.link)
									: const SizedBox(width: 0, height: 0,),
							],
						),
					);
				}
			),
		);
  	}

	TextStyle _getTextStyle(int rssi){
		if(rssi >= -55){
			return const TextStyle(color: Colors.green);
		} else if (rssi >= -75){
			return const TextStyle(color: Colors.amber);
		} else {
			return const TextStyle(color: Colors.red);
		}
	}
}