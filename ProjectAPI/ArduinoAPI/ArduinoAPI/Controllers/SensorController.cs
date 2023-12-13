using Microsoft.AspNetCore.Mvc;
using System.IO.Ports;

namespace ArduinoAPI.Controllers;

[Route("api/[controller]")]
[ApiController]
public class SensorController : ControllerBase
{
	private readonly SerialPort _serialPort;

	public SensorController()
	{
		_serialPort = new SerialPort("COM6", 9600);
		try
		{
			_serialPort.Open();
		}
		catch (Exception ex)
		{
			Console.WriteLine("Unable to open COM port - " + ex.Message);
		}
	}


	[HttpGet("TemperatureAndHumidity")]
	public IActionResult GetTemperatureAndHumidity()
	{
		try
		{
			if (_serialPort.IsOpen)
			{
				string sensorData = _serialPort.ReadLine();
				_serialPort.DiscardInBuffer();
				_serialPort.Close();


				var (Temperature, Humidity) = GetSensorData(sensorData);

				return Ok(new TemperatureHumidityData
				{
					Temperature = Temperature,
					Humidity = Humidity
				});
			}
			else
			{
				return StatusCode(500, "Serial port is not open");
			}
		}
		catch (Exception ex)
		{
			return StatusCode(500, "Error: " + ex.Message);
		}
	}

	[HttpPost("ChangeStepperSpeed")]
	public IActionResult ChangeLed([FromBody] string status)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				if (status == "low")
				{
					_serialPort.Write("1");
					_serialPort.Close();
					return Ok("Command sent to Arduino: Stepper speed LOW");
				}
				else if (status == "medium")
				{
					_serialPort.Write("2");
					_serialPort.Close();
					return Ok("Command sent to Arduino: Stepper speed MEDIUM");
				}
				else if (status == "high")
				{
					_serialPort.Write("3");
					_serialPort.Close();
					return Ok("Command sent to Arduino: Stepper speed HIGH");
				}
				else
				{
					return BadRequest("Invalid command.");
				}
			}
			else
			{
				return StatusCode(StatusCodes.Status500InternalServerError, "Serial port is not open");
			}
		}
		catch (Exception ex)
		{
			return StatusCode(StatusCodes.Status500InternalServerError, "Error: " + ex.Message);
		}


	}







	public class TemperatureHumidityData
	{
		public float Temperature { get; set; }
		public float Humidity { get; set; }
	}
	private (float Temperature, float Humidity) GetSensorData(string sensorData)
	{
		string[] parts = sensorData.Split(',');

		float temperature = float.Parse(parts[0].Split(':')[1]);
		float humidity = float.Parse(parts[1].Split(':')[1]);

		return (temperature, humidity);
	}

}


