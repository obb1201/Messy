#pragma once
#pragma once

namespace fm
{
	struct Vertex
	{
		struct Position
		{
			double X;
			double Y;
			double Z;
			bool operator==(const Position& value) const 
			{
				bool equal = true;
				equal = equal && (this->X == value.X);
				equal = equal && (this->Y == value.Y);
				equal = equal && (this->Z == value.Z);

				return equal;
			}
		};

		struct TexCoord
		{
			double U;
			double V;
		};

		struct Normal 
		{
			double X;
			double Y;
			double Z;
			bool operator==(const Normal& value) const
			{
				bool equal = true;
				equal = equal && (this->X == value.X);
				equal = equal && (this->Y == value.Y);
				equal = equal && (this->Z == value.Z);

				return equal;
			}
		};

		struct Binormal 
		{
			double X;
			double Y;
			double Z;

			void operator=(const Vector& value) 
			{
				this->X = value.X;
				this->Y = value.Y;
				this->Y = value.Z;
			}

			void operator=(const Binormal& value) 
			{
				this->X = value.X;
				this->Y = value.Y;
				this->Y = value.Z;
			}

			bool operator!=(const Binormal& value) 
			{
				bool isDifferent = true;
				isDifferent = isDifferent && (this->X != value.X);
				isDifferent = isDifferent && (this->Y != value.Y);
				isDifferent = isDifferent && (this->Z != value.Z);

				return isDifferent;
			}
		};
		struct Tangent 
		{
			double X;
			double Y;
			double Z;
		};

		Position Position;
		TexCoord TexCoord;
		Normal Normal;
		Binormal Binormal;
		Tangent Tangent;

		bool operator==(const Vertex& value)  const
		{
			bool equal = true;
			equal = equal && (this->Position == value.Position);
			equal = equal && (this->Normal == value.Normal);
			return equal;
		}
	};
}
