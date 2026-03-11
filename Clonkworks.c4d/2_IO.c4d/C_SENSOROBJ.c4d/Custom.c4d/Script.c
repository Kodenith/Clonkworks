/*-- Neues Script --*/

#strict 3
#include I5_1

public func InputList(){
  return(["Range","Except","ID String","Contained"]);
}

//reusable function for all sensors :D
public func GetDetection(){
  var Aidee = InputActive("ID String");
  if(GetType(Aidee) != C4V_String) return(0);
  Aidee = C4Id(Aidee);
  if(!Aidee) return(0);
  var Object;
  if(!InputActive("Contained"))
  Object = FindObject2(Find_NoContainer(),Find_Distance(Range),Find_ID(Aidee),Find_Exclude(this),Find_Exclude(InputActive("Except")),Sort_Distance());
  else Object = FindObject2(Find_Distance(Range),Find_ID(Aidee),Find_Exclude(this),Find_Exclude(InputActive("Except")),Sort_Distance());
  return(Object);
}

func Detect(){
  var Type;
  if(InputActive("ID String")) Type = C4Id(InputActive("ID String"));
  else Type = RMMG;
  SetGraphics(nil,this(),Type,1,4);

  return(_inherited());
}