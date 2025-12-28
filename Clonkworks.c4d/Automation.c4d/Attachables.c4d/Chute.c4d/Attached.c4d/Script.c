/*-- Neues Script --*/

#strict 2
#include ACH1

func RejectCollect(idObj,pObj){
	return(Target->~RejectEntrance(pObj));
}

func Collection(pObj){
	Enter(Target,pObj);
	Sound("Clonk");
}