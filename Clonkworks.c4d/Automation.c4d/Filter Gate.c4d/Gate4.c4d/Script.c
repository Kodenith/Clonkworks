/*-- Neues Script --*/

#strict 2
#include FLTG
local Amount;

private func Move(){
	SetFilterGraph();
}

func RejectCollect(pId,pObj){
	var Left;
	if(GetX(pObj) < GetX()) Left = true;
	
	if(Amount){
		if(Left){
			Enter(this(),pObj);
			Exit(pObj,14,12,,2);
			Sound("Clonk");
		}else{
			Enter(this(),pObj);
			Exit(pObj,-14,12,,-2);
			Sound("Clonk");
		}
		Amount--;
	}else{
		if(Left){
			Enter(this(),pObj);
			Exit(pObj,-14,12,,-2);
			Sound("Clonk");
		}else{
			Enter(this(),pObj);
			Exit(pObj,14,12,,2);
			Sound("Clonk");
		}
	}
	
	if(GetDefFragile(GetID(pObj))){
		SetXDir(0,pObj);
		SetYDir(0,pObj);
	}
	return(1);
}

func ControlThrow(pObj){
	[$TxtTurn$|Image=WIPF]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this()) return(0);
	Sound("SignalClick");
	Amount++;
}

public func ContextDoTurn(pClonk){
	[$TxtTurn$|Image=WIPF|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlThrow");
	AddCommand(pClonk,"Grab",this());
}
public func SelectableGate(){
	return(1);
}

func Initialize() {
  Amount = 0;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  return(_inherited());
}

func ControlLeft(pObj){
	[$TxtMenu1$|Image=FMNS]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this()) return(0);
	Sound("SignalClick");
	Amount--;
}

public func ContextDoFilter(pClonk){
	[$TxtMenu1$|Image=FMNS|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlLeft");
	AddCommand(pClonk,"Grab",this());
}

private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}

func SetFilterGraph(){
	
  if(Amount < 0) Amount = 0;
  if(Amount > 99) Amount = 99;
  
	//digit 1
  var FilterImage = GetNumberID(Amount/10);
  SetGraphics(0,this(),FilterImage,ShadowID(),4);
  SetObjDrawTransform(600,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4-10000,0,600,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation (RGBa(255,255,255), this(), ShadowID()); 
  
  //digit 2
  var FilterImage = GetNumberID(Amount%10);
  SetGraphics(0,this(),FilterImage,MainID(),4);
  SetObjDrawTransform(600,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4+10000,0,600,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),MainID());
}