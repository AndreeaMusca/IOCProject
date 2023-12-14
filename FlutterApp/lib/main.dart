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
  String selectedBuzzerOption = 'Option 1';
  String selectedMotorOption = 'Option A';
  double spinBoxMaxTValue = 0;
  double spinBoxMinTValue = 0;
  double spinBoxMaxHValue = 0;
  double spinBoxMinHValue = 0;
  TextEditingController idController = TextEditingController();

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
              buildSpinBoxWithButton(
                  spinBoxMinTValue, 'Min temperature', () async {
                var url = Uri.http('http://localhost:5220/api/Sensor/ChangeMinTemperature');
                var response = await http.put(url, body: {spinBoxMinTValue});
              }),
              buildSpinBoxWithButton(
                  spinBoxMaxTValue, 'Max temperature', () async {
                var url = Uri.http('localhost:5220', '/api/Sensor/ChangeMaxTemperature');
                var response = await http.put(
                    url,
                    headers: {'Content-Type': 'application/json'},
                    body: spinBoxMaxTValue.toString());
                print(response);
              }),
              buildSpinBoxWithButton(spinBoxMaxHValue, 'Min humidity', () { }),
              buildSpinBoxWithButton(spinBoxMinHValue, 'Max humidity', () {}),
              const SizedBox(height: 16),
              buildButtonUnderDropdown(
                'Buzzer',
                ['Option 1', 'Option 2', 'Option 3'],
                selectedBuzzerOption,
                (String? newValue) {
                  setState(() {
                    selectedBuzzerOption = newValue!;
                  });
                },
                () {},
              ),
              const SizedBox(height: 16),
              buildButtonUnderDropdown(
                'Motor',
                ['Option A', 'Option B'],
                selectedMotorOption,
                (String? newValue) {
                  setState(() {
                    selectedMotorOption = newValue!;
                  });
                },
                () {},
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
          buildDataRow('Temperature:', '25°C'),
          const SizedBox(height: 16),
          buildDataRow('Humidity:', '50%'),
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
