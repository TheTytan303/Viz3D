#include "Topology.h"
#include "macros.h"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	type(type)
{}
void Topology::Bind(Graphics& gfx) noexcept
{
	INFOMAN(gfx);
	//GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(type));
	GetContext(gfx)->IASetPrimitiveTopology(type);
}