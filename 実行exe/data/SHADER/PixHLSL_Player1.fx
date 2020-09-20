sampler2D baseMap;
float player_color;
float player_alpha;
texture Tex0;

//纹理采样器0
sampler Samp0 = sampler_state
{
	Texture = <Tex0>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct PS_INPUT 
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD0;
	float4 Texcolor : COLOR;
};
 
struct PS_OUTPUT 
{
	float4 Texcolor : COLOR;
};
 

PS_OUTPUT ps_main( PS_INPUT Input )
{
	//PS_OUTPUT OUT;
	PS_OUTPUT OUT = (PS_OUTPUT)0;
    //vector b = tex2D(Samp0,      Input.uv);
	//float4 texColor = tex2D(baseMap,Input.uv);
	OUT.Texcolor = Input.Texcolor;
	OUT.Texcolor.a = player_alpha;
	if(player_color != 0)
	{
	    float color_out = (OUT.Texcolor.x+OUT.Texcolor.y+OUT.Texcolor.z)/player_color;
	    OUT.Texcolor.x = color_out; 
	    OUT.Texcolor.y = color_out; 
	    OUT.Texcolor.z = color_out; 
	}
    return OUT;
}