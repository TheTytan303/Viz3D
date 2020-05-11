cbuffer CBuf
{
	float4 face_colors[1];
};
float4 main() : SV_Target
{
	return face_colors[0];
}