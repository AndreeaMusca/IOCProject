import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_spinbox/flutter_spinbox.dart';
import 'package:http/http.dart' as http;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Sensor Control App',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String selectedBuzzerOption = 'Tone 1';
  String selectedMotorOption = 'Slow';
  double spinBoxMaxTValue = 0;
  double spinBoxMinTValue = 0;
  double spinBoxMaxHValue = 0;
  double spinBoxMinHValue = 0;
  double temperature = 0.0;
  double humidity = 0.0;
  @override
  void initState() {
    super.initState();
    // Start the timer when the widget is initialized
    startTimer();
  }

  @override
  void dispose() {
    // Dispose the timer when the widget is disposed
    _timer?.cancel();
    super.dispose();
  }

  Timer? _timer;

  void startTimer() {
    // Refresh temperature and humidity every 3 seconds
    _timer = Timer.periodic(Duration(seconds: 3), (timer) async {
      await fetchTemperatureAndHumidity();
    });
  }

  Future<void> fetchTemperatureAndHumidity() async {
    try {
      var url = Uri.http('10.0.2.2:5220', '/api/Sensor/TemperatureAndHumidity');
      var response = await http.get(url);

      if (response.statusCode == 200) {
        Map<String, dynamic> data = json.decode(response.body);
        setState(() {
          temperature = data['temperature'];
          humidity = data['humidity'];
        });
      } else {
        // Handle error cases if needed
        print('Failed to fetch data: ${response.statusCode}');
      }
    } catch (e) {
      print('Error: $e');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sensor Control'),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              buildSensorDataContainer(),
              const SizedBox(height: 16),
              buildSpinBoxWithButton(spinBoxMinTValue, 'Min temperature',
                  () async {
                var url = Uri.http(
                    '10.0.2.2:5220', '/api/Sensor/ChangeMinTemperature');
                var response = await http.put(url,
                    headers: {'Content-Type': 'application/json'},
                    body: spinBoxMinTValue.toInt().toString());
              }),
              buildSpinBoxWithButton(spinBoxMaxTValue, 'Max temperature',
                  () async {
                var url = Uri.http(
                    '10.0.2.2:5220', '/api/Sensor/ChangeMaxTemperature');
                var response = await http.put(url,
                    headers: {'Content-Type': 'application/json'},
                    body: spinBoxMaxTValue.toInt().toString());
                print(response.body);
              }),
              buildSpinBoxWithButton(spinBoxMaxHValue, 'Min humidity',
                  () async {
                var url =
                    Uri.http('10.0.2.2:5220', '/api/Sensor/ChangeMinHumidity');
                var response = await http.put(url,
                    headers: {'Content-Type': 'application/json'},
                    body: spinBoxMinHValue.toInt().toString());
              }),
              buildSpinBoxWithButton(spinBoxMinHValue, 'Max humidity',
                  () async {
                var url =
                    Uri.http('10.0.2.2:5220', '/api/Sensor/ChangeMaxHumidity');
                var response = await http.put(url,
                    headers: {'Content-Type': 'application/json'},
                    body: spinBoxMaxHValue.toInt().toString());
              }),
              const SizedBox(height: 16),
              buildButtonUnderDropdown(
                'Buzzer Tone',
                ['Tone 1', 'Tone 2', 'Tone 3'],
                selectedBuzzerOption,
                (String? newValue) {
                  setState(() {
                    selectedBuzzerOption = newValue!;
                  });
                },
                () async {
                  int x = 1;
                  if (selectedBuzzerOption == 'Tone 1') x = 1;
                  if (selectedBuzzerOption == 'Tone 2') x = 2;
                  if (selectedBuzzerOption == 'Tone 3') x = 3;
                  var url =
                      Uri.http('10.0.2.2:5220', '/api/Sensor/ChangeBuzzerTone');
                  var response = await http.put(
                    url,
                    headers: {'Content-Type': 'application/json'},
                    body: x.toString(),
                  );
                },
              ),
              const SizedBox(height: 16),
              buildButtonUnderDropdown(
                'Motor Speed',
                ['Slow', 'High'],
                selectedMotorOption,
                (String? newValue) {
                  setState(() {
                    selectedMotorOption = newValue!;
                  });
                },
                () async {
                  int x= 1;
                  if(selectedMotorOption=='Slow') x=1;
                  if(selectedMotorOption=='High') x=2;
                  var url = Uri.http(
                      '10.0.2.2:5220', '/api/Sensor/ChangeStepperSpeed');
                  var response = await http.put(url,
                      headers: {'Content-Type': 'application/json'},
                      body: x.toString());
                  print(response.body);
                  print(x);
                },
              ),
            ],
          ),
        ),
      ),
    );
  }

  Container buildSensorDataContainer() {
    return Container(
      padding: const EdgeInsets.all(8.0),
      decoration: BoxDecoration(
        color: Colors.blue,
        borderRadius: BorderRadius.circular(8.0),
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          buildDataRow('Temperature:', '$temperature°C'),
          const SizedBox(height: 16),
          buildDataRow('Humidity:', '$humidity%'),
        ],
      ),
    );
  }

  Row buildDataRow(String label, String value) {
    return Row(
      children: [
        Text(
          label,
          style: const TextStyle(fontSize: 18, color: Colors.white),
        ),
        Text(
          value,
          style: const TextStyle(fontSize: 18, color: Colors.white),
        ),
      ],
    );
  }

  Column buildSpinBoxWithButton(
      double spinBoxValue, String label, void Function() onPressed) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          '$label:',
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
        SpinBox(
          value: spinBoxValue,
          onChanged: (value) {
            setState(() {
              if (label == 'Min temperature') {
                spinBoxMinTValue = value;
              } else if (label == 'Max temperature') {
                spinBoxMaxTValue = value;
              } else if (label == 'Min humidity') {
                spinBoxMinHValue = value;
              } else if (label == 'Max humidity') {
                spinBoxMaxHValue = value;
              }
            });
          },
        ),
        SizedBox(
          width: double.infinity,
          child: TextButton(
            onPressed: onPressed,
            style: TextButton.styleFrom(
              backgroundColor: Colors.blue,
              primary: Colors.white,
              padding: const EdgeInsets.symmetric(vertical: 16.0),
            ),
            child: const Text(
              'Update',
              style: TextStyle(fontSize: 16),
            ),
          ),
        ),
        const SizedBox(height: 16),
      ],
    );
  }

  Widget buildButtonUnderDropdown(
    String label,
    List<String> options,
    String selectedValue,
    void Function(String?) onChanged,
    void Function() onPressed,
  ) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          '$label:',
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
        SizedBox(
          width: double.infinity,
          child: DropdownButton<String>(
            value: selectedValue,
            onChanged: onChanged,
            items: options.map<DropdownMenuItem<String>>((String value) {
              return DropdownMenuItem<String>(
                value: value,
                child: Text(value, style: const TextStyle(fontSize: 16)),
              );
            }).toList(),
          ),
        ),
        SizedBox(
          width: double.infinity,
          child: TextButton(
            onPressed: onPressed,
            style: TextButton.styleFrom(
              backgroundColor: Colors.blue,
              primary: Colors.white,
              padding: const EdgeInsets.symmetric(vertical: 16.0),
            ),
            child: const Text(
              'Update',
              style: TextStyle(fontSize: 16),
            ),
          ),
        ),
        const SizedBox(height: 16),
      ],
    );
  }
}
