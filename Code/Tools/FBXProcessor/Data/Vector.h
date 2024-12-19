#pragma once
#pragma once
namespace fm
{
	struct Vector
	{
		double X;
		double Y;
		double Z;

		const Vector CrossProduct(const Vector& value)
		{
			double crossX = ((this->Y*value.Z) - (this->Z*value.Y));
			double crossY = ((this->Z*value.X) - (this->X*value.Z));
			double crossZ = ((this->X*value.Y) - (this->Y*value.X));

			Vector crossVector;
			crossVector.X = crossX;
			crossVector.Y = crossY;
			crossVector.Z = crossZ;

			return crossVector;
		}

		const int32 DotProduct(const Vector& value)
		{
			return this->X*value.X + this->Y*value.Y + this->Z*value.Z;
		}

		const void operator=(const Vector& value)
		{
			this->X = value.X;
			this->Y = value.Y;
			this->Z = value.Z;
		}
	};
}