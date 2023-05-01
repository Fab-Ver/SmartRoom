import 'package:flutter/material.dart';
import './HomePage.dart';

void main() => runApp(const SmartRoomApp());

class SmartRoomApp extends StatelessWidget {
  const SmartRoomApp({super.key});

  @override
  Widget build(BuildContext context){
    return MaterialApp(
    	home: const HomePage(),
		theme: ThemeData(
        	primarySwatch: Colors.blue,
      	),
		debugShowCheckedModeBanner: false,
    );
  }
}
