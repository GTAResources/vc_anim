#include "vc_anim.h"

HMODULE dllModule, hDummyHandle;

void **&RwEngineInst = *(void***)0x7870C0;

WRAPPER void *RwMallocAlign(uint, int) { EAXJMP(0x5805D0); }
WRAPPER void RwFreeAlign(void*) { EAXJMP(0x5805C0); }
WRAPPER void gtadelete(void*) { EAXJMP(0x6428B0); }
WRAPPER void *gta_nw(int) { EAXJMP(0x6403B0); }

WRAPPER RwMatrix *RwMatrixUpdate(RwMatrix * matrix) { EAXJMP(0x6437B0); }
WRAPPER RwMatrix *RwMatrixInvert(RwMatrix*, const RwMatrix*) { EAXJMP(0x643F40); }
WRAPPER RwMatrix *RwMatrixMultiply(RwMatrix*, const RwMatrix*, const RwMatrix*) { EAXJMP(0x6437C0); }
WRAPPER RwStream *RwStreamOpen(RwStreamType, RwStreamAccessType, const void *) { EAXJMP(0x6459C0); }
WRAPPER RwBool RwStreamClose(RwStream*, void*) { EAXJMP(0x6458F0); }
WRAPPER RwUInt32 RwStreamRead(RwStream*, void*, RwUInt32) { EAXJMP(0x6454B0); }
WRAPPER RwFrame *RwFrameForAllChildren(RwFrame*, RwFrameCallBack, void*) { EAXJMP(0x645060); }
WRAPPER RwFrame *RwFrameUpdateObjects(RwFrame*) { EAXJMP(0x644D00); }
WRAPPER RpClump *RpClumpForAllAtomics(RpClump*, RpAtomicCallBack, void*) { EAXJMP(0x640D00); }
WRAPPER RwBool RpClumpDestroy(RpClump*) { EAXJMP(0x641430); }
WRAPPER RwInt32 RpHAnimIDGetIndex(RpHAnimHierarchy*, RwInt32) { EAXJMP(0x646390); }
WRAPPER RpSkin *RpSkinGeometryGetSkin(RpGeometry*) { EAXJMP(0x649960); }
WRAPPER const RwMatrix *RpSkinGetSkinToBoneMatrices(RpSkin*) { EAXJMP(0x6499E0); }
WRAPPER RwUInt32 RpSkinGetNumBones(RpSkin*) { EAXJMP(0x6499C0); }
WRAPPER RwV3d *RwV3dTransformPoints(RwV3d*, const RwV3d*, RwInt32, const RwMatrix*) { EAXJMP(0x647160); }

WRAPPER void *GetModelFromName(char *name) { EAXJMP(0x4014D0); }
WRAPPER void CQuaternion::Slerp(CQuaternion&, CQuaternion&, float, float, float) { EAXJMP(0x4DFBE0); }
WRAPPER void CQuaternion::Get(RwMatrix *mat) { EAXJMP(0x4DFD30); }
WRAPPER int IsClumpSkinned(RpClump*) { EAXJMP(0x57F580); }
WRAPPER RpAtomic *AtomicRemoveAnimFromSkinCB(RpAtomic*, void*) { EAXJMP(0x489750); }

void **CModelInfo__ms_modelInfoPtrs = (void**)0x92D4C8;

static char *charclasses = (char*)0x6DCCF8;
static char *charset = (char*)0x6DCDF8;

int
gtastrcmp(const char *s1, const char *s2)
{
	char c1, c2;
	while(*s1){
		c1 = charclasses[*s1] & 0x40 ? *s1++ - 0x20 : *s1++;
		c2 = charclasses[*s2] & 0x40 ? *s2++ - 0x20 : *s2++;
		if(c1 != c2)
			return 1;
	}
	return *s2 != '\0';
}

int
lcstrcmp(const char *s1, const char *s2)
{
	int c1, c2;
	do{
		c1 = *s1 == -1 ? -1 : charset[*s1];
		c2 = *s2 == -1 ? -1 : charset[*s2];
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
		s1++;
		s2++;
	}while(c1);
	return 0;
}

