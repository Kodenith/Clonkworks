/*-- Neues Script --*/

#strict 2
#include QUS0
local Sect;

func DestroyMe(){
			var sect;
			sect = CreateObject(QUA4,0,-20);
			sect->Incinerate();
			SetR(90,sect);
			sect = CreateObject(QUA4,0,0);
			sect->Incinerate();
			SetR(90,sect);
			sect = CreateObject(QUA4,0,20);
			sect->Incinerate();
			SetR(90,sect);
		RemoveObject();
}