/*-- Neues Script --*/

#strict
local rider;

func Incineration(){
	if(rider)
	ObjectSetAction(rider,"Tumble");
	rider = 0;
}