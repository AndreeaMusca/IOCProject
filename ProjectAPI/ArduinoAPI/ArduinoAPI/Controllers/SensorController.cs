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

	[HttpPut("ChangeStepperSpeed")]
	public IActionResult ChangeStepperSpeed([FromBody] int status)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				if (status == 1)
				{
					_serialPort.Write("S:1");
					_serialPort.Close();
					return Ok("Command sent to Arduino: Stepper speed LOW");
				}
				else if (status == 2)
				{
					_serialPort.Write("S:2");
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

	[HttpPut("ChangeBuzzerTone")]
	public IActionResult ChangeBuzzerTone([FromBody] int newBuzzerTone)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				_serialPort.Write($"B:{newBuzzerTone}");
				_serialPort.Close();
				return Ok($"Command sent to Arduino: BuzzerTone: {newBuzzerTone}");

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


	#region Temperature and Humidity bounds

	[HttpPut("ChangeMaxTemperature")]
	public IActionResult ChangeMaxT([FromBody] int newTemperature)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				_serialPort.Write($"M:{newTemperature}");
				_serialPort.Close();
				return Ok($"Command sent to Arduino: MaxTemperature: {newTemperature}");

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

	[HttpPut("ChangeMinTemperature")]
	public IActionResult ChangeMinT([FromBody] int newTemperature)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				_serialPort.Write($"m:{newTemperature}");
				_serialPort.Close();
				return Ok($"Command sent to Arduino: MinTemperature: {newTemperature}");

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

	[HttpPut("ChangeMaxHumidity")]
	public IActionResult ChangeMaxH([FromBody] int newHumidity)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				_serialPort.Write($"H:{newHumidity}");
				_serialPort.Close();
				return Ok($"Command sent to Arduino: MaxHumidity: {newHumidity}");

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

	[HttpPut("ChangeMinHumidity")]
	public IActionResult ChangeMinH([FromBody] int newHumidity)
	{
		try
		{

			if (_serialPort.IsOpen)
			{
				_serialPort.Write($"h:{newHumidity}");
				_serialPort.Close();
				return Ok($"Command sent to Arduino: MinHumidity: {newHumidity}");

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

	#endregion

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


