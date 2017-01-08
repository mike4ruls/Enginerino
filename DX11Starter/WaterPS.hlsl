
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	//float4 color		: COLOR;
	float2 uv			: TEXTCORD;
	float3 normal       : NORMAL;
	float3 viewDir		: DIR;
	
};
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};
//Custom method that calculates the lighting at that specific pixel
//Returns the color at that pixel (float4)
float4 LightOnColor(DirectionalLight light, float3 nor)
{

	float3 difColor = normalize(light.Direction * -1);
	float lightAmount = saturate(dot(difColor, nor));

	return (lightAmount * light.DiffuseColor) + (light.AmbientColor * 0.8);
}


float CalcSpecular(DirectionalLight lig, VertexToPixel inp, float3 nor)
{
	//float3 p = inp.normal + L;

	float3 h= normalize(-lig.Direction + inp.viewDir);

	float3 V = 2 * (nor*15) * dot(-nor, lig.Direction);
	float3 R = lig.Direction + V;

	//float3 dir = normalize(float3(lig.Direction) + pos);
	//float NdotH = saturate(dot(R, inp.viewDir));
	float NdotH = saturate(dot(inp.normal, h));

	return pow(NdotH, 3);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
	float4 surfaceColor;
	int on;
};
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 nor = normalize(input.normal);

	float specular = CalcSpecular(light, input, nor);

	float4 finalColor1 = LightOnColor(light, nor) * specular;
	float4 finalColor2 = LightOnColor(light2, nor) * 0.3;

	float4 textureColor = { 1.0,1.0,1.0,1.0 };

	if (on == 1) {
		textureColor = diffuseTexture.Sample(basicSampler, input.uv);
	}
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering

	/*
	//Directional Light
	float3 lightD = light.Direction;
	float3 norm = normalize(input.normal);
	float NdotL = dot(input.normal, -lightDir);

	//Point Light
	float3 lightD = normalize(input.worldPos - light.position);
	float3 norm = normalize(input.normal);
	float NdotL = dot(input.normal, -lightDir);

	//Spot Light
	float angleFromCenter = max(dot(-lightDir, light.direction), 0.0f);
	float spotAmount = pow(angleFromCenter, light.spotPower);*/


	//return (float4(1, 1, 1, 1)*lightAmount) + float4(1, 1, 1, 1);
	//return float4(nor, 1);
	//return (lightAmount * light.DiffuseColor) + (light.AmbientColor * 0.8);
	return textureColor * ((finalColor1 + finalColor2) * surfaceColor);
	//return ((finalColor1 + finalColor2) * surfaceColor);
	//return light.DiffuseColor;
	//return float4(1,1,1,1);
}