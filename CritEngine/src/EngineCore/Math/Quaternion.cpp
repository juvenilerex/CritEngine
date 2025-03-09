#include "Quaternion.h"
#include <cmath>

namespace Engine {

	Quaternion Quaternion::FromEulerAngles(Vector3 angles)
	{
		float roll = angles.x;
		float pitch = angles.y;
		float yaw = angles.z;

		float cosRoll = cosf(roll * 0.5f);
		float sinRoll = sinf(roll * 0.5f);
		float cosPitch = cosf(pitch * 0.5f);
		float sinPitch = sinf(pitch * 0.5f);
		float cosYaw = cosf(yaw * 0.5f);
		float sinYaw = sinf(yaw * 0.5f);

		return Quaternion(
			sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
			cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
			cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
			cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
		);
	}

	Quaternion Quaternion::FromAxisAngle(const Vector3 axis, float angle)
	{
		Vector3 normalizedAxis = axis.Normalize();
		float sinHalfAngle = sinf(angle * 0.5f);
		float cosHalfAngle = cosf(angle * 0.5f);
		return Quaternion(cosHalfAngle, normalizedAxis.x * sinHalfAngle, normalizedAxis.y * sinHalfAngle, normalizedAxis.z * sinHalfAngle);
	}

	Quaternion Quaternion::Mul(const Quaternion& first, const Quaternion& second)
	{
		return Quaternion(
			first.w * second.x + first.x * second.w + first.y * second.z - first.z * second.y,
			first.w * second.y - first.x * second.z + first.y * second.w + first.z * second.x,
			first.w * second.z + first.x * second.y - first.y * second.x + first.z * second.w,
			first.w * second.w - first.x * second.x - first.y * second.y - first.z * second.z
		);
	}

	Quaternion Quaternion::Conjugate(const Quaternion& base)
	{
		return Quaternion(-base.x, -base.y, -base.z, base.w);
	}

	Quaternion Quaternion::Normalize(const Quaternion& base)
	{
		float length = sqrtf(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);
		if (length > 0.0f)
		{
			return Quaternion(
				base.x / length,
				base.y / length,
				base.z / length,
				base.w / length
			);
		}
		else
		{
			return Quaternion();
		}
	}

	Quaternion Quaternion::SlerpShort(const Quaternion& start, const Quaternion& end, float alpha)
	{
		float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

		int flippedMult = 1;
		if (dot < 0.0f)
		{

			flippedMult = -1;
			dot = -dot; 
		}

		if (dot > 0.9995f)
		{
			Quaternion result(
				start.x + alpha * (end.x * flippedMult - start.x),
				start.y + alpha * (end.y * flippedMult - start.y),
				start.z + alpha * (end.z * flippedMult - start.z),
				start.w + alpha * (end.w * flippedMult - start.w)
			);
			result.Normalize();
			return result;
		}
		else
		{
			float theta_0 = std::acos(dot);
			float theta = theta_0 * alpha;
			float sin_theta = std::sin(theta_0);
			float sin_theta_t = std::sin(theta);
			
			float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
			float s1 = sin_theta_t / sin_theta;

			return Quaternion(
				s0 * start.x + s1 * end.x * flippedMult,
				s0 * start.y + s1 * end.y * flippedMult,
				s0 * start.z + s1 * end.z * flippedMult,
				s0 * start.w + s1 * end.w * flippedMult
			);
		}
	}

	Quaternion Quaternion::SlerpLong(const Quaternion& start, const Quaternion& end, float alpha)
	{
		float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

		int flippedMult = 1;
		if (dot > 0.0f)
		{

			flippedMult = -1;
			dot = -dot;
		}

		if (dot < -0.9995f)
		{
			Quaternion result(
				start.x + alpha * (end.x * flippedMult - start.x),
				start.y + alpha * (end.y * flippedMult - start.y),
				start.z + alpha * (end.z * flippedMult - start.z),
				start.w + alpha * (end.w * flippedMult - start.w)
			);
			result.Normalize();
			return result;
		}
		else
		{
			float theta_0 = std::acos(dot);
			float theta = theta_0 * alpha;
			float sin_theta = std::sin(theta_0);
			float sin_theta_t = std::sin(theta);

			float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
			float s1 = sin_theta_t / sin_theta;

			return Quaternion(
				s0 * start.x + s1 * end.x * flippedMult,
				s0 * start.y + s1 * end.y * flippedMult,
				s0 * start.z + s1 * end.z * flippedMult,
				s0 * start.w + s1 * end.w * flippedMult
			);
		}
	}

	Vector3 Quaternion::RotateVector(const Quaternion& base, const Vector3& vector)
	{
		Vector3 quatVector = Vector3(base.x, base.y, base.z);
		float scalar = base.w;

		return
			2.0f * Vector3::Dot(quatVector, vector) * quatVector +
			(scalar * scalar - Vector3::Dot(quatVector, quatVector)) * vector +
			2.0f * scalar * Vector3::Cross(quatVector, vector);
	}

	Vector3 Quaternion::ToEulerAngles(const Quaternion& base)
	{
		float roll = atan2f(2.0f * (base.w * base.x + base.y * base.z), 1.0f - 2.0f * (base.x * base.x + base.y * base.y));
		float pitch = asinf(std::clamp(2.0f * (base.w * base.y - base.z * base.x), -1.0f, 1.0f));
		float yaw = atan2f(2.0f * (base.w * base.z + base.x * base.y), 1.0f - 2.0f * (base.y * base.y + base.z * base.z));

		return Vector3(roll, pitch, yaw);
	}
}