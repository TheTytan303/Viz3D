#pragma once
#include "Graphics.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Bindable;
class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void Draw(Graphics& gfx) const noexcept;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	
	//template<class T>
	//void AddColorBuffer(std::unique_ptr<class PixelConstantBuffer<T> cbuf) noexcept;


	virtual ~Drawable() = default; //TODO: check
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

	const class IndexBuffer* pIndexBuffer = nullptr;
	//ConstantBuffer<ColorBuffer>* pColorBuffer;
	std::vector<std::unique_ptr<Bindable>> binds;
};