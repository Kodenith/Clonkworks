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

public func ContextDoFilter(pClonk){
	[$TxtMenu1$|Image=WIPF|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlLeft");
	AddCommand(pClonk,"Grab",this());
}

public func SelectableGate(){
	return(0);
}

func Initialize() {
  Amount = 0;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  return(_inherited());
}

func ControlLeft(pObj){
	[$TxtMenu1$|Image=WIPF]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this()) return(0);
	CreateMenu(GetID(),pObj,this(),,"$TxtMenu1$",,1);
	AddMenuItem("$TxtTurn$","DoValue",WIPF,pObj,0,pObj);
	AddMenuItem("$Decr$","DoValue",FMNS,pObj,0,pObj);
	AddMenuItem("$TxtFin$","DoValue",,pObj,Amount,pObj);
}

func DoValue(pid,pObj){
	if(GetProcedure(pObj) != "PUSH") return(CloseMenu(pObj));
	if(GetActionTarget(0,pObj) != this()) return(CloseMenu(pObj));
	if(!pid) return(CloseMenu(pObj));
	
	if(pid == WIPF){
		Sound("CatapultSet");
		Amount++;
	}
	
	if(pid == FMNS){
		Sound("CatapultSet");
		Amount--;
	}
	
	if(Amount < 0) Amount = 0;
	if(Amount > 99) Amount = 99;
	
	ControlLeft(pObj);
	if(pid == FMNS) SelectMenuItem(1,pObj);
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

func Malfunction(){
	FilterPath = !FilterPath;
	Filter = ERTH;
	Amount = RandomX(0,99);
}