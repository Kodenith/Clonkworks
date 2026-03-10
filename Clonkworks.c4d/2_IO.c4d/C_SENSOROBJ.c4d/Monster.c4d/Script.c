/*-- Neues Script --*/

#strict 3
#include I5_1

//reusable function for all sensors :D
public func GetDetection(){
  var Object = FindObject2(Find_NoContainer(),Find_Distance(Range),Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Object),Find_Category(C4D_Vehicle),Find_Category(C4D_StaticBack)),Find_Or(Find_ID(MONS),Find_ID(FMNS)),Find_Exclude(this),Find_Exclude(InputActive("Except")),Sort_Distance());
  return(Object);
}