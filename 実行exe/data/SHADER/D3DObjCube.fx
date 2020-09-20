matrix ViewProjMatrix;
matrix matView;
texture g_txScene;
float Transparency;
float4 fvLightPosition;
float4 fvDiffuse;
float4 fvAmbient;
float fvAmbientCoefficient;

void VertScene( vector Pos : POSITION,
                float2 Tex : TEXCOORD0,
				float3 Normal :   NORMAL0,
                out vector oPos : POSITION,
                out float2 oTex : TEXCOORD0,
				out float3 LightDirection:  TEXCOORD2,
                out float3 oNormal : TEXCOORD3
				)
{
    oPos = mul( Pos, ViewProjMatrix );
    oTex = Tex;

   //float3 fvObjectPosition = mul( Pos, matView );
   float3 fvObjectPosition = mul( Pos, ViewProjMatrix );
   
   LightDirection    = float3(fvLightPosition.x,fvLightPosition.y,fvLightPosition.z) - fvObjectPosition;
   oNormal           = mul( Normal, ViewProjMatrix );
  
}

sampler  g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = PYRAMIDALQUAD;
};


float4 PixScene( float2 Tex : TEXCOORD0, 
   float3 LightDirection:   TEXCOORD2,
   float3 oNormal :         TEXCOORD3 ) : COLOR0
{   
   float3 fvLightDirection = normalize( LightDirection );
   float3 fvNormal         = normalize( oNormal );
   //float  fNDotL           = saturate(dot( fvNormal, fvLightDirection )); 

   float4 Texcolor = (float4)0;
   Texcolor = tex2D(g_samScene, Tex);
   
   
   float4 fvTotalAmbient   = fvAmbient * Texcolor; 
   float3 fvTotalDiffuse = fvDiffuse * max(0, dot(fvNormal, fvLightDirection)) * Texcolor;
   //float3 fvTotalDiffuse = fvDiffuse * Texcolor;
   //float4 fvTotalDiffuse   = fvDiffuse * fNDotL * Texcolor;
   

   float3 result = float3(fvTotalAmbient.x,fvTotalAmbient.y,fvTotalAmbient.z) + fvTotalDiffuse;
   
   return float4(result.x * fvAmbientCoefficient,result.y * fvAmbientCoefficient,result.z * fvAmbientCoefficient, Transparency);
}

technique TextureTech1
{
	pass P0
	{
		VertexShader = compile vs_3_0 VertScene();
		PixelShader =  compile ps_3_0 PixScene();
		AlphaBlendEnable= true; //设置渲染状态
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha; 
	}
}