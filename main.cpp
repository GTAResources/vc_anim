#include "vc_anim.h"

HMODULE dllModule, hDummyHandle;

WRAPPER void RwFreeAlign(void*) { EAXJMP(0x5805C0); }
WRAPPER void gtadelete(void*) { EAXJMP(0x6428B0); }
WRAPPER void *gta_nw(int) { EAXJMP(0x6403B0); }

WRAPPER RpClump *RpClumpForAllAtomics(RpClump*, RpAtomicCallBack, void*) { EAXJMP(0x640D00); }
WRAPPER RwBool RpClumpDestroy(RpClump*) { EAXJMP(0x641430); }

WRAPPER void *GetModelFromName(char *name) { EAXJMP(0x4014D0); }
WRAPPER int IsClumpSkinned(RpClump*) { EAXJMP(0x57F580); }

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
	return 0;
}

int
lcstrcmp(const char *s1, const char *s2)
{
	int c1, c2;
	while(*s1){
		c1 = *s1 == -1 ? -1 : charset[*s1];
		c2 = *s2 == -1 ? -1 : charset[*s2];
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
		s1++;
		s2++;
	}
	return 0;
}

void
patch10(void)
{
	if(sizeof(CAnimBlendSequence) != 0x30 ||
	   sizeof(CAnimBlendHierarchy) != 0x28 ||
	   sizeof(CAnimBlock) != 0x20 ||
	   sizeof(CAnimBlendNode) != 0x1C ||
	   sizeof(CAnimBlendAssociation) != 0x3C ||
	   sizeof(CAnimBlendAssocGroup) != 0x14 ||
	   sizeof(AnimBlendFrameData) != 0x18 ||
	   sizeof(CAnimBlendClumpData) != 0x14){
		printf("SIZE MISMATCH\\n");
		return;
	}

	// fucking hell
	MemoryVP::InjectHook(0x405990, CAnimManager::GetAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4059E0, CAnimManager::GetAnimationBlockIndex, PATCH_JUMP);
	MemoryVP::InjectHook(0x405A50, CAnimManager::GetAnimationBlock, PATCH_JUMP);

	MemoryVP::InjectHook(0x401010, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401050, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401180, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401190, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401270, static_cast<void(CAnimBlendAssocGroup::*)(void)>(&CAnimBlendAssocGroup::DestroyAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x4012A0, static_cast<void(CAnimBlendAssocGroup::*)(const char *name, RpClump *clump, char **names, int numAnims)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x40148B, static_cast<void(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401640, static_cast<void(CAnimBlendAssocGroup::*)(void)>(&CAnimBlendAssocGroup::DestroyAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401670, static_cast<void(CAnimBlendAssocGroup::*)(void)>(&CAnimBlendAssocGroup::ctor), PATCH_JUMP);


	MemoryVP::InjectHook(0x401A00, static_cast<void(CAnimBlendAssociation::*)(void)>(&CAnimBlendAssociation::dtor), PATCH_JUMP);
	MemoryVP::InjectHook(0x401B10, static_cast<void(CAnimBlendAssociation::*)(char)>(&CAnimBlendAssociation::dtor2), PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

/*		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr); */

		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&DllMain, &hDummyHandle);

		if (*(DWORD*)0x667BF5 == 0xB85548EC)	// 1.0
			patch10();
		else
			return FALSE;
	}

	return TRUE;
}