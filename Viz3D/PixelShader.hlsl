cbuffer CBuf
{
	float4 face_colors[1];
};
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	return face_colors[0];
}


/*
cbuffer CBuf
{
	float4 face_colors[1];
};



float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{

	return face_colors[tid/2];
}
*/