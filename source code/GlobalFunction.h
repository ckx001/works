//=============================================================================
//
// グローバル関数 [GlobalFunction.h]
// Author :  GP-12B-191 22 程凱旋
//
//=============================================================================
#pragma once
//#include "D3DObjMesh.h"
#include<string>

///////////////GAME SWITCH/////////////////////
//触发器开关 是否显示触发器框架
//#define COLLIDER_SWITCH
//开始界面开关 是否显示开始界面
#define MENU_SWITCH
//Player Die开关 是否触发Player Die
#define PLAYER_DIE_SWITCH
//摄像头转动开关 是否触发摄像头可调节
//#define CAMERA_ROT_ON_SWITCH


////////////////////////////////////////////////

#define M_PI       3.14159
#define RAD2DEG(x) ((x)*180./M_PI)  //弧度转角度 
#define DEG2RAD(x) ((x)*M_PI/180.)  //角度转弧度

#define  FILE_TYPE_BIN_PATH0 ".\\bin\\"
#define  FILE_TYPE_BIN_PATH1 "\\bin\\"

#define  FILE_TYPE_MODEL_PATH0 ".\\data\\"
#define  FILE_TYPE_MODEL_PATH1 "\\data\\MODEL\\"
#define  FILE_TYPE_MODEL_PATH2 "data\\MODEL\\"

#define CURRENT_MODEL_PATH "\\data\\MODEL\\*.x"
#define CURRENT_MODEL_PATH_CHECK "data\\MODEL\\"

#define CURRENT_OBJMODEL_PATH "\\data\\MODEL\\OBJ\\*.obj"
#define CURRENT_OBJMODEL_PATH_CHECK "data\\MODEL\\OBJ\\"
#define CURRENT_OBJMODEL_PATH_TEXTURE "data/MODEL/OBJ/"

#define MAX_UI_OBJ_EX_NUM 32768

#define OBJ_GROUP_TYPE 12005 
#define OBJ_CUBE_MAX  32768

#define GRID_SIZE (40.0f) 

using namespace std;

//全局obj索引
enum GLOBAL_OBJ_INDEX
{
	OBJ_MODEL_TYPE,
	OBJ_CUBE_TYPE,
	OBJ_OBJMODEL_TYPE,
	OBJ_INDEX_TYPE_NUM = 3  //索引的总数
};

//全局环境物体类型
enum GLOBAL_EN_TYPE_INDEX
{
	OBJ_EN_PLAYER_TYPE,
	OBJ_EN_TERRAIN_TYPE,
	OBJ_EN_ENEMY_WOLF_TYPE,
	OBJ_EN_ENEMY_CANNIBAL_TYPE,
	OBJ_EN_ENEMY_BOSS_WOLF_TYPE,
	OBJ_EN_ENEMY_BULLET_TYPE,
	OBJ_EN_ENEMY_BOSSWOLF_TYPE
};

//全局obj参数
enum GLOBAL_OBJ_PARAMETER
{
	OBJ_PARAMETER_POS_X,
	OBJ_PARAMETER_POS_Y,
	OBJ_PARAMETER_POS_Z,
	OBJ_PARAMETER_ROT_X,
	OBJ_PARAMETER_ROT_Y,
	OBJ_PARAMETER_ROT_Z,
	OBJ_PARAMETER_SCL_X,
	OBJ_PARAMETER_SCL_Y,
	OBJ_PARAMETER_SCL_Z
};

//enemy type
enum ENEMY_TYPE
{
	ENEMY_TYPE_WOLF,
	ENEMY_TYPE_CANNIBAL,
	ENEMY_TYPE_BOSSWOLF
};


//全局OBJ模型
enum GOODS_TYPE
{
	GOODS_TYPE_RED,
	GOODS_TYPE_TREE
};


//全局结构
enum STRUCT_TYPE
{
	STRUCT_TYPE_MENU,  //菜单
    STRUCT_TYPE_GAME   //游戏
};


//存档点
enum SAVE_POINT
{
	SAVE_POINT_HOME,
	SAVE_POINT_GRASS_START,  //Grass Start
	SAVE_POINT_GRASS_BOSS    //Grass boss战门前
};


//打开文件的文件类型
enum GLOBAL_PROJECT_FILE_TYPE
{
	FILE_TYPE_TEXTURE, //图片文件
	FILE_TYPE_BIN     //二进制项目文件
};

//////////////////
//GLOBAL_UI Ui[MAX_UI_OBJ_NUM];
struct GLOBAL_OBJ_PLANT
{
	D3DXVECTOR3		pos;		
	D3DXVECTOR3		rot;		
	D3DXVECTOR3		scl;		
	int index;                  
	bool use;
	D3DXMATRIX		mtxWorld;	
	LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	LPDIRECT3DTEXTURE9		g_pD3DTexture;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引
	TCHAR tag[MAX_PATH + 252];
};


struct GLOBAL_OBJ_CUBE
{
	D3DXVECTOR3		pos;		
	D3DXVECTOR3		rot;		
	D3DXVECTOR3		scl;		
	int index;                  //plant当前的索引
	bool use;
	D3DXMATRIX		mtxWorld;	// 儚乕儖僪儅僩儕僢僋僗
	LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	LPDIRECT3DTEXTURE9		g_pD3DTexture;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引
	TCHAR tag[MAX_PATH + 252];
};

