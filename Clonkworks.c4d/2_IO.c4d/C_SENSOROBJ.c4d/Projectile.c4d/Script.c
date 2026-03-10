/*-- Neues Script --*/

#strict 3
#include I5_1

public func InputList(){
  return(["Range","Except","Detect contained?"]);
}

//reusable function for all sensors :D
public func GetDetection(){
  var Objects = FindObjects(Find_Distance(Range),Find_Category(C4D_Object),Find_Exclude(this),Find_Exclude(InputActive("Except")),Sort_Distance());
  for(var i in Objects){
    if(Contained(i) && !InputActive("Detect contained?")) continue;
    if(GetDefExplosive(GetID(i))) return(i);
  }
}