/*--- Bohrkopf ---*/

#strict
local Source;

func ContactBottom(){
	SetAction("Stop");
}

func FixPos(){
	if(GetY() > GetY(Source)+150){
		SetAction("Stop");
		SetYDir(0);
		SetY(GetY()-1);
	}
}