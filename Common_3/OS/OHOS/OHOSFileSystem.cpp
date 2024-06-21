#include "../Interfaces/IFileSystem.h"
#include "../Interfaces/ILog.h"
#include "../Interfaces/IOperatingSystem.h"
#include <errno.h>
#include <sys/stat.h>
#include <rawfile/raw_file_manager.h>
#include "../Interfaces/IMemory.h"
#include "../../ThirdParty/OpenSource/EASTL/string.h"

bool UnixOpenFile(ResourceDirectory resourceDir, const char *fileName, FileMode mode, FileStream *pOut);

static size_t AssetStreamRead(FileStream *pFile, void *outputBuffer, size_t bufferSizeInBytes)
{
	return OH_ResourceManager_ReadRawFile(static_cast<RawFile *>(pFile->rawFile), outputBuffer, bufferSizeInBytes);
}

static size_t AssetStreamWrite(FileStream *pFile, const void *sourceBuffer, size_t byteCount)
{
	LOGF(LogLevel::eERROR, "Bundled HarmonyOS assets are not writable.");
	return 0;
}

static bool AssetStreamSeek(FileStream *pFile, SeekBaseOffset baseOffset, ssize_t seekOffset)
{
	int origin = SEEK_SET;
	switch (baseOffset)
	{
	case SBO_START_OF_FILE:
		origin = SEEK_SET;
		break;
	case SBO_CURRENT_POSITION:
		origin = SEEK_CUR;
		break;
	case SBO_END_OF_FILE:
		origin = SEEK_END;
		break;
	}

	return OH_ResourceManager_SeekRawFile(static_cast<RawFile *>(pFile->rawFile), seekOffset, origin) != -1;
}

static ssize_t AssetStreamGetSeekPosition(const FileStream *pFile)
{
	return OH_ResourceManager_SeekRawFile(static_cast<RawFile *>(pFile->rawFile), 0, SEEK_CUR);
}

static ssize_t AssetStreamGetSize(const FileStream *pFile)
{
	return OH_ResourceManager_GetRawFileSize(static_cast<RawFile *>(pFile->rawFile));
}

static bool AssetStreamFlush(FileStream *pFile)
{
	return true;
}

static bool AssetStreamIsAtEnd(const FileStream *pFile)
{
	return OH_ResourceManager_GetRawFileRemainingLength(static_cast<RawFile *>(pFile->rawFile)) == 0;
}

static bool AssetStreamClose(FileStream *pFile)
{

	OH_ResourceManager_CloseRawFile(static_cast<RawFile *>(pFile->rawFile));
	return true;
}

static IFileSystem gBundledFileIO =
	{
		NULL,
		AssetStreamClose,
		AssetStreamRead,
		AssetStreamWrite,
		AssetStreamSeek,
		AssetStreamGetSeekPosition,
		AssetStreamGetSize,
		AssetStreamFlush,
		AssetStreamIsAtEnd};

static bool gInitialized = false;
static const char *gResourceMounts[RM_COUNT];


const char *GetResourceMount(ResourceMount mount)
{
	return gResourceMounts[mount];
}
bool fsIsBundledResourceDir(ResourceDirectory resourceDir);

static NativeResourceManager *pAssetManager = nullptr;
bool initFileSystem(FileSystemInitDesc *pDesc)
{
	if (gInitialized)
	{
		LOGF(LogLevel::eWARNING, "FileSystem already initialized.");
		return true;
	}
	ASSERT(pDesc);
	pSystemFileIO->GetResourceMount = GetResourceMount;

	for (uint32_t i = 0; i < RM_COUNT; ++i)
		gResourceMounts[i] = "";

	struct FilePlatFormData
	{
		const char *ePath;
		const char *iPath;
		void *assetManager;
	}*pPlatformData = (FilePlatFormData *)pDesc->pPlatformData;
	pAssetManager = static_cast<NativeResourceManager*>(pPlatformData->assetManager);
	gResourceMounts[RM_CONTENT] = "\0";
	gResourceMounts[RM_DEBUG] = pPlatformData->ePath;
	gResourceMounts[RM_DOCUMENTS] = pPlatformData->iPath;
	gResourceMounts[RM_SAVE_0] = pPlatformData->ePath;

	// Override Resource mounts
	for (uint32_t i = 0; i < RM_COUNT; ++i)
	{
		if (pDesc->pResourceMounts[i])
			gResourceMounts[i] = pDesc->pResourceMounts[i];
	}

	gInitialized = true;
	return true;
}

void exitFileSystem()
{
	OH_ResourceManager_ReleaseNativeResourceManager(pAssetManager);
	gInitialized = false;
}

bool PlatformOpenFile(ResourceDirectory resourceDir, const char *fileName, FileMode mode, FileStream *pOut)
{
	char filePath[FS_MAX_PATH] = {};
	const char *resourcePath = fsGetResourceDirectory(resourceDir);
	fsAppendPathComponent(resourcePath, fileName, filePath);

	if (fsIsBundledResourceDir(resourceDir))
	{
		if ((mode & (FM_WRITE | FM_APPEND)) != 0)
		{
			LOGF(LogLevel::eERROR, "Cannot open %s with mode %i: the bundle is read-only.",
				 filePath, mode);
			return false;
		}

		RawFile *rawFile = OH_ResourceManager_OpenRawFile(pAssetManager, filePath);
		if (rawFile == nullptr)
		{
			LOGF(LogLevel::eERROR, "OH_ResourceManager_OpenRawFile failed");
			return false;
		}
		
		long len = OH_ResourceManager_GetRawFileSize(rawFile);

		*pOut = {};
		pOut->rawFile = rawFile;
		pOut->mMode = mode;
		pOut->pIO = &gBundledFileIO;
		pOut->mSize = OH_ResourceManager_GetRawFileSize(rawFile);
		return true;
	}

	return UnixOpenFile(resourceDir, fileName, mode, pOut);
}
