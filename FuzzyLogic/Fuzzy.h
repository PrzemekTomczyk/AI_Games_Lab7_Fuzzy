#pragma once

namespace FuzzyLogic
{
	inline double Grade(double t_val, double t_x0, double t_x1)
	{
		double result = 0.0;
		double x = t_val;

		if (x <= t_x0)
		{
			result = 0;
		}
		else if (x > t_x1)
		{
			result = 1;
		}
		else
		{
			result = ((x - t_x0) / (t_x1 - t_x0));
		}

		return result;
	}

	inline double Triangle(double t_val, double t_x0, double t_x1, double t_x2)
	{
		double result = 0.0;
		double x = t_val;

		if ((x <= t_x0) || (x >= t_x2))
		{
			result = 0.0;
		}
		else if (x == t_x1)
		{
			result = 1.0;
		}
		else if ((x > t_x0) && (x < t_x1))
		{
			result = ((x - t_x0) / (t_x1 - t_x0));
		}
		else
		{
			result = ((t_x2 - x) / (t_x2 - t_x1));
		}

		return result;
	}

	inline double Trapezoid(double t_val, double t_x0, double t_x1, double t_x2, double t_x3)
	{
		double result = 0.0;
		double x = t_val;

		if ((x <= t_x0) || (x >= t_x3))
		{
			result = 0.0;
		}
		else if ((x >= t_x1) && (x <= t_x2))
		{
			result = 1.0;
		}
		else if ((x > t_x0) && (x < t_x1))
		{
			result = ((x - t_x0) / (t_x1 - t_x0));
		}
		else
		{
			result = ((t_x3 - x) / (t_x3 - t_x2));
		}

		return result;
	}

	inline double AND(double t_a, double t_b)
	{
		return std::min(t_a, t_b);
	}

	inline double OR(double t_a, double t_b)
	{
		return std::max(t_a, t_b);
	}

	inline double NOT(double t_a)
	{
		return 1.0 - t_a;
	}
}