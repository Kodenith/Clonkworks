/*-- Neues Script --*/

#strict 2
#include FTGN

private func Move(){
	SetFilterGraph();
	
	var Item = Contents(0);
	if(!Item) return(0);
	if(!FilterPath){ //Filtered goes left
		if(Amount)
			Exit(Item,-12,12,,-2);
		else Exit(Item,12,12,,2);
	}else{ //Filtered goes right
		if(Amount)
			Exit(Item,12,12,,2);
		else Exit(Item,-12,12,,-2);
	}
	if(Amount) Amount--;
	if(GetDefFragile(GetID(Item))){
		SetXDir(0,Item);
		SetYDir(0,Item);
	}
}

func RejectCollect(pId,pObj){
	Enter(this(),pObj); //dont blow up or hurt fragile objects.
}

func Collection(pObj){
	Sound("Clonk");
}

func ControlThrow(pObj){
	[$TxtTurn$|Image=SG01:1]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	Sound("SignalClick");
	if(!FilterPath){
		FilterPath = true;
		Message("$TxtTurnRight$",this());
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

func SetFilterGraph(){
	
  if(Amount < 0) Amount = 0;
  if(Amount > 99) Amount = 99;
  
	//digit 1
  var FilterImage = GetNumberID(Amount/10);
  SetGraphics(0,this(),FilterImage,ShadowID(),4);
  SetObjDrawTransform(500,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4-10000,0,500,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation (RGBa(255,255,255), this(), ShadowID()); 
  
  //digit 2
  var FilterImage = GetNumberID(Amount%10);
  SetGraphics(0,this(),FilterImage,MainID(),4);
  SetObjDrawTransform(500,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4+10000,0,500,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),MainID());
}