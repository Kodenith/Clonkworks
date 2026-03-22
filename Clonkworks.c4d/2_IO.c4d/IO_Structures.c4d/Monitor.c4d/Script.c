/*-- Neues Script --*/

#strict 3
#include IO__
#include CXEC

public func WireFrom(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Cam 01","Cam 02","Cam 03"]);
}

func GetResearchBase(){ return(CC5_); }

local Channel;
func Initialize(){
  Channel = 1;
}

private func CheckCam(){
  if(!EnergyCheck(1)){
    SetAction("Idle");
    for(var i in FindObjects(Find_Action("Push"),Find_ActionTarget(this))){
        if(GetCursor(GetController(i)) == i){
            SetPlrView(GetController(i),i);
        }
    }
    return(0);
  }

  var Cam;
  Channel = BoundBy(Channel,1,3);
  if(Channel == 1) Cam = InputActive("Cam 01");
  if(Channel == 2) Cam = InputActive("Cam 02");
  if(Channel == 3) Cam = InputActive("Cam 03");

  if(!Cam || GetType(Cam) != C4V_C4Object || Cam->~HasCamera() != 1){
    if(GetAction() != "NoConnection") SetAction("NoConnection");
    for(var i in FindObjects(Find_Action("Push"),Find_ActionTarget(this))){
        if(GetCursor(GetController(i)) == i){
            SetPlrView(GetController(i),i);
        }
    }
  }else{
    if(GetAction() != "Displaying"){
      SetAction("Displaying");
    }

    DisplayCam();

    for(var i in FindObjects(Find_Action("Push"),Find_ActionTarget(this))){
        if(GetCursor(GetController(i)) == i){
            SetPlrView(GetController(i),Cam);
        }
    }
  }
  
}

func ControlThrow(pClonk){
  [$TxtChange$]
  Channel++;
  if(Channel > 3) Channel = 1;
  Message("$TxtChannelSet$",this,Channel);
  CheckCam();
  Sound("Click");
}

//INFOBAR
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(3);
}

public func InfobarValue(){
	return(Channel);
}

public func InfobarColor(){
  return(GetColorDw());
}

public func InfobarInfo(){
	return(Format("$TxtChannel$",Channel));
}

private func DisplayCam(){
  var Cam;
  if(Channel == 1) Cam = InputActive("Cam 01");
  if(Channel == 2) Cam = InputActive("Cam 02");
  if(Channel == 3) Cam = InputActive("Cam 03");

  SetGraphics(nil,this(),GetID(Cam),2,4);
  SetObjDrawTransform(500,0,0,0,500,0,this,2);
}

private func ClearDisplay(){
  SetObjDrawTransform(0,0,0,0,0,0,this,2);
}

func Malfunction(){
  Incinerate();
}