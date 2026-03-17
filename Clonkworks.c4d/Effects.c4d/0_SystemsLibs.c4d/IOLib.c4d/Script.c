/* INPUT OUTPUT BY KODENITH */
//wire color is 343
#strict 3

//special
local Locked;

//can this be wired from and to?
public func WireFrom(){ return(GetCon() > 99); }
public func WireTo(){ return(GetCon() > 99); }

//is this a circuit?
public func IsCircuit(){ return(1); }

//this function returns a list of string that can be input.
//default is two inputs
public func InputList(){
  return(["A","B"]);
}

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Out"]);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    return(1);
}

//check if the wire is ticking a certain input in this.
public func InputActive(string InputName){
  for(var Wire in FindObjects(Find_ID(WIRE))){
      if(GetActionTarget(1,Wire) == this){
          //if(Wire->GetOutputValue() == nil && Wire->ConnectedTo() == InputName) return(nil);
          if(Wire->GetOutputValue() != nil && Wire->ConnectedTo() == InputName) return(Wire->GetOutputValue());
      }
  }
}

//does an input have a wire connected to it?
public func DoesInputHaveWire(string InputName){
  for(var Wire in FindObjects(Find_ID(WIRE))){
      if(GetActionTarget(1,Wire) == this){
         if(Wire->ConnectedTo() == InputName) return(1);
      }
  }
}

/* WIRE CLEAN MENU */

public func ControlDigDouble(pClonk){
  [$TxtRemoveContext$|Image=IO__:1|Conditon=WireFrom]
  if(Locked) return(1);
    if(Hostile(GetOwner(),GetOwner(pClonk)) && !FindObject(WSAB)) return(1);
  if(GetAction(pClonk) != "Push" ) return(0);
  if(GetActionTarget(0,pClonk) != this) return(0);

  if(!WireFrom()){
    Message("$TxtCantRO$",pClonk);
    return(0);
  }

  CreateMenu(GetID(),pClonk,this,0,"$TxtRemoveMenu$",0,1);
  for(var Output in FindObjects(Find_ID(WIRE),Find_ActionTarget(this))){
    if(GetID(GetActionTarget(1,Output)) == WRKT) continue;
    AddMenuItem(Format("$TxtOutputOption$",LocalN("OutputString",Output),GetName(GetActionTarget(1,Output)),LocalN("InputString",Output)),Format("DisconnectWire(Object(%d),Object(%d))",ObjectNumber(Output),ObjectNumber(pClonk)),WIRE,pClonk);
  }
}

public func DisconnectWire(Wire,Clonk){
  if(Clonk){
      if(GetAction(Clonk) != "Push" ) return(0);
      if(GetActionTarget(0,Clonk) != this) return(0);
  }

  if(!GetActionTarget(1,Wire)) return(0);

  GetActionTarget(1,Wire)->Sound("Click");
  GetActionTarget(1,Wire)->CastObjects(WRKT,1,10);
  RemoveObject(Wire);
  Sound("Click");
  if(Clonk) ControlDigDouble(Clonk);
}

public func ContextClearWires(pClonk){
  [$TxtRemoveContext$|Image=IO__:1|Conditon=WireFrom]
      if(GetCon() < 100) return(1);
  AddCommand(pClonk,"Call",this,pClonk,,,,"ControlDigDouble");
  AddCommand(pClonk,"Grab",this);
}

/* COLORATION
only for context menu */

public func AllowRecolor(){ return(GetCon() > 99 && GetDefCoreVal("ColorByOwner","DefCore",GetID()) == 1); }

public func ContextRecolor(pClonk){
  [$TxtColorContext$|Image=WATR|Conditon=AllowRecolor]
    if(Locked) return(1);
      if(GetCon() < 100) return(1);
  if(Hostile(GetOwner(),GetOwner(pClonk)) && !FindObject(WSAB)) return(1);
  if(!AllowRecolor()){
    Message("$TxtCantRecolor$",pClonk);
    return(0);
  }

  if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this){
    AddCommand(pClonk,"Call",this,pClonk,,,,"ContextRecolor");
    AddCommand(pClonk,"Grab",this);
    return(0);
  }

  OpenColorPicker(pClonk,this,GetColorDw(),false,255);
}


public func ColorPickerCallback(Color, Clonk){
  //DebugLog("Test");
  if(GetAction(Clonk) != "Push" ) return(0);
  if(GetActionTarget(0,Clonk) != this) return(0);

  
  SetColorDw(Color);
  Message("$TxtColorSet$",this);
  Sound("Splash1");
}

public func Lock(state){
  Locked = state;
}

public func IsCircuitLocked(){ return(Locked); }

global func LockAllCircuits(){
  for(var i in FindObjects(Find_Func("IsCircuit"))) i->Lock(1);
}

global func LockAllCircuitsOwned(iPlr){
  for(var i in FindObjects(Find_Func("IsCircuit"),Find_Owner(iPlr))) i->Lock(1);
}

global func HideLockedCircuits(){
      for(var i in FindObjects(Find_Func("IsCircuit"),Find_Func("IsCircuitLocked"))) SetVisibility(VIS_None,i);
}

public func RejectGrabbed(pObj){
  if(Hostile(GetOwner(),GetOwner(pObj)) && !FindObject(WSAB)) return(1);
}

//dependance - created for castle walls :)
public func StartDependingOn(pObj){
  AddEffect("CircuitDependance",this,1,5,this,nil,pObj);
}

func FxCircuitDependanceStart(Target,EffectNum,Temp,Obj){
  EffectVar(0,Target,EffectNum) = Obj;
}

func FxCircuitDependanceTimer(Target,EffectNum,Time){
  var Depend = EffectVar(0,Target,EffectNum);
  if(!Depend || OnFire(Depend) || GetCon(Depend) < 100){
      DoDamage(999);
      return(-1);
  }
}