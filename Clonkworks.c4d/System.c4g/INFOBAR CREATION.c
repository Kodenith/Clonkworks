/*-- Neues Script --*/

#strict 2
#appendto CLNK

func Initialize(){
	if(!FindObject2(Find_ID(I_BR),Find_Owner(GetOwner()))) CreateObject(I_BR,,,GetOwner());
	return(_inherited());
}