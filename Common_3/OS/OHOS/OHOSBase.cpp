#include "../Interfaces/IApp.h"
#include <rawfile/raw_file_manager.h>
#include <napi/native_api.h>
namespace
{
	char *malloc_string(napi_env env, napi_value value)
	{
		size_t typeLen = 0;
		napi_get_value_string_utf8(env, value, nullptr, 0, &typeLen);
		auto str = reinterpret_cast<char *>(malloc((typeLen + 1) * sizeof(char)));
		napi_get_value_string_utf8(env, value, str, typeLen + 1, &typeLen);
		return str;
	}
	double GetPropertyByName(napi_env env, napi_value value, const char *name)
	{
		napi_value prop = nullptr;
		napi_get_named_property(env, value, name, &prop);
		double v;
		napi_get_value_double(env, prop, &v);
		return v;
	}

} // namespace

IApp *pApp = nullptr;

char *g_ePath;
char *g_iPath;
NativeResourceManager *g_assetManager;

uint32_t g_widthPixels;
uint32_t g_heightPixels;
float g_density;
uint32_t g_densityDpi;
float g_scaledDensity;
float g_xdpi;
float g_ydpi;


/************************************************************************/
// MARK: - RegisterWindow
/************************************************************************/
extern bool RegisterWindow(napi_env env, napi_value exports);
napi_value CreateSystemCB(napi_env env, napi_callback_info info);
napi_value ReleaseSystemCB(napi_env env, napi_callback_info info);

/************************************************************************/
// MARK: - OHOSMain
/************************************************************************/
int OHOSMain(void *env_, void *exports_, IApp *app)
{
	napi_property_descriptor desc [] = {
		{"CreateSystem", nullptr, CreateSystemCB, nullptr, nullptr, nullptr, napi_default, nullptr},
		{"ReleaseSystem", nullptr, ReleaseSystemCB, nullptr, nullptr, nullptr, napi_default, nullptr},
	};

	auto env = reinterpret_cast<napi_env>(env_);
	auto exports = reinterpret_cast<napi_value>(exports_);
	napi_define_properties(env, exports, sizeof(desc) / sizeof(desc [0]), desc);
	RegisterWindow(env, exports);

	pApp = app;
	return 0;
}


/************************************************************************/
// MARK: - CreateSystem
/************************************************************************/
napi_value CreateSystemCB(napi_env env, napi_callback_info info)
{
	size_t argc = 3;
	napi_value argv [3] = {nullptr};
	napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

	g_ePath = malloc_string(env, argv [1]);
	g_iPath = malloc_string(env, argv [2]);
	g_assetManager = OH_ResourceManager_InitNativeResourceManager(env, argv [0]);

	// DPI
	char path [64] = "@ohos.display";
	size_t typeLen = 0;
	napi_value string;
	napi_create_string_utf8(env, path, typeLen, &string);
	// 加载系统库
	napi_value sysModule;
	napi_load_module(env, path, &sysModule);
	// 获取系统库中的"getDefaultDisplaySync"方法
	napi_value func = nullptr;
	napi_get_named_property(env, sysModule, "getDefaultDisplaySync", &func);
	napi_value funcResult;
	napi_call_function(env, sysModule, func, 0, nullptr, &funcResult);

	g_widthPixels = GetPropertyByName(env, funcResult, "width");
	g_heightPixels = GetPropertyByName(env, funcResult, "height");
	g_density = GetPropertyByName(env, funcResult, "densityPixels");
	g_densityDpi = GetPropertyByName(env, funcResult, "densityDPI");
	g_scaledDensity = GetPropertyByName(env, funcResult, "scaledDensity");
	g_xdpi = GetPropertyByName(env, funcResult, "xDPI");
	g_ydpi = GetPropertyByName(env, funcResult, "yDPI");

	return nullptr;
}

/************************************************************************/
// MARK: - ReleaseSystem
/************************************************************************/
napi_value ReleaseSystemCB(napi_env env, napi_callback_info info)
{

	free(g_ePath);
	free(g_iPath);
	OH_ResourceManager_ReleaseNativeResourceManager(g_assetManager);

	return nullptr;
}
