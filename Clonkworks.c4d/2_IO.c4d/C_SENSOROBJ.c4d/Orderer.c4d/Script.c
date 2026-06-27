/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

func WireFrom(){ return(0); }

public func InputList(){
  return(["Object ID","Amount","Send Order","Clear Order"]);
}

local ItemID;
local Amount;
local TopAmount;
protected func Update(){
  if(InputActive("Send Order") && !ItemID){
    var TempID, TempAmount;

    TempID = InputActive("Object ID");
    if(!TempID || GetType(TempID) != C4V_String) return(0);
    TempID = C4Id(TempID);
    if(!TempID) return(0);

    TempAmount = InputActive("Amount");
    if(GetType(TempAmount) != C4V_Int) return(0);

    ItemID = TempID;
    Amount = TempAmount;
  }

  if(Amount == nil) Amount = 0;
  if(TopAmount == nil) TopAmount = 0;
  if(Amount > TopAmount) TopAmount = Amount;

  if(InputActive("Clear Order") || Amount == 0){
    ItemID = nil;
    Amount = 0;
    TopAmount = 0;
  }

  if(Amount < 0) Amount = 0;
  if(Amount && GetAction() != "Active") SetAction("Active");
  else if(!Amount && GetAction() == "Active") SetAction("Attached");

  var graphicsFace;
  if(ItemID && GetType(ItemID) == C4V_C4ID){
      graphicsFace = ItemID;
  }else{
      graphicsFace = _MRK;
  }

  SetGraphics(nil,this,graphicsFace,1,4);
  SetObjDrawTransform(800,0,-15000,0,800,0,this,1);
}

func MustBeOnWall(){ return(1); }

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50 && !Locked) OnDetach();
}
func CanBeDetached(){ return(!Locked); }
func OnDetach(pClonk){
  if(pClonk && Hostile(GetOwner(),GetController(pClonk)) && !FindObject(WSAB)) return(0);
  if(Locked) return(0);
  Sound("Connect");
  Split2Components();
}

//INFOBAR
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(TopAmount);
}

public func InfobarValue(){
	return(Amount);
}

public func InfobarColor(){
	  return(RGBa(0,255,255));
}

public func InfobarInfo(){
  if(!ItemID || !Amount) return("$NoOrd$");
	return(Format("$Ord$",ItemID,Amount));
}

//Automatic depositing stuff
func AutoDepositHere(){ return(1); }

func AD_NeedItem(pStation){
  if(!Amount || !ItemID) return(0);
	return(ItemID);
}

func ConfirmSent(){
  Amount--;
}