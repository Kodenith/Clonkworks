/*-- Neues Script --*/

#strict 3
#include IO__
#include CXEC
#include BAS1

public func WireFrom(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Push"]);
}

func Damage(){
  if(GetDamage() > 80 && !Locked) Explode(30);
}

func GetResearchBase(){ return(CC1_); }

local Head;

func Initialize(){
  Head = CreateObject(PIS2);
  Head->SetAction("Attached",this);
  return(_inherited());
}

func CheckPush(){
  if(GetCon() < 100) return(0);
  if(!Head) Initialize();

  if(InputActive("Push")) Head->DoPush();
}

//Classic
func ControlLeft(){
  [$TxtLeft$]
  SetAction("Left");
}

func ControlRight(){
  [$TxtLeft$]
  SetAction("Right");
}

func ControlDigSingle(){
  [$TxtStop$]
  SetAction("Idle");
    SetRDir(0,Head);
}

//jnr
public func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  if(comdir == COMD_Left) SetAction("Left");
  if(comdir == COMD_Right) SetAction("Right");
  if(comdir == COMD_Stop){
    SetAction("Idle");
    SetRDir(0,Head);
  }
}

//Movement 
func MoveLeft(){
  if(!Head) return(0);
  SetRDir(-3,Head);
}

func MoveRight(){
  if(!Head) return(0);
  SetRDir(3,Head);
}

func Grabbed(){
  if(Par(1)) SetRDir(0,Head);
}


func Destruction(){
  if(Head) RemoveObject(Head);
    return(_inherited());
}