struct GLOBAL_OBJ_MODEL
{
	D3DXVECTOR3			pos;
	D3DXVECTOR3			rot;
	D3DXVECTOR3			scl;

	bool                use;
	bool                active;
	int                 type;
	LPDIRECT3DTEXTURE9	pD3DTexture;
	LPD3DXMESH			pD3DXMesh;
	LPD3DXBUFFER		pD3DXBuffMat;
	DWORD				nNumMat;

	D3DXMATRIX			mtxWorld;

	int ModelPathNameLen;   //文件名长度
	TCHAR ModelPathName[MAX_PATH + 252];
	int ModelPathNameUiIndex; //对于ui texture列表的索引

	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引
	TCHAR tag[MAX_PATH + 252];

};


struct GLOBAL_OBJTEXTURELIST
{
	TCHAR TextureListPathName[100];
	int TextureListPathNameLen = 0;
};

struct GLOBAL_OBJ_OBJMODEL
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3	RelativePos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	bool use;
	LPDIRECT3DTEXTURE9	pD3DTexture;
	LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuff;
	int TexturePathNameLen;   //文件名长度
	TCHAR TexturePathName[MAX_PATH + 252];
	int TexturePathNameUiIndex; //对于ui texture列表的索引
	int Group = -1; //对于ui 所属Group

	int ModelPathNameLen;   //文件名长度
	TCHAR ModelPathName[MAX_PATH + 252];
	int ModelPathNameUiIndex; //对于ui texture列表的索引

	int ObjGroupNum; //obj文件中的组数
	int TrianglesList[100];  //每个组中的三角形数量分别渲染
	int matGroups; //材质的组数
	LPDIRECT3DTEXTURE9	pD3DTextureList[100]; //纹理数组
	GLOBAL_OBJTEXTURELIST TextureListPath[10];
	float Tf[100][3]; //透明度数组
	TCHAR tag[MAX_PATH + 252];
};

struct GLOBAL_UI
{
	bool use;
	//int SelectedIndex = -1; //选中的obj在列表的索引
	int SelectedType = -1; //选中的obj的类型
	int SelectedTypeIdIndex;
};

/////////////////////
const D3DXCOLOR WHITE_COLOR = D3DCOLOR_ARGB(255, 255, 255, 255);
const D3DXCOLOR BLACK_COLOR = D3DCOLOR_ARGB(255, 0, 0, 0);
const D3DXCOLOR RED_COLOR = D3DCOLOR_ARGB(255, 255, 0, 0);
const D3DXCOLOR GREEN_COLOR = D3DCOLOR_ARGB(255, 0, 255, 0);
const D3DXCOLOR BLUE_COLOR = D3DCOLOR_ARGB(255, 0, 0, 255);
const D3DXCOLOR YELLOW_COLOR = D3DCOLOR_ARGB(255, 255, 255, 0);
const D3DXCOLOR CYAN_COLOR = D3DCOLOR_ARGB(255, 0, 255, 255);
const D3DXCOLOR MAGENTA_COLOR = D3DCOLOR_ARGB(255, 255, 0, 255);
const D3DXCOLOR LIGHTGREEN = D3DCOLOR_ARGB(255, 60, 184, 120);
const D3DXCOLOR PUREGREEN = D3DCOLOR_ARGB(255, 0, 166, 81);
const D3DXCOLOR DARKGREEN = D3DCOLOR_ARGB(255, 0, 114, 54);
const D3DXCOLOR BEACH_SAND = D3DCOLOR_ARGB(255, 255, 249, 157);
const D3DXCOLOR DESERT_SAND = D3DCOLOR_ARGB(255, 250, 205, 135);
const D3DXCOLOR LIGHT_YELLOW_GREEN = D3DCOLOR_ARGB(255, 124, 197, 118);
const D3DXCOLOR PURE_YELLOW_GREEN = D3DCOLOR_ARGB(255, 57, 181, 74);
const D3DXCOLOR DARK_YELLOW_GREEN = D3DCOLOR_ARGB(255, 25, 123, 48);
const D3DXCOLOR LIGHT_BROWN = D3DCOLOR_ARGB(255, 198, 156, 109);
const D3DXCOLOR DARK_BROWN = D3DCOLOR_ARGB(255, 115, 100, 87);

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE_COLOR, WHITE_COLOR, WHITE_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 RED_MTRL = InitMtrl(RED_COLOR, RED_COLOR, RED_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN_COLOR, GREEN_COLOR, GREEN_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE_COLOR, BLUE_COLOR, BLUE_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW_COLOR, YELLOW_COLOR, YELLOW_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 CYAN_MTRL = InitMtrl(CYAN_COLOR, CYAN_COLOR, CYAN_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 MAGENTA_MTRL = InitMtrl(MAGENTA_COLOR, MAGENTA_COLOR, MAGENTA_COLOR, BLACK_COLOR, 2.0f);
const D3DMATERIAL9 BLACK_MTRL = InitMtrl(BLACK_COLOR, BLACK_COLOR, BLACK_COLOR, BLACK_COLOR, 0.0f);

void AddObjTypeNumList(int type);
int* RetObjTypeNumList();
void DelObjTypeNumList();
void GetSysPath();
TCHAR* GetProjectPath();
void string2tchar(std::string &src, TCHAR* buf);
bool CubeInFrustum(D3DXVECTOR3 pos);