#include "Camera.h"



namespace dx = DirectX;

dx::XMMATRIX Camera::getMatrix() const noexcept
{
	const auto pos = getPosition();
	return dx::XMMatrixLookAtLH(
		pos, dx::XMVectorZero(),
		dx::XMVectorSet(0, 1, 0, 0)
	) * dx::XMMatrixRotationRollPitchYaw(
		pitch, -yaw, roll
	);
}

void Camera::SpawnControlWindow() noexcept
{
	//TODO
}

void Camera::Reset() noexcept
{
	r = 20;
	theta = 0;
	phi = 0;
	pitch = 0;
	roll = 0;
	yaw = 0;
}
void Camera::UpdateR(float r) noexcept
{
	this->r += r;
}
void Camera::UpdateRollPitchYaw(float roll, float pitch, float yaw) noexcept
{
	this->pitch += pitch;
	this->roll += roll;
	this->yaw += yaw;
}
void Camera::UpdateCamera(float r, float theta, float phi, float pitch, float roll, float yaw)
{
	this->r		+= r;
	this->theta	+= theta;
	this->phi	+= phi;
	this->pitch	+= pitch;
	this->roll	+= roll;
	this->yaw	+= yaw;
}
void Camera::SetCamera(float r, float theta, float phi, float pitch, float roll, float yaw)
{
	this->r		= r;
	this->theta = theta;
	this->phi	= phi;
	this->pitch = pitch;
	this->roll	= roll;
	this->yaw	= yaw;
}

void Camera::SetCamera(DirectX::XMMATRIX)
{
	//TODO
}

DirectX::XMVECTOR Camera::getPosition() const noexcept
{
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0, 0, r, 0),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0)
	);
	return pos;
}

DirectX::XMVECTOR Camera::getDirection() const noexcept
{
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0, 0, r, 0),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0)
	);
	DirectX::XMVECTOR raydirection = DirectX::XMVectorSet(
		0 - DirectX::XMVectorGetX(pos),
		0 - DirectX::XMVectorGetY(pos),
		0 - DirectX::XMVectorGetZ(pos),
		0 - DirectX::XMVectorGetW(pos)
	);
	raydirection = DirectX::XMVector3Normalize(raydirection);
	return raydirection;
}

