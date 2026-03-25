/*-- Neues Script --*/

#strict

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

func IsAnvilProduct(){
	return(1);
}

func Activate(){
	[$TxtDetach$]
	var found;
	if(found = FindObject2(Find_OnLine(0,0,0,20),Find_NoContainer(),Find_Func("CanBeDetached"))){
		Sound("Wrench*");
		found->~OnDetach(Par(0));
	}
}