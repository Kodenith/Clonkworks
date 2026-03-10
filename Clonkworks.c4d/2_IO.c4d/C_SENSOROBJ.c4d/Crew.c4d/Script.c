/*-- Neues Script --*/

#strict 3
#include I5_1

public func InputList(){
  return(["Range","Except","Owned By (String)","Not Owned By (String)","Hostile to (String)","Friendly to (String)"]);
}

//reusable function for all sensors :D
public func GetDetection(){
  var Object;
  if(InputActive("Owned By (String)") && GetType(InputActive("Owned By (String)")) == C4V_String){
    Object = FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Distance(Range),Find_Owner(GetPlayerByName(InputActive("Owned By (String)"))),Find_Exclude(InputActive("Exclude")),Sort_Distance());
  }else if(InputActive("Not Owned By (String)") && GetType(InputActive("Not Owned By (String)")) == C4V_String){
    Object = FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Distance(Range),Find_Not(Find_Owner(GetPlayerByName(InputActive("Not Owned By (String)")))),Find_Exclude(InputActive("Exclude")),Sort_Distance());
  }else if(InputActive("Hostile to (String)") && GetType(InputActive("Hostile to (String)")) == C4V_String){
    Object = FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Distance(Range),Find_Hostile(GetPlayerByName(InputActive("Hostile to (String)"))),Find_Exclude(InputActive("Exclude")),Sort_Distance());
  }else if(InputActive("Friendly to (String)") && GetType(InputActive("Friendly to (String)")) == C4V_String){
    Object = FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Distance(Range),Find_Not(Find_Hostile(GetPlayerByName(InputActive("Friendly to (String)")))),Find_Exclude(InputActive("Exclude")),Sort_Distance());
  }else{
    Object = FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Distance(Range),Find_Exclude(InputActive("Exclude")),Sort_Distance());
  }
  return(Object);
}

func GetPlayerByName(Name){
  for(var i = 0; i < GetPlayerCount(); i++){
    if(GetPlayerName(GetPlayerByIndex(i)) == Name) return(GetPlayerByIndex(i));
  }
}