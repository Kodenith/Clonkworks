/*-- Neues Script --*/

#strict 2
#include QUS0
local Sect;


func DestroyMe(){
		for(var y = -20; y < 80; y+= 20){
			var sect;
			sect = CreateObject(QUA4,0,y);
			sect->Incinerate();
			SetR(90,sect);
		}
		RemoveObject();
}