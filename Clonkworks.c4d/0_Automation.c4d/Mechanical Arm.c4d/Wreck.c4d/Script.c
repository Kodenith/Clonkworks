/*-- Neues Script --*/

#strict 2
#include BAS2
local Claw;
local PointerList;
local SelectedPointer;
local Rope;

func Incineration(){
	if(Claw) RemoveObject(Claw);
	if(PointerList) for(var i in PointerList) RemoveObject(i);
	return(_inherited());
}
