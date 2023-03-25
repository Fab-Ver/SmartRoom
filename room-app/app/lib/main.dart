import 'package:app/bluetooth_settings.dart';
import 'package:flutter/material.dart';
import 'package:lite_rolling_switch/lite_rolling_switch.dart';
import 'package:sleek_circular_slider/sleek_circular_slider.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Room',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
	  initialRoute: '/',
	  routes: {
		'/' : (context) => const HomePage(title: 'Smart Room App'),
		'/bluetooth' : (context) => const BluetoothPage(title: 'Bluetooth Settings')
	  },
	  debugShowCheckedModeBanner: false,
      //home: const HomePage(title: 'Smart Room App'),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({super.key, required this.title});

  final String title;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
		actions: <Widget>[
			IconButton(
				onPressed: () => Navigator.pushNamed(context, '/bluetooth'),
				icon: const Icon(Icons.bluetooth),
				tooltip: 'Bluetooth Settings',),
		],
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            LiteRollingSwitch(
              value: true,
              textOn: 'ON',
              textOff: 'OFF',
              colorOn: Colors.green,
              colorOff: Colors.red,
              iconOn: Icons.light_mode,
              iconOff: Icons.light_mode_outlined,
              animationDuration: const Duration(milliseconds: 100),
              onChanged: (bool state) {
                print(state);
              },
              onDoubleTap: () {},
              onSwipe: () {},
              onTap: () {},
            ),
            SleekCircularSlider(
              min: 0,
              max: 100,
              initialValue: 0,
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
                print(value.toInt());
              },
            )
          ],
        ),
      ),
    );
  }
}
