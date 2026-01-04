/*--- Bohrkopf ---*/

#strict

local Source;

func Collect(i,o){
	Enter(Source,o);
}

func Find(){
	var find;
	find = FindObjects(Find_AtPoint(),Find_OCF(OCF_Collectible),Find_NoContainer());
	for(var f in find){
		if(GetAlive(f)) return(0);
		Source->Sound("Clonk");
		Enter(Source,f);
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