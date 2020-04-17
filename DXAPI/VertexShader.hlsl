struct VSOut{
	float4 pos : SV_Position;
};

cbuffer cbuf 
{
	matrix transform;
};

VSOut main(float3 pos : Position) 
{
	VSOut returnVale;
	returnVale.pos = mul(float4(pos, 1.0f),transform);
	return returnVale;
}