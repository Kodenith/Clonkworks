/*--- Spyglass ---*/

#strict 3

local LastCircuit;

protected func Hit()
{
  if(!Random(1))
    Sound("Crystal*");
  else Sound("WoodHit*");
  return(1);
}

public func Activate(pClonk){
  [$TxtUse$]
  Sound("SpellSysCombo1");
  var Circuit = FindObject2(Find_NoContainer(),Find_Func("IsCircuit"),Find_AtPoint());
  if(Circuit){
    ShowCircuitMenu(pClonk,Circuit);
  }else{
    ShowIDMenu(pClonk);
  } 

  return(1);
}

public func ShowCircuitMenu(Viewer,Circuit){
  //Initializing Menu
  CloseMenu(Viewer);
  CreateMenu(GetID(),Viewer,this,0,Format("$TxtMenu1$",GetName(Circuit)),0,1);
  LastCircuit = Circuit;

  var LoadInput = Circuit->WireTo();
  var LoadOutput = Circuit->WireFrom();
  if(!LoadInput && !LoadOutput){
    AddMenuItem("$Bummer$",nil,nil,Viewer);
    return(0);
  }

  //Input
  if(LoadInput){
  AddMenuItem("$CHead1$",nil,nil,Viewer);
  var Inputs = Circuit->InputList();
  for(var Input in Inputs){
      var cId = ROCK;
      if(Circuit->InputActive(Input)){
        cId = GOLD;
      }

      var Data = Circuit->InputActive(Input);
      if(GetType(Data) == C4V_Array) Data = "[Array Data]";

      AddMenuItem(Format("$CInput$",Input,Data),nil,cId,Viewer);
  }
  }

  //Output
  if(LoadOutput){
  AddMenuItem("$CHead2$",nil,nil,Viewer);
  var Outputs = Circuit->OutputList();
  for(var Output in Outputs){
      var cId = ROCK;
      if(Circuit->OutputActive(Output)){
        cId = GOLD;
      }

      var Data = Circuit->OutputActive(Output);
      if(GetType(Data) == C4V_Array) Data = "[Array Data]";

      var WireAmount = ObjectCount2(Find_ID(WIRE),Find_ActionTarget(Circuit));
      AddMenuItem(Format("$CInput$",Output,Data),nil,cId,Viewer,WireAmount);
  }
  }
}

private func UpdateMenu(){
  var Viewer = Contained();
  if(!Contained()) return(0);

  if(GetMenu(Viewer) == GetID()){
    ShowCircuitMenu(Viewer,LastCircuit);
  }
}

func IsAnvilProduct(){ return(1); }

public func ShowIDMenu(Viewer){
  CreateMenu(CC5_,Viewer,this,0,"$TxtMenu2$",0,1);
  LastCircuit = 0;

  var Objects = FindObjects(Find_NoContainer(),Find_Distance(65),Find_Exclude(this));
  var IdList = [];
  for(var Obj in Objects){
    ArrayAdd(IdList,GetID(Obj),1);
  }

  for(var Obj in IdList){
    AddMenuItem(Format("$IDesc$",GetName(,Obj),Obj),nil,Obj,Viewer);
  }
}