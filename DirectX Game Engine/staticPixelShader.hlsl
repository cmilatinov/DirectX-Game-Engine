Texture2D cubeTex : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 position : SV_POSITION, float2 uv : UV) : SV_TARGET{
	float4 outColor;

	outColor = cubeTex.Sample(samplerState, uv);

	return outColor;
}
