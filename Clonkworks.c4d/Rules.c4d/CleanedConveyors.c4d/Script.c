/*-- Gefangennahme --*/

#strict 2

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

func Clean(){
	var Conveyors = FindObjects(Find_ID(CNVY),Find_Func("IsLocked"),Sort_Random());
	if(Conveyors && Conveyors[0])
		Conveyors[0]->Clean();
}