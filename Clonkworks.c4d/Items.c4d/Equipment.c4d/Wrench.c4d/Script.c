/*-- Neues Script --*/

#strict

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

func isAnvilProduct(){
	return(1);
}

func Activate(){
	[$TxtDetach$]
	var found;
	if(found = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("CanBeDetached"))){
		Sound("Wrench*");
		found->~OnDetach();
	}
}