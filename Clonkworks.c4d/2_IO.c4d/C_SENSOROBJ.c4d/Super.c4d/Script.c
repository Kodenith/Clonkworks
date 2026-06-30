/* Super Sensor */
//it's logic is a lot different than for most sensors.

#strict 3
#include IO__

public func OutputList(){
  return(["Detected","Detected Object","Iterated Object"]);
}

public func InputList(){
  return(["Range","X Offset","Y Offset","Related Object","Filter ID","Filter OCF","Filter Category","Contained/Contained Inside Of","Exclude Object or ID","Doing Action"]);
}

func OutputActive(iName){
  var Range;
  if(InputActive("Range") && GetType(InputActive("Range")) == C4V_Int){
    Range = BoundBy(InputActive("Range"),1,450);
  }else {
    return(0); // a range is required
  }

  var XOff = 0;
  if(InputActive("X Offset") && GetType(InputActive("X Offset")) == C4V_Int) XOff = InputActive("X Offset");
  var YOff = 0;
  if(InputActive("Y Offset") && GetType(InputActive("Y Offset")) == C4V_Int) YOff = InputActive("Y Offset");

  var Parent = InputActive("Related Object");
  if(!Parent || GetType(Parent) != C4V_C4Object){
    Parent = this;
  }

  var IDCom = nil;
  if(InputActive("Filter ID") && GetType(InputActive("Filter ID")) == C4V_String){
    var cId = C4Id(InputActive("Filter ID"));
    if(cId && GetType(cId) == C4V_C4ID) IDCom = Find_ID(cId);
  }

  var OCFcom = nil;
  if(InputActive("Filter OCF") && GetType(InputActive("Filter OCF")) == C4V_Int){
    OCFcom = Find_OCF(InputActive("Filter OCF"));
  }

  var CATcom = nil;
  if(InputActive("Filter Category") && GetType(InputActive("Filter Category")) == C4V_Int){
    CATcom = Find_OCF(InputActive("Filter Category"));
  }

  var ContainCom = nil;
  if(InputActive("Contained/Contained Inside Of")){
    var Thing = InputActive("Contained/Contained Inside Of");
    if(GetType(Thing) == C4V_C4Object) ContainCom = Find_Container(Thing);
    else if(GetType(Thing) == C4V_Int && Thing > 0) ContainCom = Find_AnyContainer();
    else if(GetType(Thing) == C4V_Int && Thing < 0) ContainCom = Find_NoContainer();
  }

  var ExcludedCom = nil;
  if(InputActive("Exclude Object or ID") && GetType(InputActive("Exclude Object or ID")) == C4V_C4Object) ExcludedCom = Find_Exclude(InputActive("Exclude Object or ID"));
  else if(InputActive("Exclude Object or ID") && GetType(InputActive("Exclude Object or ID")) == C4V_C4ID) ExcludedCom = Find_Not(Find_ID(InputActive("Exclude Object or ID")));

  var ActCom = nil;
  if(InputActive("Doing Action") && GetType(InputActive("Doing Action")) == C4V_String) ActCom = Find_Action(InputActive("Doing Action"));

  //since nils stop further checks, i put everything into an array. nils go last.
  var Criteria = [];
  var AddNils = 0;
  if(IDCom) ArrayAdd(Criteria,IDCom);
  else AddNils++;
  if(OCFcom) ArrayAdd(Criteria,OCFcom);
  else AddNils++;
  if(CATcom) ArrayAdd(Criteria,CATcom);
  else AddNils++;
  if(ContainCom) ArrayAdd(Criteria,ContainCom);
  else AddNils++;
  if(ExcludedCom) ArrayAdd(Criteria,ExcludedCom);
  else AddNils++;
  if(ActCom) ArrayAdd(Criteria,ActCom);
  else AddNils++;
  for(var x = 0; x < AddNils; x++){
    ArrayAdd(Criteria,nil);
  }

  var ObjectList = FindObjects(Find_Distance(Range,AbsX(GetX(Parent))+XOff,AbsY(GetY(Parent))+YOff),Criteria[0],Criteria[1],Criteria[2],Criteria[3],Criteria[4],Criteria[5],Find_Not(Find_ID(GetID(Parent))),Sort_Distance());
  if(!ObjectList) return(0);
  if(iName == "Detected"){
    if(ObjectList[0]) return(1);
  }else if(iName == "Detected Object"){
    return(ObjectList[0]);
  }else if(iName == "Iterated Object"){
    var Index = FrameCounter()%GetLength(ObjectList);
    return(ObjectList[Index]);
  }
}

func MustBeOnWall(){ return(1); }

func Detect(){
  //i only ever use this for state checks
  if(OutputActive("Detected") && GetAction() == "Attached"){
    SetAction("Active");
    if(!Locked) Sound("Command");
  }else if(!OutputActive("Detected") && GetAction() != "Attached"){
    SetAction("Attached");
    if(!Locked) Sound("Command");
  }

  if(FindObject2(Find_ActionTarget(this),Find_Action("Push")) && FrameCounter()%15 == 0)
    DrawRangeCircle();
}

private func DrawRangeCircle(){
  var Range = InputActive("Range");
  if(GetType(Range) != C4V_Int) return(0);

  var Parent = InputActive("Related Object");
  if(!Parent || GetType(Parent) != C4V_C4Object){
    Parent = this;
  }

  var XOff = 0;
  if(InputActive("X Offset") && GetType(InputActive("X Offset")) == C4V_Int) XOff = InputActive("X Offset");
  var YOff = 0;
  if(InputActive("Y Offset") && GetType(InputActive("Y Offset")) == C4V_Int) YOff = InputActive("Y Offset");

  for(var x = 180; x >= -180; x-=2){
    if(!Random(3)) Parent->CreateParticle("NoGravSpark", Sin(x, Range, 1)+XOff, Cos(x, Range, 1)+YOff , Sin(x, 0, 1), Cos(x, 0, 1), 25, RGBa(0,255,0));
  }
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