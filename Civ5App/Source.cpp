#include "pch.h"
#include "Proc.h"
#include "mem.h"

int main()
{
	// Get Process ID of target process

	DWORD ProcID = GetProcID(L"CivilizationV_DX11.exe");

	// GetModuleBaseAddress

	uintptr_t modBaseAddr = GetModuleBaseAddress(ProcID, L"CvGameCore_Expansion2.dll");

	// Get Handle to Process
	HANDLE hProc = 0;
	hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcID);

	// Resolve Base address of pointer chain

	uintptr_t MainBaseAddr = modBaseAddr + 0x3DE9DC;
	uintptr_t TechBaseAddr = modBaseAddr + 0x3DFFB0;
	uintptr_t UnitBaseAddr = modBaseAddr + 0x3BD290;
	uintptr_t AttackAddr = modBaseAddr + 0x222C35;


	// set gold, faith, culture, happiness en loop

	std::vector<unsigned int> unitMoveOffsets = { 0x2C, 0xCC };
	std::vector<unsigned int> resourceOffsets = { 0xC30, 0 };
	std::vector<unsigned int> goldOffsets = { 0xF528, 0x8 };
	std::vector<unsigned int> faithOffsets = { 0xDC };
	std::vector<unsigned int> posHappinessOffsets = { 0xE8 };
	std::vector<unsigned int> cultureOffsets = { 0xA4 };
	std::vector<unsigned int> techUnlockOffsets = { 0xBA0, 0x4, 0x0 };
	std::vector<unsigned int> techProgressOffsets = { 0xBA0, 0xC, 0x0 };


	uintptr_t unitMovePtr = FindDMAAddy(hProc, UnitBaseAddr, unitMoveOffsets);
	uintptr_t resourcePtr = FindDMAAddy(hProc, MainBaseAddr, resourceOffsets);
	uintptr_t goldPtr = FindDMAAddy(hProc, MainBaseAddr, goldOffsets);
	uintptr_t faithPtr = FindDMAAddy(hProc, MainBaseAddr, faithOffsets);
	uintptr_t posHappinessPtr = FindDMAAddy(hProc, MainBaseAddr, posHappinessOffsets);
	uintptr_t culturePtr = FindDMAAddy(hProc, MainBaseAddr, cultureOffsets);
	uintptr_t techUnlockPtr = FindDMAAddy(hProc, TechBaseAddr, techUnlockOffsets);
	uintptr_t techProgressPtr = FindDMAAddy(hProc, TechBaseAddr, techProgressOffsets);

	std::cout << "unitMovePtr : " << std::hex << unitMovePtr << std::endl;

	int Value = 13370000;
	int happiness = 6969;
	char unlock = 1;
	int resourceValue = 1337;
	int unitMoveValue = 1440;
	int techProgressValue = 25000;

	//std::cout << "test : " << std::hex << (BYTE*)(unitMovePtr) << std::endl; Need to ask about this.

	bool bGold = false, bFaith = false, bCulture = false, bHappiness = false, bUnitMoves = false, bAttack = false;

	std::cout << "[+] F4 for Unlimited Unit Moves\n";
	std::cout << "[+] F5 for unlimited Resources\n";
	std::cout << "[+] F6 to unlock all Tech\n";
	std::cout << "[+] F7 for unlimited Gold\n";
	std::cout << "[+] F8 for unlimited Faith\n";
	std::cout << "[+] F9 for unlimited Culture\n";
	std::cout << "[+] F10 for unlimited Happiness\n";
	std::cout << "[+] END to exit\n\n";
	std::cout << "===============================\n\n";

	while (1)
	{

		if (GetAsyncKeyState(VK_F4) & 1)
		{
			bUnitMoves = !bUnitMoves;
			if (bUnitMoves)
				std::cout << "[+] Unlimited Unit Moves Enabled\n";
			else
				std::cout << "[+] Unlimited Unit Moves Disabled\n";
		}
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			std::cout << "[+] Unlimited Resources Enabled\n";
			for (int i = 0; i < 6; i++)
			{
				WriteProcessMemory(hProc, (BYTE*)(resourcePtr + 4 * i), &resourceValue, sizeof(int), NULL);
			}
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			for (int i = 1; i < 80; i++)
			{
				uintptr_t unlockAddr = techUnlockPtr + i;
				WriteProcessMemory(hProc, (BYTE*)unlockAddr, &unlock, sizeof(char), NULL);
				//WriteProcessMemory(hProc, (BYTE*)techProgressPtr, &techProgressValue, sizeof(int), NULL);
			}
			std::cout << "[+] All Tech has been unlocked.\n";
		}

		if (GetAsyncKeyState(VK_F7) & 1)
		{
			bGold = !bGold;
			if (bGold)
				std::cout << "[+] Unlimited Gold Enabled.\n";
			else
				std::cout << "[+] Unlimited Gold Disabled.\n";
		}

		if (GetAsyncKeyState(VK_F8) & 1)
		{
			bFaith = !bFaith;
			if (bFaith)
				std::cout << "[+] Unlimited Faith Enabled.\n";
			else
				std::cout << "[+] Unlimited Faith Disabled.\n";
		}

		if (GetAsyncKeyState(VK_F9) & 1)
		{
			bCulture = !bCulture;

			if (bCulture)
				std::cout << "[+] Unlimited Culture Enabled.\n";
			else
				std::cout << "[+] Unlimited Culture Disabled.\n";
		}

		if (GetAsyncKeyState(VK_F10) & 1)
		{
			bHappiness = !bHappiness;
			if (bHappiness)
				std::cout << "[+] Unlimited Happiness Enabled.\n";
			else
				std::cout << "[+] Unlimited Happiness Disabled.\n";
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			bAttack = !bAttack;

			if (bAttack)
			{
				std::cout << "Unlimited Attack Enabled.\n";
				// Nop
				mem::NopEx(hProc, (BYTE*)AttackAddr, 3);
			}
			else
			{
				std::cout << "Unlimited Attack Disabled.\n";
				// Patch with original
				mem::PatchEx(hProc, (BYTE*)AttackAddr, (BYTE*)"\x89\x7E\x04", 3);
			}
		}

		if (GetAsyncKeyState(VK_END) & 1)
			break;

		if (bUnitMoves)
		{
			unitMovePtr = FindDMAAddy(hProc, UnitBaseAddr, unitMoveOffsets);
			WriteProcessMemory(hProc, (BYTE*)unitMovePtr, &unitMoveValue, sizeof(int), NULL);
		}
		if (bGold)
			WriteProcessMemory(hProc, (BYTE*)goldPtr, &Value, sizeof(int), NULL);
		if(bFaith)
			WriteProcessMemory(hProc, (int*)faithPtr, &Value, sizeof(int), NULL);
		if(bCulture)
			WriteProcessMemory(hProc, (int*)culturePtr, &Value, sizeof(int), NULL);
		if(bHappiness)
			WriteProcessMemory(hProc, (int*)posHappinessPtr, &happiness, sizeof(int), NULL);

	}

	CloseHandle(hProc);

	std::cout << "Shutting down\n";
}