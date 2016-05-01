#ifndef COLORLED_H
#define COLORLED_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/// <summary>
/// Provides methods for interfacing with a 3-channel RGB LED.
/// </summary>
class ColorLED
{
public:

	/// <param name="redPin">Pin for the LED's red channel.</param>
	/// <param name="greenPin">Pin for the LED's green channel.</param>
	/// <param name="bluePin">Pin for the LED's blue channel.</param>
	inline ColorLED(const byte redPin, const byte greenPin, const byte bluePin) :
		_redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {}

	/// <summary>
	/// Initializes the connection to the LED.
	/// </summary>
	inline void begin();

	/// <summary>
	/// Sets RGB (red, green, blue) values for the LED's color.
	/// </summary>
	/// <param name="red">0-255 value, 0 being black and 255 being maximum red.</param>
	/// <param name="green">0-255 value, 0 being black and 255 being maximum green.</param>
	/// <param name="blue">0-255 value, 0 being black and 255 being maximum blue.</param>
	inline void rgb(const byte red, const byte green, const byte blue);
	/// <summary>
	/// Sets HSL (hue, saturation, lightness) values for the LED's color.
	/// </summary>
	/// <param name="hue">0-360 value shifting through the entire color spectrum.</param>
	/// <param name="saturation">0-100 value, 0 being gray and 100 being maximum hue.</param>
	/// <param name="lightness">0-100 value, 0 being black, 100 being white, and 50 being maximum hue.</param>
	inline void hsl(const word hue, const byte saturation, const byte lightness);

protected:

	/// <summary>
	/// Preforms calculation for a HSL channel.
	/// </summary>
	inline double _hsl_channel(const double p, const double q, double t);

	byte _redPin, _greenPin, _bluePin;

};

inline void ColorLED::begin()
{
	pinMode(this->_redPin, OUTPUT);
	pinMode(this->_greenPin, OUTPUT);
	pinMode(this->_bluePin, OUTPUT);
}

inline double ColorLED::_hsl_channel(const double p, const double q, double t)
{
	if (t < 0.0) t += 1.0;
	if (t > 1.0) t -= 1.0;
	if (t < 0.16666667) return p + (q - p) * 6.0 * t;
	else if (t < 0.5) return q;
	else if (t < 0.66666667) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
	else return p;
}

inline void ColorLED::rgb(const byte red, const byte green, const byte blue)
{
	analogWrite(this->_redPin, red);
	analogWrite(this->_greenPin, green);
	analogWrite(this->_bluePin, blue);
}
inline void ColorLED::hsl(const word hue, const byte saturation, const byte lightness)
{
	byte r, g, b;
	if (saturation == 0)
	{
		r = g = b = (byte)(((float)(lightness % 101) / 100.0f) * 255.0f);
	}
	else
	{
		static double h, l, s;
		h = (double)(hue % 360) / 360.0;
		l = (double)(lightness % 101) / 100.0;
		s = (double)(saturation % 101) / 100.0;
		static double p, q;
		q = (l < 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
		p = 2.0 * l - q;
		r = (byte)(this->_hsl_channel(p, q, h + 0.33333333) * 255.0);
		g = (byte)(this->_hsl_channel(p, q, h) * 255.0);
		b = (byte)(this->_hsl_channel(p, q, h + 0.66666667) * 255.0);
	}

	this->rgb(r, g, b);
}

#endif