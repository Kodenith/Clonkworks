#strict 2
#include CTW0

func AutoUse(){
  if(!ObjectCount2(Find_Container(this()),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive))){
    if(!cannon) return(0);
    if(cannon->~DangerAhead()){
      cannon->~AutoFire(this());
    }else{
      cannon->~AutoStop(this());
    }
  }
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  ChangeDef(CTW0);
  CreateObject(PRCS,0,3);
  Sound("Connect");
}