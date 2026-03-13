/*-- Neues Script --*/

#strict 3

func Initialize() {
  
  return(1);
}

func RejectGrabbed(pByObject){
  SetCommand(pByObject,"Grab",GetActionTarget());
  return(1);
}

func UpdateSolid(){
  if(GetCon() < 100) return(0);
  if(GetAction() == "Attached") SetSolidMask(0,25,14,25);
  if(GetAction() == "Push") SetSolidMask();
  if(GetAction() == "Retract"){
    if(GetPhase() < 2) SetSolidMask();
    else SetSolidMask((7*14)-(GetPhase()*14),25,14,25);
  }
}

func Push(){
  var Fx,Fy,Tx,Ty;
  Fx = GetVertex(2,0);
  Fy = GetVertex(2,1);
  Tx = GetVertex(1,0);
  Ty = GetVertex(1,1);

  var Pushables = FindObjects(Find_NoContainer(),Find_OnLine(Fx,Fy,Tx,Ty),Find_Or(Find_Category(C4D_Object),Find_Category(C4D_Vehicle),Find_Category(C4D_Living)));
  var PushPower = 5;
  for(var pushed in Pushables){
    var tPushPower = PushPower;
    if(GetMass(pushed) >= 520) continue;
    if(GetMass(pushed) > 70) tPushPower = 4;
    if(GetMass(pushed) >= 120) tPushPower = 3;
    if(GetMass(pushed) >= 220) tPushPower = 2;
    if(GetMass(pushed) >= 320) tPushPower = 1;
    if(GetMass(pushed) >= 420) tPushPower = RandomX(0,1);
    Fling(pushed,Sin(GetR(), PushPower), -Cos(GetR(), tPushPower),nil,true);
  }
}

public func DoPush(){
  if(GetAction() != "Attached" || !EnergyCheck(10000)) return(false);
  DoEnergy(-10000);
  Sound("Chuff");
  SetAction("Push");
  return(true);
}

func WireRedirect(){
  return(GetActionTarget());
}