#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
class Camera
{
private:
	//DirectX::XMMATRIX fam;
	float r = 20;
	float theta;
	float phi;
	float pitch;
	float roll;
	float yaw;
public:
	DirectX::XMMATRIX getMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void UpdateR(float r) noexcept;
	void UpdateRollPitchYaw(float roll, float pitch, float yaw) noexcept;
	void UpdateCamera(float r, float theta, float phi, float pitch, float roll, float yaw);
	void SetCamera(float r, float theta, float phi, float pitch, float roll, float yaw);
	void SetCamera(DirectX::XMMATRIX);

	DirectX::XMVECTOR getPosition() const noexcept;
	DirectX::XMVECTOR getDirection() const noexcept;

	//DirectX::XMMatrixRotationY(angleY) *
	//DirectX::XMMatrixRotationX(angleX) *
	//DirectX::XMMatrixRotationZ(angleZ) *  
	//DirectX::XMMatrixTranslation(x, y, z)
};