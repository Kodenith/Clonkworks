/*-- Neues Script --*/

#strict 2
#include BAS2
local Sect

func Incineration(){
	if(Sect && !OnFire(Sect)) Incinerate(Sect);
}