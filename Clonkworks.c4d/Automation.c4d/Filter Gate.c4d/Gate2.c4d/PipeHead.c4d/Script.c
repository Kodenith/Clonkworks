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
	
	if(GetY() > GetY(Source)+150){
		SetAction("Stop");
		SetYDir(0);
		SetY(GetY()-1);
	}
}

func ContactBottom(){
	SetAction("Stop");
}