static_assert(sizeof(CAnimBlendSequence) == 0x30, "CAnimBlendSequence: error");
static_assert(sizeof(CAnimBlendHierarchy) == 0x28, "CAnimBlendSequence: error");
static_assert(sizeof(CAnimBlock) == 0x20, "CAnimBlendSequence: error");
static_assert(sizeof(CAnimBlendNode) == 0x1C, "CAnimBlendSequence: error");
static_assert(sizeof(CAnimBlendAssociation) == 0x3C, "CAnimBlendSequence: error");
static_assert(sizeof(CAnimBlendAssocGroup) == 0x14, "CAnimBlendSequence: error");
static_assert(sizeof(AnimBlendFrameData) == 0x18, "CAnimBlendSequence: error");
//static_assert(sizeof(CAnimBlendClumpData) == 0x14, "CAnimBlendSequence: error");
static_assert(sizeof(RFrame) == 0x14, "CAnimBlendSequence: error");
static_assert(sizeof(RTFrame) == 0x20, "CAnimBlendSequence: error");

void
patch10(void)
{
	// fucking hell
	MemoryVP::InjectHook(0x401010, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401050, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401180, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401190, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401270, static_cast<void(CAnimBlendAssocGroup::*)(void)>(&CAnimBlendAssocGroup::DestroyAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x4012A0, static_cast<void(CAnimBlendAssocGroup::*)(const char *name, RpClump *clump, char **names, int numAnims)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401380, static_cast<void(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401640, (&CAnimBlendAssocGroup::DestroyAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401670, (&CAnimBlendAssocGroup::ctor), PATCH_JUMP);

	MemoryVP::InjectHook(0x4016A0, (&CAnimBlendAssociation::SetFinishCallback), PATCH_JUMP);
	MemoryVP::InjectHook(0x4016C0, (&CAnimBlendAssociation::SetDeleteCallback), PATCH_JUMP);
	MemoryVP::InjectHook(0x4016E0, (&CAnimBlendAssociation::SetBlend), PATCH_JUMP);
	MemoryVP::InjectHook(0x401700, (&CAnimBlendAssociation::Start), PATCH_JUMP);
	MemoryVP::InjectHook(0x401720, (&CAnimBlendAssociation::GetNode), PATCH_JUMP);
	MemoryVP::InjectHook(0x401740, (&CAnimBlendAssociation::SyncAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401770, (&CAnimBlendAssociation::SetCurrentTime), PATCH_JUMP);
	MemoryVP::InjectHook(0x401820, static_cast<void(CAnimBlendAssociation::*)(CAnimBlendAssociation&)>(&CAnimBlendAssociation::Init), PATCH_JUMP);
	MemoryVP::InjectHook(0x4018F0, static_cast<void(CAnimBlendAssociation::*)(RpClump *clump, CAnimBlendHierarchy *anim)>(&CAnimBlendAssociation::Init), PATCH_JUMP);
	MemoryVP::InjectHook(0x401A00, (&CAnimBlendAssociation::dtor), PATCH_JUMP);
	// no need to hook the copy constructor, it's only called from our code. too complicated anyway
	MemoryVP::InjectHook(0x401AB0, (&CAnimBlendAssociation::ctor), PATCH_JUMP);
	MemoryVP::InjectHook(0x401B10, (&CAnimBlendAssociation::dtor2), PATCH_JUMP);
	MemoryVP::InjectHook(0x402C90, (&CAnimBlendAssociation::UpdateBlend), PATCH_JUMP);
	MemoryVP::InjectHook(0x402D60, (&CAnimBlendAssociation::UpdateTime), PATCH_JUMP);

	MemoryVP::InjectHook(0x401B90, (&CAnimBlendClumpData::ForAllFrames), PATCH_JUMP);
	MemoryVP::InjectHook(0x401BC0, (&CAnimBlendClumpData::SetNumberOfBones), PATCH_JUMP);
	MemoryVP::InjectHook(0x401C00, (&CAnimBlendClumpData::dtor), PATCH_JUMP);
	MemoryVP::InjectHook(0x401C40, (&CAnimBlendClumpData::ctor), PATCH_JUMP);

	MemoryVP::InjectHook(0x401C70, (&CAnimBlendHierarchy::RemoveUncompressedData), PATCH_JUMP);
	MemoryVP::InjectHook(0x401C80, (&CAnimBlendHierarchy::Uncompress), PATCH_JUMP);
	MemoryVP::InjectHook(0x401CD0, (&CAnimBlendHierarchy::RemoveQuaternionFlips), PATCH_JUMP);
	MemoryVP::InjectHook(0x401D00, (&CAnimBlendHierarchy::CalcTotalTimeCompressed), PATCH_JUMP);
	MemoryVP::InjectHook(0x401DF0, (&CAnimBlendHierarchy::CalcTotalTime), PATCH_JUMP);
	MemoryVP::InjectHook(0x401EE0, (&CAnimBlendHierarchy::SetName), PATCH_JUMP);
	MemoryVP::InjectHook(0x401F00, (&CAnimBlendHierarchy::Shutdown), PATCH_JUMP);
	MemoryVP::InjectHook(0x401F40, (&CAnimBlendHierarchy::dtor), PATCH_JUMP);
	// ctor is called before we're attached (static init)

	MemoryVP::InjectHook(0x401FB0, (&CAnimBlendNode::CalcDeltasCompressed), PATCH_JUMP);
	MemoryVP::InjectHook(0x4021C0, (&CAnimBlendNode::SetupKeyFrameCompressed), PATCH_JUMP);
	MemoryVP::InjectHook(0x402240, (&CAnimBlendNode::FindKeyFrame), PATCH_JUMP);
	MemoryVP::InjectHook(0x402360, (&CAnimBlendNode::GetEndTranslation), PATCH_JUMP);
	MemoryVP::InjectHook(0x402400, (&CAnimBlendNode::GetCurrentTranslation), PATCH_JUMP);
	MemoryVP::InjectHook(0x402550, (&CAnimBlendNode::CalcDeltas), PATCH_JUMP);
	MemoryVP::InjectHook(0x4026C0, (&CAnimBlendNode::NextKeyFrame), PATCH_JUMP);
	MemoryVP::InjectHook(0x402770, (&CAnimBlendNode::Update), PATCH_JUMP);
	MemoryVP::InjectHook(0x402A00, (&CAnimBlendNode::Init), PATCH_JUMP);

	MemoryVP::InjectHook(0x402A20, (&CAnimBlendSequence::RemoveQuaternionFlips), PATCH_JUMP);
	MemoryVP::InjectHook(0x402AF0, (&CAnimBlendSequence::SetNumFrames), PATCH_JUMP);
	MemoryVP::InjectHook(0x402B80, (&CAnimBlendSequence::SetBoneTag), PATCH_JUMP);
	MemoryVP::InjectHook(0x402B90, (&CAnimBlendSequence::SetName), PATCH_JUMP);
	MemoryVP::InjectHook(0x402BB0, (&CAnimBlendSequence::dtor), PATCH_JUMP);
	MemoryVP::InjectHook(0x402BF0, (&CAnimBlendSequence::ctor), PATCH_JUMP);
	MemoryVP::InjectHook(0x402C20, (&CAnimBlendSequence::dtor2), PATCH_JUMP);

	MemoryVP::InjectHook(0x404930, CAnimManager::GetNumRefsToAnimBlock, PATCH_JUMP);
	MemoryVP::InjectHook(0x404940, CAnimManager::RemoveAnimBlockRefWithoutDelete, PATCH_JUMP);
	MemoryVP::InjectHook(0x404950, CAnimManager::RemoveAnimBlockRef, PATCH_JUMP);
	MemoryVP::InjectHook(0x404980, CAnimManager::AddAnimBlockRef, PATCH_JUMP);
	MemoryVP::InjectHook(0x404990, CAnimManager::RemoveAnimBlock, PATCH_JUMP);
	MemoryVP::InjectHook(0x404A00, CAnimManager::RemoveLastAnimFile, PATCH_JUMP);
	MemoryVP::InjectHook(0x404A50, CAnimManager::LoadAnimFile, PATCH_JUMP);
	MemoryVP::InjectHook(0x4053A0, CAnimManager::RegisterAnimBlock, PATCH_JUMP);
	MemoryVP::InjectHook(0x405430, CAnimManager::CreateAnimAssocGroups, PATCH_JUMP);
	MemoryVP::InjectHook(0x4055E0, CAnimManager::LoadAnimFiles, PATCH_JUMP);
	MemoryVP::InjectHook(0x405640, CAnimManager::BlendAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4058B0, CAnimManager::AddAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x405940, (CAnimBlendAssociation *(*)(int, const char*))CAnimManager::GetAnimAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x405960, (CAnimBlendAssociation *(*)(int, int))CAnimManager::GetAnimAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x405980, CAnimManager::GetAnimGroupName, PATCH_JUMP);
	MemoryVP::InjectHook(0x405990, CAnimManager::GetAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4059E0, CAnimManager::GetAnimationBlockIndex, PATCH_JUMP);
	MemoryVP::InjectHook(0x405A50, CAnimManager::GetAnimationBlock, PATCH_JUMP);
	MemoryVP::InjectHook(0x405AA0, CAnimManager::RemoveFromUncompressedCache, PATCH_JUMP);
	MemoryVP::InjectHook(0x405AC0, CAnimManager::UncompressAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x405B80, CAnimManager::Shutdown, PATCH_JUMP);
	MemoryVP::InjectHook(0x405BF0, CAnimManager::Initialise, PATCH_JUMP);

	MemoryVP::InjectHook(0x402E20, (CAnimBlendAssociation*(*)(RpClump*))RpAnimBlendClumpGetFirstAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x404600, RpAnimBlendClumpCheckKeyFrames, PATCH_JUMP);
	MemoryVP::InjectHook(0x404690, RpAnimBlendClumpUpdateAnimations, PATCH_JUMP);
	MemoryVP::InjectHook(0x407520, RpAnimBlendAllocateData, PATCH_JUMP);
	MemoryVP::InjectHook(0x407560, RpAnimBlendClumpDestroy, PATCH_JUMP);
	MemoryVP::InjectHook(0x4075A0, (CAnimBlendAssociation*(*)(CAnimBlendAssociation*,uint))RpAnimBlendGetNextAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4075D0, (CAnimBlendAssociation*(*)(CAnimBlendAssociation*))RpAnimBlendGetNextAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4075F0, (CAnimBlendAssociation*(*)(RpClump*, uint))RpAnimBlendClumpGetFirstAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x407620, RpAnimBlendClumpGetMainPartialAssociation_N, PATCH_JUMP);
	MemoryVP::InjectHook(0x407660, RpAnimBlendClumpGetMainAssociation_N, PATCH_JUMP);
	MemoryVP::InjectHook(0x4076A0, RpAnimBlendClumpGetMainPartialAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4076F0, RpAnimBlendClumpGetMainAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x407780, RpAnimBlendClumpGetAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4077B0, RpAnimBlendClumpRemoveAssociations, PATCH_JUMP);
	MemoryVP::InjectHook(0x407800, RpAnimBlendClumpRemoveAllAssociations, PATCH_JUMP);
	MemoryVP::InjectHook(0x407830, RpAnimBlendClumpSetBlendDeltas, PATCH_JUMP);
	MemoryVP::InjectHook(0x407870, RpAnimBlendClumpIsInitialized, PATCH_JUMP);
	MemoryVP::InjectHook(0x407890, RpAnimBlendClumpInit, PATCH_JUMP);
	MemoryVP::InjectHook(0x4079A0, RpAnimBlendClumpInitSkinned, PATCH_JUMP);
	MemoryVP::InjectHook(0x407AC0, RpAnimBlendClumpFillFrameArray, PATCH_JUMP);
	MemoryVP::InjectHook(0x407B60, RpAnimBlendClumpFindBone, PATCH_JUMP);
	MemoryVP::InjectHook(0x407BB0, RpAnimBlendClumpFindFrame, PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

/*		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);*/

		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&DllMain, &hDummyHandle);

		if (*(DWORD*)0x667BF5 == 0xB85548EC)	// 1.0
			patch10();
		else
			return FALSE;
	}

	return TRUE;
}
