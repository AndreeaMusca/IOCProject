using Microsoft.AspNetCore.Mvc;
using System.IO.Ports;

namespace ArduinoAPI.Controllers
{
	[Route("api/[controller]")]
	[ApiController]
	public class ArduinoController : ControllerBase
	{
		private SerialPort _serialPort;

		public ArduinoController()
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


		[HttpPut("ChangeLEDStatus")]
		public IActionResult ChangeLed([FromBody] string status)
		{
			try
			{ 
				if (_serialPort.IsOpen)
				{
					if (status == "on")
					{
						_serialPort.Write("1");
						_serialPort.Close();
						return Ok("Command sent to Arduino: LED turned ON");
					}
					else if (status == "off")
					{
						_serialPort.Write("0");
						_serialPort.Close();
						return Ok("Command sent to Arduino: LED turned OFF");
					}
					else
					{
						return BadRequest("Invalid command. Use 'on' or 'off'.");
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


	}
}
