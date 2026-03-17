/*-- Neues Script --*/

#strict 3
#include IO__
#include CXEC

public func WireFrom(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Stab"]);
}

func Damage(){
  if(GetDamage() > 80 && !Locked) Explode(30);
}
func GetResearchBase(){ return(CC1_); }

func Initialize(){
  SetAction("Retract");
}

func CheckStab(){
  if(GetCon() < 100) return(0);
  if(GetAction() == "Stab"){
    if(GetPhase() >= 4 && !InputActive("Stab")){
      SetAction("Retract");
    }

      //falling on active spikes is also a no no
      var OnTip = FindObjects(Find_AtPoint(0,GetVertex(0,1)),Find_OCF(OCF_Alive),Find_Category(C4D_Living),Find_NoContainer());
      for(var Stabbed in OnTip){
          if(GetYDir(Stabbed) < 0) return(0);
          Punch(Stabbed,10);
      } 
  }

  if(GetAction() == "Retract"){
    if(GetPhase() >= 4 && InputActive("Stab") && EnergyCheck(EnergyCheck(10000))){
      SetAction("Stab");
      DoEnergy(-10000);
    }
  }
}

func Stab(){
  var Stabbed = FindObjects(Find_OCF(OCF_Alive),Find_Category(C4D_Living),Find_NoContainer(),Find_OnLine(0,GetVertex(0,1),0,10));
  if(Stabbed && GetLength(Stabbed) > 0) Sound("Rip");
  for(var i in Stabbed){
    Punch(i,RandomX(10,20));
  }

  StabSound();
}

func StabSound(){ Sound("SpikeStab"); }
