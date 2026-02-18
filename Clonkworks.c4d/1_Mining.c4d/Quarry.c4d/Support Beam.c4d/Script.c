/*-- Neues Script --*/

#strict 2
#include BAS2
local Sect;

func Incineration(){
	if(Sect && !OnFire(Sect)) Incinerate(Sect);
	DestroyMe();
}

func DestroyMe(){
		for(var y = -20; y < 60; y+= 20){
			var sect;
			sect = CreateObject(QUA4,0,y);
			sect->Incinerate();
			SetR(90,sect);
		}
		RemoveObject();
}

func SupportSize(){ return(GetDefHeight(GetID())); }

func Initialize(){
	SetCategory(1);
	//make sure nothing is in the way, otherwise remove.
	if(FindObject2(Find_Category(C4D_Structure),Find_NoContainer(),Find_InRect(GetDefOffset(GetID()),GetDefOffset(GetID(),1),GetDefWidth(GetID()),GetDefHeight(GetID())))) RemoveObject();
}