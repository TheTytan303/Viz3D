//cbuffer CBuf
//{
//	float3 vertex_colors[8];
//};

//struct VSIn
//{
//	float4 pos : SV_POSITION;
//	uint vid: VertexID
//};
//float4 main(VSIn input) : SV_Target
//{
//	return float4(vertex_colors[input.vid],0);
//}


/*
cbuffer CBuf
{
	float3 vertex_colors[8];
};
float4 main(uint vid : SV_VertexID) : SV_Target
{
	return float4(vertex_colors[vid],0);
}
*/


float4 main(float3 color : Color) : SV_Target
{
	return float4(color, 1.0f);
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

/*
cbuffer CBuf
{
	float4 vertex_colors[8];
};
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return vertex_colors[tid];
}
*/