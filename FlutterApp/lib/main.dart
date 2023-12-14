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
  ValueNotifier<double> spinBox1Value = ValueNotifier(0);
  ValueNotifier<double> spinBox2Value = ValueNotifier(0);
  ValueNotifier<double> spinBox3Value = ValueNotifier(0);
  ValueNotifier<double> spinBox4Value = ValueNotifier(0);
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
              Container(
                padding: const EdgeInsets.all(8.0),
                decoration: BoxDecoration(
                  color: Colors.blue,
                  borderRadius: BorderRadius.circular(8.0),
                ),
                child: const Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Row(
                      children: [
                        Text(
                          'Temperature:',
                          style: TextStyle(fontSize: 18, color: Colors.white),
                        ),
                        Text(
                          '25°C',
                          style: TextStyle(fontSize: 18, color: Colors.white),
                        ),
                      ],
                    ),
                    SizedBox(height: 16),
                    Row(
                      children: [
                        Text(
                          'Humidity:',
                          style: TextStyle(fontSize: 18, color: Colors.white),
                        ),
                        Text(
                          '50%',
                          style: TextStyle(fontSize: 18, color: Colors.white),
                        ),
                      ],
                    ),
                  ],
                ),
              ),
              const SizedBox(height: 16),

              TextField(
                controller: idController,
                style: const TextStyle(fontSize: 18),
                decoration: const InputDecoration(
                  labelText: 'Enter ID',
                  labelStyle: TextStyle(fontSize: 18),
                ),
              ),
              const SizedBox(height: 16),

              SizedBox(
                width: double.infinity,
                child: TextButton(
                  onPressed: () {
                    print('Update ID pressed');
                  },
                  style: TextButton.styleFrom(
                    backgroundColor: Colors.blue,
                    primary: Colors.white,
                    padding: const EdgeInsets.symmetric(vertical: 16.0),
                  ),
                  child: const Text(
                    'Update ID',
                    style: TextStyle(fontSize: 16),
                  ),
                ),
              ),
              const SizedBox(height: 16),

              buildSpinBoxWithButton(spinBox1Value, 'Min temperature', spinBox2Value.value, spinBox2Value),
              buildSpinBoxWithButton(spinBox2Value, 'Max temperature', spinBox1Value.value, spinBox1Value),
              buildSpinBoxWithButton(spinBox3Value, 'Min humidity', spinBox4Value.value, spinBox4Value),
              buildSpinBoxWithButton(spinBox4Value, 'Max humidity', spinBox3Value.value, spinBox3Value),

              const SizedBox(height: 16),
              buildButtonUnderDropdown('Buzzer', ['Option 1', 'Option 2', 'Option 3'], selectedBuzzerOption, (String? newValue) {
                setState(() {
                  selectedBuzzerOption = newValue!;
                });
              }),
              const SizedBox(height: 16),
              buildButtonUnderDropdown('Motor', ['Option A', 'Option B'], selectedMotorOption, (String? newValue) {
                setState(() {
                  selectedMotorOption = newValue!;
                });
              }),
            ],
          ),
        ),
      ),
    );
  }

  Column buildSpinBoxWithButton(ValueNotifier<double> spinBoxValue, String label, double maxValue, ValueNotifier<double> oppositeSpinBoxValue) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          '$label:',
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
        SpinBox(
          value: spinBoxValue.value,
          onChanged: (value) {
            if (value <= maxValue && value <= oppositeSpinBoxValue.value) {
              spinBoxValue.value = value;
            } else {
              print('Value cannot be higher than Max value');
            }
          },
        ),
        SizedBox(
          width: double.infinity,
          child: TextButton(
            onPressed: () {
              print('Button pressed for $label');
            },
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

  Widget buildButtonUnderDropdown(String label, List<String> options, String selectedValue, void Function(String?) onChanged) {
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
            items: options
                .map<DropdownMenuItem<String>>((String value) {
              return DropdownMenuItem<String>(
                value: value,
                child: Text(value, style: const TextStyle(fontSize: 16)),
              );
            })
                .toList(),
          ),
        ),
        SizedBox(
          width: double.infinity,
          child: TextButton(
            onPressed: () {
              print('Button pressed for $label');
            },
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