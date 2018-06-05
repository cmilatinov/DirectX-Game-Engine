
cbuffer PROJECTION : register(b0) {
	float4x4 projectionMatrix;
};

cbuffer VIEW : register(b1) {
	float4x4 viewMatrix;
};

cbuffer CONSTANT_BUFFER : register(b2) {
	float4x4 modelMatrix;
};


struct VOut{
	float4 position : SV_POSITION;
	float2 uv : UV;
};


VOut main( float4 position : POSITION, float2 uv : UV ) {
	VOut output;

	output.position = mul(projectionMatrix, mul(viewMatrix, mul(modelMatrix, position)));
	output.uv = uv;

	return output;
}

