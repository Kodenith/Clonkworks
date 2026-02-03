/*-- Neues Script --*/

#strict 2
#include FLTG

private func Move(){
	SetFilterGraph();
}

func SetFilterGraph(){
	
  var FilterImage = Filter;
  if(Filter == 1) FilterImage = WRKS;
  if(Filter == 2) FilterImage = FNDR;
  if(Filter == 3) FilterImage = L_FU;
  if(Filter == 4) FilterImage = RVLT;
  if(Filter == 5) FilterImage = FLAM;
  if(Filter == 6) FilterImage = MAM5;
  if(Filter == 7) FilterImage = S_DC;
  
	//outline
  SetGraphics(0,this(),FilterImage,ShadowID(),4);
  SetObjDrawTransform(500,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4,0,500,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation(RGBa(0,0,1),this(),ShadowID());
  
  //main
  SetGraphics(0,this(),FilterImage,MainID(),4);
  SetObjDrawTransform(400,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4,0,400,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4)-10000,this(),MainID());
}

func RejectCollect(pId,pObj){
	if(Contained(pObj)) return(1);
	var Left;
	if(GetX(pObj) < GetX()) Left = true;
	if(Inside(GetX(pObj),GetX()-14,(GetX()-14)+29) && Inside(GetY(pObj),GetY()+1,(GetY()+1)+11))
		Enter(this(),pObj);
	else return(1);
	Sound("Clonk");
	
	var SideExit;
	if(FilterCompatibleItem(pObj) && !FilterPath) SideExit = true;
	if(FilterCompatibleItem(pObj) && FilterPath) SideExit = false;
	if(!FilterCompatibleItem(pObj) && !FilterPath) SideExit = false;
	if(!FilterCompatibleItem(pObj) && FilterPath) SideExit = true;
	
	if(SideExit){
		var Item = pObj;
		if(Left) Exit(Item,14,12,,2);
		else Exit(Item,-14,12,,-2);
		if(GetDefFragile(GetID(Item))){
			SetXDir(0,Item);
			SetYDir(0,Item);
		}
	}else{
		Enter(this,pObj);
		if(Left) 
		 Exit(pObj,0,-10,,2,-2);
		else Exit(pObj,0,-10,,-2,-2);
	}
	
	return(1);
}

func ControlThrow(pObj){
	[$TxtTurn$|Image=SG01:1]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	Sound("SignalClick");
	if(!FilterPath){
		FilterPath = true;
		Message("$TxtTurnDown$",this());
	}else{
		FilterPath = false;
		Message("$TxtTurnLeft$",this());
	}
}

public func ContextDoTurn(pClonk){
	[$TxtTurn$|Image=SG01:1|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlThrow");
	AddCommand(pClonk,"Grab",this());
}

public func SelectableGate(){
	return(1);
}

func Initialize() {
  if(!Filter)   Filter = 6;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  return(_inherited());
}
