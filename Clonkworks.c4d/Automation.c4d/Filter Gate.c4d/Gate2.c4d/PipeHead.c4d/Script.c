/*--- Bohrkopf ---*/

#strict

local Source;

func Collect(i,o){
	Enter(Source,o);
}

func Find(){
	var find;
	if(find = FindObject2(Find_AtPoint(),Find_OCF(OCF_Collectible),Find_NoContainer())){
		if(GetAlive(find)) return(0);
		Source->Sound("Clonk");
		Enter(Source,find);
	}
}

func ContactBottom(){
	SetAction("Stop");
}