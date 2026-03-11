/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  if(FindObject(TIME))   return(["Wind Direction","Temperature","Time","Width","Height"]);
  return(["Wind Direction","Temperature","Width","Height"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "Wind Direction") return(GetWind(0,0,true));
   if(OutputName == "Temperature") return(GetTemperature());
   if(OutputName == "Width") return(LandscapeWidth());
   if(OutputName == "Height") return(LandscapeHeight());
   if(OutputName == "Time"){
      var time = Local(1,FindObject(TIME));
      if(!time) time = 0;
      time++;
      return(time/1000);
   }
  return(0);
}

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