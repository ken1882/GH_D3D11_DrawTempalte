#ifndef __SHADER_H__
#define __SHADER_H__

constexpr const char* shader = R"(
// Constant buffer
cbuffer ConstantBuffer : register(b0)
{
	matrix projection;
}

// PSI (PixelShaderInput)
struct PSI
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

// VertexShader
PSI VS( float4 pos : POSITION, float4 color : COLOR )
{
	PSI psi;
	psi.pos = mul( projection, pos  );
	psi.color = color;
	return psi;
}

// PixelShader
float4 PS(PSI psi) : SV_TARGET
{
	return psi.color;
}
)";

#endif // __SHADER_H__
