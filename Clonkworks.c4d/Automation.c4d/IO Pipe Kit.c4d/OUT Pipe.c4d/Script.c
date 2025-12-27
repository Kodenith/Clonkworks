/*-- Neues Script --*/

#strict 2
#include PPIN

func DoAttachment(){
	var found;
	if(found = FindObject2(Find_ID(PPIN),Find_Distance(5),Find_NoContainer())){
		var To = LocalN("Target",found);
		var Cont = Contents(0, Target);
		if(!Cont) return(_inherited());
		if(Enter(To,Cont)){
			Sound("Grapple");
		}
	}
	return(_inherited());
}